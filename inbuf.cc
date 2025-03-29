#include "inbuf.h"
#include "lib.h"
#include <unistd.h>

enum {
        INBUF_DEFAULT_SIZE = 4096,
};

inbuf::inbuf(int fd)
        : _buf {new char[INBUF_DEFAULT_SIZE]},
        _p {_buf},
        _endp {_buf},
        _fd {fd}
{
        if (fd < 0)
                usage("inbuf::inbuf(): negative file descriptr");
}

inbuf::~inbuf()
{
        delete[] _buf;
}

int inbuf::fill(void)
{
        auto n = read(_fd, _buf, INBUF_DEFAULT_SIZE);
        if (n < 0)
                return INBUF_ERR;
        if (n == 0)
                return INBUF_EOF;

        _p = _buf;
        _endp = _buf + n;
        return 0;
}

int inbuf::getc(void)
{
        if (_p == _endp) {
                if (auto ret = fill(); ret < 0)
                        return ret;
        }
        return *_p++;
}
