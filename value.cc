#include "lib.h"
#include "value.h"
#include "scope.h"
#include <array>
#include <algorithm>

using std::string;
using std::vector;

static const std::array<string,NR_VAL> VAL_NAMES {
        "VAL_ERR",
        "VAL_STR",
        "VAL_INT",
        "VAL_ARR",
        "VAL_NIL",
        "VAL_FUNC",
        "VAL_STRUCT",
        "VAL_STRUCT_DEF",
};

value::value(void)
{
        init(VAL_ERR, "", 0, nullptr, nullptr);
}

value::value(int type)
{
        init(type, "", 0, nullptr, nullptr);
}

value::value(int type, long i)
{
        init(type, "", i, nullptr, nullptr);
}

value::value(int type, const string& s)
{
        init(type, s, 0, nullptr, nullptr);
        if (type == VAL_STRUCT_DEF)
                _members = new scope{curr_scope};
}

value::value(int type, value *vp)
{
        init(type, "", 0, vp, nullptr);
}

value::value(int type, scope *parent)
{
        init(type, "", 0, nullptr, nullptr);
        _members = new scope{parent};
}

value::~value()
{
        delete _func;
        delete _members;
        if (_type == VAL_STRUCT_DEF)
                delete _def;

        for (auto &vp : _arr)
                delete vp;
}

void value::init(int type, const string& s, long i, value *vp, ast *func)
{
        _def = nullptr;
        _members = nullptr;
        _func = func;
        _ptr = vp;
        _i = i;
        _type = type;
        _s = s;
        if (!type_ok(VAL_ERR, NR_VAL, _type))
                usage("value::init(): invalid value type: %d", _type);
}

const string& value::s(void) const
{
        return _s;
}

int value::type(void) const
{
        return _type;
}

const string& value::name(void) const
{
        return VAL_NAMES.at(_type);
}

long value::i(void) const
{
        return _i;
}

void value::push(value *vp)
{
        _arr.push_back(vp);
}

const vector<value*>& value::arr(void) const
{
        return _arr;
}

vector<value*>& value::arr_non_const(void)
{
        return _arr;
}

void value::set_s(const string& s)
{
        _s = s;
}

void value::set_i(long i)
{
        _i = i;
}

void value::dump(int space)
{
        printf("{\n");

        indent(space + 2);
        printf("type: %s,\n", name().c_str());

        if (_type == VAL_STRUCT_DEF) {
                indent(space + 2);
                printf("def: ");
                _def->dump(space + 4);
        }

        if (_type == VAL_STRUCT) {
                indent(space + 2);
                printf("def: ");
                _def->dump(space + 4);

                indent(space + 2);
                printf("members: ");
                _members->dump(space + 4);
        }

        if (_type == VAL_STR) {
                indent(space + 2);
                printf("s: %s,\n", _s.c_str());
        }

        if (_type == VAL_INT) {
                indent(space + 2);
                printf("i: %ld,\n", _i);
        }

        if (_type == VAL_ARR) {
                indent(space + 2);
                printf("arr: [\n");
                for (const auto &ep : _arr) {
                        indent(space + 4);
                        ep->dump(space + 6);
                }
                indent(space + 2);
                printf("],\n");
        }

        if (_type == VAL_FUNC) {
                indent(space + 2);
                printf("func: ");
                _func->dump(space + 4);
        }

        indent(space - 2);
        printf("},\n");
}

void value::arr_set(size_t i, value *vp)
{
        if (i >= _arr.size())
                usage("value::arr_set(): array index out of range: %zu", i);

        if (_arr[i])
                delete _arr[i];

        _arr[i] = vp;
}

value *value::copy(void)
{
        if (_type == VAL_ARR)
                return this;

        if (_type == VAL_FUNC)
                return this;

        if (_type == VAL_STRUCT)
                return this;

        if (_type == VAL_STR)
                return new value{VAL_STR, _s};

        return new value{VAL_INT, _i};
}

value *value::arr_get(size_t i)
{
        if (i >= _arr.size())
                usage("value::arr_set(): array index out of range: %zu", i);

        return _arr[i];
}

bool value::truthy(void) const
{
        if (_type == VAL_INT)
                return _i != 0;

        if (_type == VAL_STR)
                return _s.size() != 0;

        if (_type == VAL_NIL)
                return false;

        return true;
}

void value::print(void)
{
        if (_type == VAL_STR)
                printf("%s", _s.c_str());

        if (_type == VAL_INT)
                printf("%ld", _i);

        if (_type == VAL_ARR) {
                printf("[ ");
                for (const auto &vp : _arr)
                        vp->print();
                printf("]");
        }

        // pretty print a function
}

void value::arr_push(value *vp)
{
        _arr.push_back(vp);
}

void value::arr_sort(void)
{
        auto first = _arr[0]->type();
        for (size_t i = 1; i < _arr.size(); i++) {
                if (_arr[i]->type() != first)
                        usage("value::arr_sort(): cannot sort non-homogonous array");
        }

        if (first == VAL_STR) {
                std::sort(_arr.begin(), _arr.end(), [](value *a, value *b) {
                        return a->s() < b->s();
                });
        } else {
                std::sort(_arr.begin(), _arr.end(), [](value *a, value *b) {
                        return a->i() < b->i();
                });
        }
}

value::value(int type, ast *func)
{
        init(type, "", 0, nullptr, func);
}

ast *value::func(void)
{
        return _func;
}

value *value::member(const std::string& s)
{
        return _members->get(s);
}

void value::set_member(const std::string& s, value *vp)
{
        _members->set(s, vp);
}

scope *value::members(void)
{
        return _members;
}

bool value::agg(void)
{
        return _type == VAL_ARR || _type == VAL_STRUCT;
}

void value::set_struct_def(value *d)
{
        _struct_def = d;
}

void value::set_def(ast *p)
{
        _def = p;
}

ast *value::def(void)
{
        return _def;
}

void value::set_func(ast *p)
{
        _func = p;
}
