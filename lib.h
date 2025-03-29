#ifndef LIB_H
#define LIB_H

#include <string>

void usage(const char *fmt, ...);
bool type_ok(int low, int high, int n);
void indent(int space);
const char *cstr(const std::string& s);
void in_range(int low, int high, int n, const char *fmt, ...);

#endif // #ifndef LIB_H
