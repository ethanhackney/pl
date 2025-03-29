#include "lib.h"
#include "scope.h"
#include "value.h"
#include <unordered_set>

using std::string;

static scope global_scope;
scope *curr_scope = &global_scope;

scope::scope(void)
{
        _parent = nullptr;
}

scope::scope(scope *parent)
{
        _parent = parent;
}

scope::~scope()
{
        std::unordered_set<value*> freed;

        for (auto &p : _tab) {
                if (freed.find(p.second) == freed.end()) {
                        delete p.second;
                        freed.insert(p.second);
                }
        }
}

value *scope::get(const string& name)
{
        auto p = _tab.find(name);
        if (p != _tab.end())
                return p->second;

        if (_parent)
                return _parent->get(name);

        return nullptr;
}

void scope::set(const string& name, value *v)
{
        std::unordered_set<int> simple_type {
                VAL_STR,
                VAL_INT,
        };

        if (_tab.find(name) != _tab.end())
                free(name);

        if (simple_type.find(v->type()) == simple_type.end())
                _ref[v]++;

        _tab[name] = v;
}

void scope::dump(int space)
{
        printf("{\n");

        for (const auto &p : _tab) {
                indent(space + 2);
                printf("[%s]: ", cstr(p.first));
                p.second->dump(space + 4);
        }

        indent(space - 2);
        printf("}\n");
}

scope *scope::parent(void)
{
        return _parent;
}

void scope::free(const std::string& name)
{
        std::unordered_set<int> simple_type {
                VAL_STR,
                VAL_INT,
        };

        auto curr = _tab[name];

        if (simple_type.find(curr->type()) != simple_type.end()) {
                delete curr;
                return;
        }

        _ref[curr]--;
        if (_ref[curr] == 0)
                delete curr;
}

const std::unordered_map<std::string,value*>& scope::tab(void) const
{
        return _tab;
}
