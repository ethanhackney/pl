#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"

class parser {
private:
        lexer& _lex;

        void expect(int type) const;
        ast  *expr(void) const;
        ast  *term(void) const;
        ast  *factor(void) const;
        ast  *if_stmt(void) const;
        ast  *while_stmt(void) const;
        ast  *cmp_expr(void) const;
        ast  *logic_expr(void) const;
        int  type(void) const;
public:
        parser(lexer& lex);

        ast *stmt(void) const;
};

#endif // #ifndef PARSER_H
