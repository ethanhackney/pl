#include "lib.h"
#include "scope.h"
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
        if (_tab.find(name) != _tab.end()) {
                auto curr = _tab[name];
                if (curr->type() != VAL_ARR) {
                        delete curr;
                } else {
                        _ref[curr]--;
                        if (_ref[curr] == 0)
                                delete curr;
                }
        }

        if (v->type() == VAL_ARR)
                _ref[v]++;

        _tab[name] = v;
}

void scope::dump(void)
{
        printf("{\n");

        for (const auto &p : _tab) {
                indent(2);
                printf("[%s]: ", p.first.c_str());
                p.second->dump(4);
        }

        printf("}\n");
}

scope *scope::parent(void)
{
        return _parent;
}
