#include "lib.h"
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

void usage(const char *fmt, ...)
{
        va_list va;

        va_start(va, fmt);
        vfprintf(stderr, fmt, va);
        va_end(va);
        fprintf(stderr, ": %s\n", strerror(EINVAL));
        exit(EXIT_FAILURE);
}

bool type_ok(int low, int high, int n)
{
        return low <= n && n < high;
}

void indent(int space)
{
        for (auto i = 0; i < space; i++)
                putchar(' ');
}

const char *cstr(const std::string& s)
{
        return s.c_str();
}

void in_range(int low, int high, int n, const char *fmt, ...)
{
        if (type_ok(low, high, n))
                return;

        va_list va;

        va_start(va, fmt);
        vfprintf(stderr, fmt, va);
        va_end(va);
        fprintf(stderr, ": %s\n", strerror(EINVAL));
        exit(EXIT_FAILURE);
}
