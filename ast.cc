#include "ast.h"
#include "lib.h"
#include "scope.h"
#include "value.h"
#include <cstdio>
#include <unordered_set>

using std::string;
using std::vector;

ast::ast(void)
{
}

ast::ast(int type, long i)
        : _i {i},
        _type {type}
{
        typeok();
}

void ast::typeok(void) const
{
        in_range(AST_ERR, NR_AST, _type, "invalid ast type: %d", _type);
}

ast::ast(int type)
        : _type {type}
{
        typeok();
}

ast::ast(int type, const string& str)
        : _str {str},
        _type {type}
{
        typeok();
}

ast::~ast()
{
        delete _expr;
        delete _left;
        delete _right;

        for (auto &ap : _idx)
                delete ap;

        for (auto &ap : _stmts)
                delete ap;

        for (auto &ap : _arr)
                delete ap;

        for (auto &ap : _args)
                delete ap;
}

int ast::type(void) const
{
        return _type;
}

const ast *ast::expr(void) const
{
        return _expr;
}

void ast::set_expr(ast *expr)
{
        _expr = expr;
}

const string& ast::name(void) const
{
        static const string names[NR_AST] {
                "AST_ERR",
                "AST_ASSIGN",
                "AST_STR",
                "AST_INT",
                "AST_ARR",
                "AST_ARR_ASSIGN",
                "AST_NIL",
                "AST_IF",
                "AST_WHILE",
                "AST_SUB",
                "AST_VAR_REF",
                "AST_LT",
                "AST_ADD",
                "AST_LOG_AND",
                "AST_MUL",
                "AST_PRINT",
                "AST_MEMBER_ASSIGN",
                "AST_ARR_REF",
                "AST_FUNC_DEF",
                "AST_FUNC_CALL",
                "AST_EQ",
                "AST_RET",
                "AST_STRUCT_DEF",
                "AST_CTOR",
        };
        return names[_type];
}

const string& ast::str(void) const
{
        return _str;
}

long ast::i(void) const
{
        return _i;
}

void ast::push(ast *ap)
{
        _arr.push_back(ap);
}

