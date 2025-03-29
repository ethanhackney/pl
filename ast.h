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
        std::vector<std::string> _members;
        std::vector<std::string> _params;
        std::vector<ast*>        _arr;
        std::vector<ast*>        _idx;
        std::vector<ast*>        _stmts;
        std::vector<ast*>        _args;
        std::string              _str;
        long                     _i;
        ast                      *_left;
        ast                      *_right;
        ast                      *_expr;
        int                      _type;

        std::string str_expr(void);
        value       *arr_ref(void);
        value       *var_ref(void);
        value       *expr_eval(void);
        value       *call(void) const;
        void        init(int type, const std::string& str, ast *expr, long i, ast *idx);
        long        math_eval(void);
        long        logic_eval(void);
        ast         *copy(void) const;
        int         expr_type(void);
public:

        ast(void);
        ast(int type);
        ast(int type, long i);
        ast(int type, const std::string& str);
        ast(int type, const std::string& str, ast *expr);
        ast(int type, const std::string& str, ast *idx, ast *expr);
        ~ast();

        const std::vector<ast*>& arr(void) const;
        const std::vector<ast*>& idx(void) const;
        const std::string&       name(void) const;
        const std::string&       str(void) const;
        std::vector<ast*>&       arr_non_const(void);
        std::vector<ast*>&       stmts(void);
        const std::string&       param(size_t i) const;
        const ast                *expr(void) const;
        size_t                   n_params(void) const;
        value                    *eval(void) const;
        long                     i(void) const;
        void                     push_idx(ast *idx);
        void                     set_expr(ast *expr);
        void                     push(ast *ap);
        void                     dump(int space);
        void                     push_stmt(ast *ap);
        void                     set_left(ast *ap);
        void                     set_right(ast *ap);
        void                     set_type(int type);
        void                     push_member(const std::string& s);
        void                     set_str(const std::string& s);
        void                     push_param(const std::string& s);
        void                     push_arg(ast *p);
        int                      type(void) const;
        ast                      *left(void);
        ast                      *expr(void);
};

#endif // #ifndef AST_H
