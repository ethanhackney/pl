#include "lexer.h"
#include "lib.h"
#include <cctype>
#include <unordered_map>

using std::string;

static const std::unordered_map<int,int> char_map {
        { INBUF_ERR, TOK_ERR    },
        { INBUF_EOF, TOK_EOF    },
        { '=',       TOK_ASSIGN }, 
        { '[',       TOK_LBRACK },
        { ']',       TOK_RBRACK },
        { ',',       TOK_COMMA  },
        { '(',       TOK_LPAREN },
        { ')',       TOK_RPAREN },
        { '{',       TOK_LBRACE },
        { '}',       TOK_RBRACE },
        { '-',       TOK_SUB    },
        { '<',       TOK_LT     },
        { '+',       TOK_ADD    },
        { '*',       TOK_STAR   },
        { '.',       TOK_DOT    },
};

static const std::unordered_map<string,int> kwords {
        { "nil",    TOK_NIL    },
        { "if",     TOK_IF     },
        { "while",  TOK_WHILE  },
        { "print",  TOK_PRINT  },
        { "func",   TOK_FUNC   },
        { "return", TOK_RET    },
        { "struct", TOK_STRUCT },
};

lexer::lexer(inbuf& buf)
        : _buf {buf},
        _putback {0}
{
        next();
}

const string& lexer::lex(void) const
{
        return _curr.lex();
}

int lexer::type(void) const
{
        return _curr.type();
}

void lexer::expect(int t)
{
        if (type() != t)
                usage("lexer::expect(): want %s, got %s", cstr(token{t}.name()), cstr(_curr.name()));
        next();
}

void lexer::next(void)
{
        for (;;) {
                auto c = nextchar();

                if (isspace(c))
                        continue;

                if (c == '#') {
                        while (c != '\n')
                                c = nextchar();
                        continue;
                }

                auto p = char_map.find(c);
                if (p != char_map.end()) {
                        if (p->second == TOK_ASSIGN) {
                                c = nextchar();
                                if (c == '=') {
                                        _curr = token{TOK_EQ};
                                } else {
                                        _putback = c;
                                        _curr = token{TOK_ASSIGN};
                                }
                        } else {
                                _curr = token{p->second};
                        }
                } else if (isdigit(c)) {
                        _curr = token{TOK_INT, read_num(c)};
                } else if (isalpha(c) || c == '_') {
                        _curr = token{TOK_ID, read_word(c)};
                        auto p = kwords.find(_curr.lex());
                        if (p != kwords.end())
                                _curr = token{p->second};
                } else if (c == '"') {
                        _curr = token{TOK_STR, read_str()};
                } else if (c == '&') {
                        c = nextchar();
                        if (c != '&') {
                                _putback = c;
                                continue;
                        }
                        _curr = token{TOK_LOG_AND};
                } else {
                        _curr = token{TOK_ERR};
                }

                return;
        }
}

int lexer::nextchar(void)
{
        int c;

        if (_putback) {
                c = _putback;
                _putback = 0;
        } else {
                c = _buf.getc();
        }

        return c;
}

string lexer::read_num(int c)
{
        string n;

        while (isdigit(c)) {
                n += c;
                c = nextchar();
        }

        _putback = c;
        return n;
}

string lexer::read_word(int c)
{
        string w;

        while (isalpha(c) || isdigit(c) || c == '_') {
                w += c;
                c = nextchar();
        }

        _putback = c;
        return w;
}

string lexer::read_str(void)
{
        string s;
        int c;

        while ((c = nextchar()) != INBUF_EOF && c != INBUF_ERR && c != '"') {
                if (c == '\\') {
                        c = nextchar();
                        if (c == 'n')
                                s += '\n';
                } else {
                        s += c;
                }
        }

        return s;
}

long lexer::i(void) const
{
        return _curr.i();
}