void ast::dump(int space) const
{
        printf("{\n");

        indent(space + 2);
        printf("type: %s,\n", cstr(name()));

        if (_type == AST_CTOR) {
                indent(space + 2);
                printf("str: %s,\n", cstr(_str));
        }

        if (_type == AST_STRUCT_DEF) {
                indent(space + 2);
                printf("str: %s,\n", cstr(_str));

                indent(space + 2);
                printf("stmts: [\n");
                for (const auto &ap : _stmts) {
                        indent(space + 4);
                        ap->dump(space + 6);
                }

                indent(space + 2);
                printf("],\n");
        }

        if (_type == AST_RET) {
                indent(space + 2);
                printf("expr: ");
                _expr->dump(space + 4);
        }

        if (_type == AST_VAR_REF) {
                indent(space + 2);
                printf("str: %s,\n", cstr(_str));
        }

        if (_type == AST_ASSIGN) {
                indent(space + 2);
                printf("str: %s,\n", cstr(_str));

                indent(space + 2);
                printf("expr: ");
                _expr->dump(space + 4);
        }

        if (_type == AST_PRINT) {
                indent(space + 2);
                printf("expr: ");
                _args[0]->dump(space + 4);
        }

        if (_type == AST_STR) {
                indent(space + 2);
                printf("str: %s,\n", cstr(_str));
        }

        if (_type == AST_INT) {
                indent(space + 2);
                printf("i: %ld,\n", _i);
        }

        if (_type == AST_ARR) {
                indent(space + 2);
                printf("arr: [\n");
                for (const auto &ep: _arr) {
                        indent(space + 4);
                        ep->dump(space + 6);
                }
                indent(space + 2);
                printf("],\n");
        }

        if (_type == AST_ARR_ASSIGN) {
                indent(space + 2);
                printf("str: %s,\n", cstr(_str));

                indent(space + 2);
                printf("idx: [\n");
                for (const auto &ep: _idx) {
                        indent(space + 4);
                        ep->dump(space + 6);
                }
                indent(space + 2);
                printf("],\n");

                indent(space + 2);
                printf("expr: ");
                _expr->dump(space + 4);
        }

        if (_type == AST_IF) {
                indent(space + 2);
                printf("expr: ");
                _expr->dump(space + 4);

                indent(space + 2);
                printf("stmts: [\n");
                for (const auto &ap : _stmts) {
                        indent(space + 4);
                        ap->dump(space + 6);
                }

                indent(space + 2);
                printf("],\n");
        }

        if (_type == AST_WHILE) {
                indent(space + 2);
                printf("expr: ");
                _expr->dump(space + 4);

                indent(space + 2);
                printf("stmts: [\n");
                for (const auto &ap : _stmts) {
                        indent(space + 4);
                        ap->dump(space + 6);
                }

                indent(space + 2);
                printf("],\n");
        }

        if (_type == AST_MEMBER_ASSIGN) {
                indent(space + 2);
                printf("members: [\n");

                for (const auto &s : _members) {
                        indent(space + 4);
                        printf("%s,\n", cstr(s));
                }

                indent(space + 2);
                printf("],\n");
        }

        if (_type == AST_FUNC_DEF) {
                indent(space + 2);
                printf("str: %s,\n", cstr(_str));

                indent(space + 2);
                printf("params: [\n");
                for (const auto &s : _params) {
                        indent(space + 4);
                        printf("%s,\n", cstr(s));
                }
                indent(space + 2);
                printf("],\n");

                indent(space + 2);
                printf("stmts: [\n");
                for (const auto &ap : _stmts) {
                        indent(space + 4);
                        ap->dump(space + 6);
                }

                indent(space + 2);
                printf("],\n");
        }

        if (_type == AST_ARR_REF) {
                indent(space + 2);
                printf("str: %s,\n", cstr(_str));

                indent(space + 2);
                printf("idx: [\n");
                for (const auto &ep: _idx) {
                        indent(space + 4);
                        ep->dump(space + 6);
                }
                indent(space + 2);
                printf("],\n");
        }

        if (_type == AST_FUNC_CALL) {
                indent(space + 2);
                printf("str: %s,\n", cstr(_str));

                indent(space + 2);
                printf("args: [\n");
                for (const auto &ap : _args) {
                        indent(space + 4);
                        ap->dump(space + 6);
                }

                indent(space + 2);
                printf("]\n");
        }


        std::unordered_set<int> binops {
                AST_LT,
                AST_LOG_AND,
                AST_ADD,
                AST_SUB,
                AST_MUL,
                AST_EQ,
        };

        if (binops.find(_type) != binops.end()) {
                indent(space + 2);
                printf("left: ");
                _left->dump(space + 4);

                indent(space + 2);
                printf("right: ");
                _right->dump(space + 4);
        }

        indent(space - 2);
        printf("},\n");
}

value *ast::expr_eval(void) const
{
        value *vp = nullptr;

        if (_type == AST_CTOR) {
                vp = new value{VAL_STRUCT};
                vp->set_def(curr_scope->get(_str)->def());

                ast *ctor = nullptr;
                for (auto &ap : vp->def()->stmts()) {
                        if (ap->str() == "ctor")
                                ctor = ap;
                        else if (ap->type() != AST_FUNC_DEF)
                                vp->set_member(ap->str(), ap->expr()->expr_eval());
                }

                auto tmp = curr_scope;
                curr_scope = vp->members();

                auto s = new scope{curr_scope};
                for (size_t i = 0; i < ctor->n_params(); i++)
                        s->set(ctor->param(i), _args[i]->expr_eval());

                curr_scope = s;
                for (auto &ap : ctor->stmts())
                        ap->eval();

                delete s;
                curr_scope = tmp;
        } else if (_type == AST_FUNC_CALL) {
                vp = call();
        } else if (_type == AST_SUB || _type == AST_ADD || _type == AST_MUL) {
                if (_type == AST_ADD && expr_type() == AST_STR) {
                        vp = new value{VAL_STR, str_expr()};
                } else {
                        vp = new value{VAL_INT, math_eval()};
                }
        } else if (_type == AST_LT || _type == AST_EQ) {
                vp = new value{VAL_INT, logic_eval()};
        } else if (_type == AST_LOG_AND) {
                vp = new value{VAL_INT, logic_eval()};
        } else if (_type == AST_NIL) {
                vp = new value{VAL_NIL};
        } else if (_type == AST_STR) {
                vp = new value{VAL_STR, _str};
        } else if (_type == AST_INT) {
                vp = new value{VAL_INT, _i};
        } else if (_type == AST_ARR) {
                vp = new value{VAL_ARR};
                for (const auto &ap : _arr)
                        vp->push(ap->expr_eval());
        } else if (_type == AST_VAR_REF) {
                vp = var_ref()->copy();
        } else if (_type == AST_ARR_REF) {
                vp = arr_ref()->copy();
        } else {
                usage("ast::expr_eval(): bad expression type: %s", cstr(name()));
        }

        return vp;
}

