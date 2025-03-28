#include "lib.h"
#include "token.h"
#include <array>

using std::string;

static const std::array<string,NR_TOK> TOK_NAMES {
        "TOK_ERR",
        "TOK_EOF",
        "TOK_WORD",
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

token::token(int type)
{
        init(type, "");
}

token::token(int type, const string& lex)
{
        init(type, lex);
}

token::token(void)
{
        init(TOK_ERR, "");
}

void token::init(int type, const string& lex)
{
        _lex = lex;
        _type = type;
        if (!type_ok(TOK_ERR, NR_TOK, _type))
                usage("token::init(): invalid token type: %d", _type);
        if (_type == TOK_INT)
                _i = strtol(lex.c_str(), nullptr, 10);
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
        return TOK_NAMES.at(_type);
}

long token::i(void) const
{
        return _i;
}
