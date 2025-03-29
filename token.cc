#include "lib.h"
#include "token.h"

using std::string;

token::token(int type, const string& lex)
        : _lex {lex},
        _type {type}
{
        in_range(TOK_ERR, NR_TOK, _type, "token::init(): invalid token type: %d", _type);
        if (_type == TOK_INT)
                _i = strtol(cstr(lex), nullptr, 10);
}

token::token(void)
{
}

const string& token::lex(void) const
{
        return _lex;
}

int token::type(void) const
{
        return _type;
}

const string& token::name(void) const
{
        static const string names[NR_TOK] {
                "TOK_ERR",
                "TOK_EOF",
                "TOK_ID",
                "TOK_ASSIGN",
                "TOK_INT",
                "TOK_STR",
                "TOK_LBRACK",
                "TOK_RBRACK",
                "TOK_COMMA",
                "TOK_NIL",
                "TOK_IF",
                "TOK_LPAREN",
                "TOK_RPAREN",
                "TOK_LBRACE",
                "TOK_RBRACE",
                "TOK_WHILE",
                "TOK_LT",
                "TOK_SUB",
                "TOK_ADD",
                "TOK_LOG_AND",
                "TOK_STAR",
                "TOK_PRINT",
                "TOK_DOT",
                "TOK_FUNC",
                "TOK_RET",
                "TOK_EQ",
                "TOK_STRUCT",
        };
        return names[_type];
}

long token::i(void) const
{
        return _i;
}