value *ast::eval(void) const
{
        if (_type == AST_STRUCT_DEF) {
                auto vp = new value{VAL_STRUCT_DEF, _str};
                vp->set_def(copy());
                curr_scope->set(_str, vp);
                return nullptr;

        }

        if (_type == AST_RET)
                return _expr->expr_eval();

        if (_type == AST_FUNC_CALL) {
                auto vp = call();
                delete vp;
                return nullptr;
        }

        if (_type == AST_FUNC_DEF) {
                auto vp = new value{VAL_FUNC, copy()};
                curr_scope->set(_str, vp);
                return nullptr;
        }

        if (_type == AST_PRINT) {
                for (const auto &p : _args) {
                        auto res = p->expr_eval();
                        res->print();
                        delete res;
                }
                return nullptr;
        }

        if (_type == AST_IF) {
                auto res = _expr->expr_eval();
                value *vp = nullptr;
                if (res->truthy()) {
                        for (const auto &ap : _stmts) {
                                vp = ap->eval();
                                if (vp)
                                        break;
                        }
                }
                delete res;
                return vp;
        }

        if (_type == AST_WHILE) {
                auto res = _expr->expr_eval();
                value *vp = nullptr;
                while (res->truthy()) {
                        for (const auto &ap : _stmts) {
                                vp = ap->eval();
                                if (vp)
                                        break;
                        }
                        delete res;
                        res = _expr->expr_eval();
                }
                delete res;
                return nullptr;
        }

        if (_type == AST_ASSIGN) {
                auto id = _str;
                auto vp = _expr->expr_eval();
                curr_scope->set(id, vp);
                return nullptr;
        }

        if (_type == AST_ARR_ASSIGN) {
                auto id = _str;

                auto vp = curr_scope->get(id);
                if (!vp)
                        usage("ast::ast_eval(): undefined array: %s", cstr(id));

                if (curr_scope->get(id)->type() != VAL_ARR)
                        usage("ast::ast_eval(): trying to index non array");

                for (size_t i = 0; i < _idx.size() - 1; i++) {
                        auto idx = _idx[i]->expr_eval();
                        if (idx->type() != VAL_INT)
                                usage("ast::ast_eval(): array index must be an integer");

                        if (vp->type() != VAL_ARR)
                                usage("ast::ast_eval(): trying to index non array");

                        vp = vp->arr_get(idx->i());
                        delete idx;
                }

                auto idx = _idx[_idx.size() - 1]->expr_eval();
                if (idx->type() != VAL_INT)
                        usage("ast::ast_eval(): array index must be an integer");

                if (vp->type() != VAL_ARR)
                        usage("ast::ast_eval(): trying to index non array");


                vp->arr_set(idx->i(), _expr->expr_eval());
                delete idx;
                return nullptr;
        }

        // TODO: change this away from "assignment"
        //
        // NOTE: member access will have to be moved to expression code for
        // stuff like
        //
        //      full_name = name.first + name.second
        if (_type == AST_MEMBER_ASSIGN) {
                auto id = _str;

                auto vp = curr_scope->get(id);
                if (!vp)
                        usage("ast::ast_eval(): undefined struct: %s", cstr(id));

                if (vp->type() == VAL_ARR) {
                        auto id = _str;

                        auto vp = curr_scope->get(id);
                        if (!vp)
                                usage("ast::ast_eval(): undefined struct: %s", cstr(id));

                        if (vp->type() != VAL_ARR)
                                usage("ast::ast_eval(): only array struct supported right now");

                        auto mem = _members[0];

                        if (mem == "push")
                                vp->push(_expr->expr_eval());
                        if (mem == "sort")
                                vp->arr_sort();
                        return nullptr;
                }

                if (vp->type() != VAL_STRUCT)
                        usage("ast::ast_eval(): member assign only for structs");

                auto tmp = curr_scope;
                curr_scope = vp->members();

                for (size_t i = 0; i < _members.size() - 1; i++) {
                        vp = curr_scope->get(_members[i]);
                        if (!vp->agg())
                                usage("ast::ast_eval(): only array or struct supported for member access");
                        curr_scope = vp->members();
                }

                curr_scope->set(_members[_members.size() - 1], _expr->expr_eval());
                curr_scope = tmp;
                return nullptr;
        }

        return nullptr;
}

