#include "parser.h"
#include "scope.h"
#include "value.h"
#include "lib.h"

parser::parser(lexer& lex)
        : _lex {lex}
{
}

ast *parser::stmt(void) const
{
        if (type() == TOK_STRUCT) {
                expect(TOK_STRUCT);
                auto ap = new ast{AST_STRUCT_DEF, _lex.lex()};
                expect(TOK_ID);
                expect(TOK_LBRACE);
                // check if already defined
                while (type() != TOK_RBRACE) {
                        auto s = stmt();
                        if (s->type() != AST_ASSIGN && s->type() != AST_FUNC_DEF)
                                usage("parser::stmt(): must be assignment or function def");
                        ap->push_stmt(s);
                }
                expect(TOK_RBRACE);
                return ap;
        }

        if (type() == TOK_RET) {
                auto ap = new ast{AST_RET};
                expect(TOK_RET);
                ap->set_expr(logic_expr());
                return ap;
        }

        if (type() == TOK_FUNC) {
                expect(TOK_FUNC);
                auto ap = new ast{AST_FUNC_DEF, _lex.lex()};
                // check if already defined
                expect(TOK_ID);
                expect(TOK_LPAREN);
                while (type() != TOK_RPAREN) {
                        ap->push_param(_lex.lex());
                        expect(TOK_ID);
                        if (type() == TOK_COMMA)
                                expect(TOK_COMMA);
                }
                expect(TOK_RPAREN);
                expect(TOK_LBRACE);
                while (type() != TOK_RBRACE)
                        ap->push_stmt(stmt());

                expect(TOK_RBRACE);
                return ap;
        }

        if (type() == TOK_PRINT) {
                auto ap = new ast{AST_PRINT};
                expect(TOK_PRINT);
                expect(TOK_LPAREN);
                while (type() != TOK_RPAREN) {
                        ap->push_arg(logic_expr());
                        if (type() == TOK_COMMA)
                                expect(TOK_COMMA);
                }
                expect(TOK_RPAREN);
                return ap;
        }
        if (type() == TOK_IF)
                return if_stmt();

        if (type() == TOK_WHILE)
                return while_stmt();

        ast *ap = nullptr;
        auto id = _lex.lex();
        expect(TOK_ID);

        if (type() == TOK_LPAREN) {
                ap = new ast{AST_FUNC_CALL, id};

                expect(TOK_LPAREN);
                while (type() != TOK_RPAREN) {
                        ap->push_arg(expr());
                        if (type() == TOK_COMMA)
                                expect(TOK_COMMA);
                }
                expect(TOK_RPAREN);
                return ap;
        }

        if (type() == TOK_LBRACK) {
                ap = new ast{AST_ARR_ASSIGN, id};
                while (type() == TOK_LBRACK) {
                        expect(TOK_LBRACK);
                        ap->push_idx(logic_expr());
                        expect(TOK_RBRACK);
                }
        } else if (type() == TOK_DOT) {
                ap = new ast{AST_MEMBER_ASSIGN, id};
                while (type() == TOK_DOT) {
                        expect(TOK_DOT);
                        ap->push_member(_lex.lex());
                        expect(TOK_ID);
                }
                if (type() == TOK_LPAREN) {
                        expect(TOK_LPAREN);
                        if (type() != TOK_RPAREN)
                                ap->set_expr(logic_expr());
                        expect(TOK_RPAREN);
                        return ap;
                }
        } else {
                ap = new ast{AST_ASSIGN, id};
        }

        expect(TOK_ASSIGN);
        ap->set_expr(logic_expr());
        return ap;
}

ast *parser::logic_expr(void) const
{
        auto left = cmp_expr();

        while (type() == TOK_LOG_AND) {
                auto root = new ast{AST_LOG_AND};
                root->set_left(left);
                expect(TOK_LOG_AND);
                root->set_right(cmp_expr());
                left = root;
        }

        return left;
}

