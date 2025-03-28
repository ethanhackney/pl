#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "inbuf.h"
#include <string>
#include <vector>

class lexer {
private:
        inbuf& _buf;
        token  _curr;
        int    _putback;

        std::string read_num(int c);
        std::string read_word(int c);
        std::string read_str(void);
        int         nextchar(void);
public:
        lexer(inbuf& buf);

        const std::string& lex(void) const;
        const std::string& name(void) const;
        const token&       curr(void) const;
        const token&       next(void);
        const int          type(void) const;
        void               expect(int type);
};

#endif // LEXER_H