void ast::push_idx(ast *idx)
{
        if (!idx)
                usage("ast::push_idx: array index cannot be null");

        _idx.push_back(idx);
}

void ast::push_stmt(ast *stmt)
{
        if (!stmt)
                usage("ast::push_stmt: statment cannot be null");

        _stmts.push_back(stmt);
}

void ast::set_left(ast *left)
{
        _left = left;
}

void ast::set_right(ast *right)
{
        _right = right;
}

long ast::math_eval(void) const
{
        value *vp = nullptr;
        long tmp;

        switch (_type) {
        case AST_SUB:
                return _left->math_eval() - _right->math_eval();
        case AST_ADD:
                return _left->math_eval() + _right->math_eval();
        case AST_MUL:
                return _left->math_eval() * _right->math_eval();
        case AST_INT:
                return _i;
        case AST_VAR_REF:
                vp = var_ref();

                if (vp->type() != VAL_INT)
                        usage("ast::math_eval: non integer used in comparison: %s", cstr(_str));

                return vp->i();
        case AST_ARR_REF:
                vp = arr_ref();

                if (vp->type() != VAL_INT)
                        usage("ast::math_eval: non integer used in comparison: %s", cstr(_str));

                return vp->i();
        case AST_FUNC_CALL:
                vp = call();

                if (vp->type() != VAL_INT)
                        usage("ast::logic_eval: non integer used in comparison: %s", cstr(_str));

                tmp = vp->i();
                delete vp;
                return tmp;
        }

        usage("ast::math_eval: bad ast type for expression: %s", cstr(name()));
        exit(EXIT_FAILURE);
}

long ast::logic_eval(void) const
{
        value *vp = nullptr;
        long tmp;

        switch (_type) {
        case AST_LOG_AND:
                return _left->logic_eval() && _right->logic_eval();
        case AST_LT:
                return _left->logic_eval() < _right->logic_eval();
        case AST_EQ:
                return _left->logic_eval() == _right->logic_eval();
        case AST_SUB:
        case AST_ADD:
        case AST_MUL:
                return math_eval();
        case AST_INT:
                return _i;
        case AST_VAR_REF:
                vp = var_ref();

                if (vp->type() != VAL_INT)
                        usage("ast::logic_eval: non integer used in comparison: %s", cstr(_str));

                return vp->i();
        case AST_ARR_REF:
                vp = arr_ref();

                if (vp->type() != VAL_INT)
                        usage("ast::logic_eval: non integer used in comparison: %s", cstr(_str));

                return vp->i();
        case AST_FUNC_CALL:
                vp = call();

                if (vp->type() != VAL_INT)
                        usage("ast::logic_eval: non integer used in comparison: %s", cstr(_str));

                tmp = vp->i();
                delete vp;
                return tmp;
        }

        usage("ast::logic_eval: bad ast type for comparison: %s", cstr(name()));
        exit(EXIT_FAILURE);
}

