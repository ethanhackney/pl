#include "inbuf.h"
#include <unistd.h>

inbuf::inbuf(int fd, int size)
{
        init(fd, size);
}

void inbuf::init(int fd, int size)
{
        _fd = fd;
        _sz = size;
        _buf = new char[_sz];
        _p = _buf;
        _endp = _buf;
}

inbuf::inbuf(int fd)
{
        init(fd, INBUF_DEFAULT_SIZE);
}

inbuf::~inbuf()
{
        delete[] _buf;
}

bool inbuf::empty(void)
{
        return _p == _endp;
}

int inbuf::fill(void)
{
        auto n = read(_fd, _buf, _sz);
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
        if (empty()) {
                auto ret = fill();
                if (ret < 0)
                        return ret;
        }
        return *_p++;
}
