#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "inbuf.h"
#include "scope.h"
#include "value.h"
#include <vector>

#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <err.h>
#include <sysexits.h>

// add anonymous functions

int main(int argc, char **argv)
{
        for (auto i = 1; i < argc; i++) {
                auto fd = open(argv[i], O_RDONLY);
                if (fd < 0) {
                        warn("could not open %s", argv[i]);
                        continue;
                }

                inbuf buf {fd};
                lexer lex {buf};
                parser p {lex};

                while (lex.type() != TOK_EOF) {
                        auto ap = p.stmt();
                        ap->eval();
                        delete ap;
                }

                if (close(fd) < 0)
                        err(EX_OSERR, "could not close %s", argv[i]);
        }

        auto vp = curr_scope->get("query");
        if (vp)
                vp->dump(0);

        vp = curr_scope->get("q");
        if (vp)
                vp->dump(0);
}