void ast::set_type(int type)
{
        _type = type;
        typeok();
}

void ast::push_member(const std::string& mem)
{
        if (mem == "")
                usage("ast::push_member: member cannot be empty string");

        _members.push_back(mem);
}

int ast::expr_type(void) const
{
        ast *prev = nullptr;
        auto curr = _left;

        while (curr) {
                prev = curr;
                curr = curr->left();
        }

        if (prev->type() == AST_NIL)
                return AST_NIL;
        if (prev->type() == AST_INT)
                return AST_INT;
        if (prev->type() == AST_STR)
                return AST_STR;

        if (prev->type() == AST_VAR_REF) {
                auto vp = prev->var_ref();
                if (vp->type() == VAL_STR)
                        return AST_STR;
                if (vp->type() == VAL_INT)
                        return AST_INT;
        }

        if (prev->type() == AST_ARR_REF) {
                auto vp = prev->arr_ref();
                if (vp->type() == VAL_STR)
                        return AST_STR;
                if (vp->type() == VAL_INT)
                        return AST_INT;
        }

        if (prev->type() == AST_FUNC_CALL) {
                for (const auto &p : prev->stmts()) {
                        if (p->type() == AST_RET)
                                return p->expr_type();
                }
        }

        usage("ast::expr_type(): unknown expression type");
        exit(1);
}

ast *ast::left(void)
{
        return _left;
}

std::string ast::str_expr(void) const
{
        value *vp = nullptr;
        string tmp;

        switch (_type) {
        case AST_ADD:
                return _left->str_expr() + _right->str_expr();
        case AST_STR:
                return _str;
        case AST_VAR_REF:
                vp = var_ref();

                if (vp->type() != VAL_STR)
                        usage("ast::str_expr: non string used in string expression: %s", cstr(_str));

                return vp->s();
        case AST_ARR_REF:
                vp = arr_ref();

                if (vp->type() != VAL_STR)
                        usage("ast::str_expr: non string used in string expression: %s", cstr(_str));

                return vp->s();
        case AST_FUNC_CALL:
                vp = call();

                if (vp->type() != VAL_STR)
                        usage("ast::str_expr: non string used in string expression: %s", cstr(_str));

                tmp = vp->s();
                delete vp;
                return tmp;
        }

        usage("ast::str_expr: bad ast type for string expression: %s", cstr(name()));
        exit(EXIT_FAILURE);
}

value *ast::arr_ref(void) const
{
        auto vp = curr_scope->get(_str);
        if (!vp)
                usage("ast::arr_ref(): undefined variable: %s", cstr(_str));

        for (size_t i = 0; i < _idx.size() - 1; i++) {
                auto idx = _idx[i]->expr_eval();
                if (idx->type() != VAL_INT)
                        usage("ast::arr_ref(): array index must be an integer");

                if (vp->type() != VAL_ARR)
                        usage("ast::arr_ref(): trying to index non array");

                vp = vp->arr_get(idx->i());
                delete idx;
        }

        auto idx = _idx[_idx.size() - 1]->expr_eval();
        if (idx->type() != VAL_INT)
                usage("ast::arr_ref(): array index must be an integer");

        vp = vp->arr_get(idx->i());
        delete idx;
        return vp;
}

value *ast::var_ref(void) const
{
        auto vp = curr_scope->get(_str);

        if (!vp)
                usage("ast::var_ref: undefined variable: %s", cstr(_str));

        return vp;
}

void ast::push_param(const std::string& param)
{
        _params.push_back(param);
}

