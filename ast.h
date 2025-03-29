#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

enum {
        AST_ERR,
        AST_ASSIGN,
        AST_STR,
        AST_INT,
        AST_ARR,
        AST_ARR_ASSIGN,
        AST_NIL,
        AST_IF,
        AST_WHILE,
        AST_SUB,
        AST_VAR_REF,
        AST_LT,
        AST_ADD,
        AST_LOG_AND,
        AST_MUL,
        AST_PRINT,
        AST_MEMBER_ASSIGN,
        AST_ARR_REF,
        AST_FUNC_DEF,
        AST_FUNC_CALL,
        AST_EQ,
        AST_RET,
        AST_STRUCT_DEF,
        AST_CTOR,
        NR_AST,
};

class value;

class ast {
private:
        std::vector<std::string> _members {};
        std::vector<std::string> _params  {};
        std::vector<ast*>        _arr     {};
        std::vector<ast*>        _idx     {};
        std::vector<ast*>        _stmts   {};
        std::vector<ast*>        _args    {};
        std::string              _str     {};
        long                     _i       {-1};
        ast                      *_left   {nullptr};
        ast                      *_right  {nullptr};
        ast                      *_expr   {nullptr};
        int                      _type    {AST_ERR};

        const std::string& name(void)       const;
        std::string        str_expr(void)   const;
        value              *arr_ref(void)   const;
        value              *var_ref(void)   const;
        value              *expr_eval(void) const;
        value              *call(void)      const;
        void               typeok(void)     const;
        long               math_eval(void)  const;
        long               logic_eval(void) const;
        ast                *copy(void)      const;
        int                expr_type(void)  const;
public:
        ast(void);
        ast(int type);
        ast(int type, long i);
        ast(int type, const std::string& str);
        ~ast();

        const std::vector<ast*>& stmts(void)     const;
        const std::string&       str(void)       const;
        const std::string&       param(size_t i) const;
        const ast                *expr(void)     const;
        size_t                   n_params(void)  const;
        value                    *eval(void)     const;
        long                     i(void)         const;
        void                     dump(int space) const;
        int                      type(void)      const;

        void push_idx(ast *idx);
        void set_expr(ast *expr);
        void push(ast *ap);
        void push_stmt(ast *stmt);
        void set_left(ast *left);
        void set_right(ast *right);
        void set_type(int type);
        void push_member(const std::string& mem);
        void push_param(const std::string& param);
        void push_arg(ast *p);
        ast  *left(void);
};

#endif // #ifndef AST_H
