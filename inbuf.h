#ifndef INBUF_H
#define INBUF_H

enum {
        INBUF_DEFAULT_SIZE = 4096,
};

enum {
        INBUF_ERR = -2,
        INBUF_EOF = -1,
};

class inbuf {
private:
        char *_buf;
        char *_p;
        char *_endp;
        int  _fd;
        int  _sz;

        void init(int fd, int size);
        bool empty(void);
        int fill(void);
public:
        inbuf(int fd, int size);
        inbuf(int fd);
        ~inbuf();

        int getc(void);
};

#endif // #ifndef INBUF_H
