#ifndef LEXER_H
#define LEXER_H

#include "inbuf.h"
#include "token.h"
#include <string>
#include <vector>

class lexer {
private:
        inbuf& _buf;
        token  _curr    {};
        int    _putback {0};

        std::string read_num(int c);
        std::string read_word(int c);
        std::string read_str(void);
        void        next(void);
        int         nextchar(void);
public:
        lexer(inbuf& buf);

        const std::string& lex(void)  const;
        long               i(void)    const;
        int                type(void) const;

        void               expect(int type);
};

#endif // LEXER_H