ast *ast::copy(void) const
{
        ast *ap = nullptr;

        if (_type == AST_CTOR)
                return new ast{AST_CTOR, _str};

        if (_type == AST_STRUCT_DEF) {
                ap = new ast{AST_STRUCT_DEF, _str};

                for (const auto &p : _stmts)
                        ap->push_stmt(p->copy());
        }

        if (_type == AST_RET) {
                ap = new ast{AST_RET};
                ap->set_expr(_expr->copy());
        }

        if (_type == AST_ERR)
                return new ast{AST_ERR};

        if (_type == AST_NIL)
                return new ast{AST_NIL};

        if (_type == AST_VAR_REF)
                return new ast{AST_VAR_REF, _str};

        if (_type == AST_ASSIGN) {
                ap = new ast{AST_ASSIGN, _str};
                ap->set_expr(_expr->copy());
        }

        if (_type == AST_PRINT) {
                ap = new ast{AST_PRINT};
                for (const auto &p : _args)
                        ap->push_arg(p->copy());
        }

        if (_type == AST_STR)
                return new ast{AST_STR, _str};

        if (_type == AST_INT)
                return new ast{AST_INT, _i};

        if (_type == AST_ARR) {
                ap = new ast{AST_ARR};

                for (const auto &ep: _arr)
                        ap->push(ep->copy());
        }

        if (_type == AST_ARR_ASSIGN) {
                ap = new ast{AST_ARR_ASSIGN, _str};

                for (const auto &ep: _idx)
                        ap->push_idx(ep->copy());

                ap->set_expr(_expr->copy());
        }

        if (_type == AST_IF) {
                ap = new ast{AST_IF};

                ap->set_expr(_expr->copy());

                for (const auto &p : _stmts)
                        ap->push_stmt(p->copy());
        }

        if (_type == AST_WHILE) {
                ap = new ast{AST_WHILE};

                ap->set_expr(_expr->copy());

                for (const auto &p : _stmts)
                        ap->push_stmt(p->copy());
        }

        if (_type == AST_MEMBER_ASSIGN) {
                ap = new ast{AST_MEMBER_ASSIGN, _str};

                for (const auto &s : _members)
                        ap->push_member(s);
        }

        if (_type == AST_FUNC_DEF) {
                ap = new ast{AST_FUNC_DEF, _str};

                for (const auto &s : _params)
                        ap->push_param(s);

                for (const auto &p : _stmts)
                        ap->push_stmt(p->copy());
        }

        if (_type == AST_ARR_REF) {
                ap = new ast{AST_ARR_REF, _str};

                for (const auto &ep : _idx)
                        ap->push_idx(ep->copy());
        }

        if (_type == AST_FUNC_CALL) {
                ap = new ast{AST_FUNC_CALL, _str};

                for (const auto &p : _args)
                        ap->push_arg(p->copy());
        }

        std::unordered_set<int> binops {
                AST_LT,
                AST_LOG_AND,
                AST_ADD,
                AST_SUB,
                AST_MUL,
                AST_EQ,
        };

        if (binops.find(_type) != binops.end()) {
                ap = new ast{_type};
                
                ap->set_left(_left->copy());
                ap->set_right(_right->copy());
        }

        return ap;
}

void ast::push_arg(ast *p)
{
        _args.push_back(p);
}

size_t ast::n_params(void) const
{
        return _params.size();
}

const std::string& ast::param(size_t i) const
{
        return _params.at(i);
}

const std::vector<ast*>& ast::stmts(void) const
{
        return _stmts;
}

value *ast::call(void) const
{
        auto vp = curr_scope->get(_str);

        if (!vp)
                usage("ast::call(): undefined function: %s", cstr(_str));

        if (vp->type() != VAL_FUNC)
                usage("ast::call(): not a function: %s", cstr(_str));

        auto np = vp->func();
        if (_args.size() != np->n_params())
                usage("ast::call(): wrong number of arguments: %s", cstr(_str));

        auto s = new scope{curr_scope};
        for (size_t i = 0; i < _args.size(); i++)
                s->set(np->param(i), _args[i]->expr_eval());

        curr_scope = s;
        value *ret = nullptr;
        for (auto &ap : np->stmts()) {
                ret = ap->eval();
                if (ret)
                        break;
        }

        curr_scope = s->parent();
        delete s;

        return ret;
}
