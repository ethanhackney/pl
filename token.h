#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum {
        TOK_ERR,
        TOK_EOF,
        TOK_ID,
        TOK_ASSIGN,
        TOK_STR,
        TOK_INT,
        TOK_LBRACK,
        TOK_RBRACK,
        TOK_COMMA,
        TOK_NIL,
        TOK_IF,
        TOK_LPAREN,
        TOK_RPAREN,
        TOK_LBRACE,
        TOK_RBRACE,
        TOK_WHILE,
        TOK_LT,
        TOK_SUB,
        TOK_ADD,
        TOK_LOG_AND,
        TOK_STAR,
        TOK_PRINT,
        TOK_DOT,
        TOK_FUNC,
        TOK_RET,
        TOK_EQ,
        TOK_STRUCT,
        NR_TOK,
};

class token {
private:
        std::string _lex;
        long        _i;
        int         _type;

        void init(int type, const std::string& lex);
public:
        token(int type);
        token(int type, const std::string& lex);
        token(void);

        const std::string& lex(void) const;
        const std::string& name(void) const;
        long               i(void) const;
        int                type(void) const;
};

#endif // #ifndef TOKEN_H
