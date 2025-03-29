#ifndef VALUE_H
#define VALUE_H

#include "ast.h"
#include "scope.h"
#include <vector>
#include <string>

enum {
        VAL_ERR,
        VAL_STR,
        VAL_INT,
        VAL_ARR,
        VAL_NIL,
        VAL_FUNC,
        VAL_STRUCT,
        VAL_STRUCT_DEF,
        NR_VAL,
};

class value {
private:
        std::vector<value*> _arr;
        std::string         _s;
        scope               *_members;
        value               *_struct_def;
        value               *_ptr;
        long                _i;
        int                 _type;
        ast                 *_func;
        ast                 *_def;

        void init(int type, const std::string& s, long i, value *vp, ast *func);
public:

        value(void);
        value(int type);
        value(int type, long i);
        value(int type, const std::string& s);
        value(int type, value *vp);
        value(int type, ast *func);
        value(int type, scope *parent);
        ~value();

        const std::vector<value*>& arr(void) const;
        std::vector<value*>&       arr_non_const(void);
        const std::string&         s(void) const;
        const std::string&         name(void) const;
        scope                      *members(void);
        value                      *copy(void);
        value                      *ptr(void);
        value                      *arr_get(size_t i);
        value                      *member(const std::string& s);
        void                       set_struct_def(value *d);
        long                       i(void) const;
        void                       push(value *vp);
        void                       set_s(const std::string& s);
        void                       set_i(long i);
        void                       dump(int space);
        void                       arr_set(size_t i, value *vp);
        void                       print(void);
        void                       arr_push(value *vp);
        void                       arr_sort(void);
        void                       set_member(const std::string& s, value *vp);
        void                       set_def(ast *p);
        void                       set_func(ast *p);
        bool                       truthy(void) const;
        bool                       agg(void);
        int                        type(void) const;
        ast                        *func(void);
        ast                        *def(void);
};

#endif // VALUE_H
