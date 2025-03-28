#ifndef VALUE_H
#define VALUE_H

#include "ast.h"
#include <vector>
#include <string>

enum {
        VAL_ERR,
        VAL_STR,
        VAL_INT,
        VAL_ARR,
        VAL_NIL,
        VAL_PTR,
        VAL_FUNC,
        NR_VAL,
};

class value {
private:
        std::vector<value*> _arr;
        std::string         _s;
        value               *_ptr;
        long                _i;
        int                 _type;
        ast                 *_func;

        void init(int type, const std::string& s, long i, value *vp, ast *func);
public:

        value(void);
        value(int type);
        value(int type, long i);
        value(int type, const std::string& s);
        value(int type, value *vp);
        value(int type, ast *func);
        ~value();

        const std::vector<value*>& arr(void) const;
        std::vector<value*>&       arr_non_const(void);
        const std::string&         s(void) const;
        const std::string&         name(void) const;
        value                      *copy(void);
        value                      *ptr(void);
        value                      *arr_get(size_t i);
        long                       i(void) const;
        void                       push(value *vp);
        void                       set_s(const std::string& s);
        void                       set_i(long i);
        void                       dump(int space);
        void                       arr_set(size_t i, value *vp);
        void                       print(void);
        void                       arr_push(value *vp);
        void                       arr_sort(void);
        bool                       truthy(void) const;
        int                        type(void) const;
        ast                        *func(void);
};

#endif // VALUE_H
