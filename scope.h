#ifndef SCOPE_H
#define SCOPE_H

#include "ast.h"
#include <string>
#include <unordered_map>
#include <vector>

class value;

class scope {
private:
        std::unordered_map<std::string,value*> _tab;
        std::unordered_map<value*,int>         _ref;
        scope                                  *_parent;

        void free(const std::string& name);
public:
        scope(void);
        scope(scope *parent);
        ~scope();

        const std::unordered_map<std::string,value*>& tab(void) const;
        scope                                         *parent(void);
        value                                         *get(const std::string& name);
        void                                          set(const std::string& name, value *v);
        void                                          dump(int space);
};

extern scope *curr_scope;

#endif // #ifndef SCOPE_H
