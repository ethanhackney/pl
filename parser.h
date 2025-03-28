#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"

class parser {
private:
        lexer& _lex;

        void expect(int type);
        ast  *expr(void);
        ast  *term(void);
        ast  *factor(void);
        ast  *if_stmt(void);
        ast  *while_stmt(void);
        ast  *cmp_expr(void);
        ast  *logic_expr(void);
        int  type(void);
public:
        parser(lexer& lex);

        ast *stmt(void);
};

#endif // #ifndef PARSER_H
