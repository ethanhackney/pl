#ifndef INBUF_H
#define INBUF_H

enum {
        INBUF_ERR = -2,
        INBUF_EOF = -1,
};

class inbuf {
private:
        char *_buf  {nullptr};
        char *_p    {nullptr};
        char *_endp {nullptr};
        int  _fd    {-1};

        int fill(void);
public:
        inbuf(int fd);
        ~inbuf();

        int getc(void);
};

#endif // #ifndef INBUF_H