ast *parser::cmp_expr(void) const
{
        auto left = expr();

        while (type() == TOK_LT || type() == TOK_EQ) {
                ast *root;
                if (type() == TOK_LT) {
                        root = new ast{AST_LT};
                        expect(TOK_LT);
                } else {
                        root = new ast{AST_EQ};
                        expect(TOK_EQ);
                }
                root->set_left(left);
                root->set_right(expr());
                left = root;
        }

        return left;
}

ast *parser::expr(void) const
{
        auto left = term();

        while (type() == TOK_SUB || type() == TOK_ADD) {
                auto root = new ast{};

                if (type() == TOK_ADD) {
                        root->set_type(AST_ADD);
                        expect(TOK_ADD);
                } else {
                        root->set_type(AST_SUB);
                        expect(TOK_SUB);
                }

                root->set_left(left);
                root->set_right(term());
                left = root;
        }

        return left;
}

ast *parser::if_stmt(void) const
{
        auto ap = new ast{AST_IF};

        expect(TOK_IF);
        expect(TOK_LPAREN);

        ap->set_expr(logic_expr());

        expect(TOK_RPAREN);

        expect(TOK_LBRACE);

        while (type() != TOK_RBRACE)
                ap->push_stmt(stmt());

        expect(TOK_RBRACE);
        return ap;
}

void parser::expect(int type) const
{
        _lex.expect(type);
}

int parser::type(void) const
{
        return _lex.type();
}

ast *parser::while_stmt(void) const
{
        auto ap = new ast{AST_WHILE};

        expect(TOK_WHILE);
        expect(TOK_LPAREN);

        ap->set_expr(logic_expr());

        expect(TOK_RPAREN);

        expect(TOK_LBRACE);

        while (type() != TOK_RBRACE)
                ap->push_stmt(stmt());

        expect(TOK_RBRACE);
        return ap;
}

ast *parser::term(void) const
{
        auto left = factor();

        while (type() == TOK_STAR) {
                auto root = new ast{AST_MUL};
                expect(TOK_STAR);
                root->set_left(left);
                root->set_right(factor());
                left = root;
        }

        return left;
}

ast *parser::factor(void) const
{
        ast *ap = nullptr;

        if (type() == TOK_ID) {
                ap = new ast{AST_VAR_REF, _lex.lex()};
                expect(TOK_ID);

                if (type() == TOK_LBRACK) {
                        ap->set_type(AST_ARR_REF);
                        while (type() == TOK_LBRACK) {
                                expect(TOK_LBRACK);
                                ap->push_idx(expr());
                                expect(TOK_RBRACK);
                        }
                } else if (type() == TOK_LPAREN) {
                        ap->set_type(AST_FUNC_CALL);
                        expect(TOK_LPAREN);

                        while (type() != TOK_RPAREN) {
                                ap->push_arg(logic_expr());
                                if (type() == TOK_COMMA)
                                        expect(TOK_COMMA);
                        }
                        expect(TOK_RPAREN);
                } else if (type() == TOK_DOT) {
                        // TODO: make it work properly with methods
                        ap->set_type(AST_MEMBER_REF);
                        while (type() == TOK_DOT) {
                                expect(TOK_DOT);
                                ap->push_member(_lex.lex());
                                expect(TOK_ID);
                        }
                }
        } else if (type() == TOK_NIL) {
                ap = new ast{AST_NIL};
                expect(TOK_NIL);
        } else if (type() == TOK_INT) {
                ap = new ast{AST_INT, _lex.i()};
                expect(TOK_INT);
        } else if (type() == TOK_STR) {
                ap = new ast{AST_STR, _lex.lex()};
                expect(TOK_STR);
        } else if (type() == TOK_LBRACK) {
                expect(TOK_LBRACK);
                ap = new ast{AST_ARR};
                while (type() != TOK_RBRACK) {
                        ap->push(expr());
                        if (type() == TOK_COMMA)
                                expect(TOK_COMMA);
                }
                expect(TOK_RBRACK);
        } else if (type() == TOK_LPAREN) {
                expect(TOK_LPAREN);
                ap = logic_expr();
                expect(TOK_RPAREN);
        }

        return ap;
}
