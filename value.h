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
        std::vector<value*> _arr      {};
        std::string         _s        {};
        scope               *_members {nullptr};
        value               *_ptr     {nullptr};
        long                _i        {-1};
        int                 _type     {VAL_ERR};
        ast                 *_func    {nullptr};
        ast                 *_def     {nullptr};

        void typeok(void) const;
public:

        value(void);
        value(int type);
        value(int type, long i);
        value(int type, const std::string& s);
        value(int type, ast *func);
        ~value();

        const std::vector<value*>& arr(void)                                   const;
        const std::string&         s(void)                                     const;
        const std::string&         name(void)                                  const;
        scope                      *members(void)                              const;
        value                      *arr_get(size_t i)                          const;
        value                      *member(const std::string& mem)             const;
        long                       i(void)                                     const;
        void                       dump(int space)                             const;
        bool                       truthy(void)                                const;
        bool                       agg(void)                                   const;
        void                       print(void)                                 const;
        void                       set_member(const std::string& s, value *vp) const;
        int                        type(void)                                  const;
        ast                        *func(void)                                 const;
        ast                        *def(void)                                  const;

        value *copy(void);
        void  push(value *vp);
        void  set_s(const std::string& s);
        void  set_i(long i);
        void  arr_set(size_t i, value *vp);
        void  arr_sort(void);
        void  set_def(ast *def);
        void  set_func(ast *func);
};

#endif // VALUE_H
