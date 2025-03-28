#ifndef SCOPE_H
#define SCOPE_H

#include "ast.h"
#include "value.h"
#include <string>
#include <unordered_map>
#include <vector>

class scope {
private:
        std::unordered_map<std::string,value*> _tab;
        std::unordered_map<value*,int>         _ref;
        scope                                  *_parent;
public:
        scope(void);
        scope(scope *parent);
        ~scope();

        scope *parent(void);
        value *get(const std::string& name);
        void  set(const std::string& name, value *v);
        void  dump(void);
};

extern scope *curr_scope;

#endif // #ifndef SCOPE_H
