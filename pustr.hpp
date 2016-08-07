#pragma once
#include <type_traits>
#include <cstdio>
#include <cstddef>

#define PU_DFLT_BUF_SZ 160

template<typename T> struct PP;

struct pu_arg{
    const void *val;
    size_t (*pusnprint)(char *b, size_t l, const void *a);

    using pptype = size_t (*)(char *, size_t, const void *);
    
    template <typename T, std::enable_if_t<!std::is_pointer<T>::value, int> = 0>
    pu_arg(const T &val) : val(&val){
        this->pusnprint = (pptype) &PP<T>::pusnprint;
    }
    
    template <typename T, std::enable_if_t<std::is_pointer<T>::value, int> = 0>
    pu_arg(const T &val) : val(&val){
        using T2 =
            std::add_pointer_t<
            std::remove_cv_t<
            std::remove_pointer_t<T>>>;
        
        this->pusnprint = (pptype) &PP<T2>::pusnprint;
    }
};

size_t _pusnprintf(char *b, size_t max, const char *fmt, const pu_arg *args);
int _puprintf(const char *fmt, const pu_arg *args);


template<typename... Ts>
size_t pusnprintf(char *b, size_t max, const char *fmt, const Ts&... args){
    const pu_arg pu_args[] = {pu_arg(args)...};
    return _pusnprintf(b, max, fmt, pu_args);
}

template<typename... Ts>
size_t puprintf(const char *fmt, const Ts&... args){
    const pu_arg pu_args[] = {pu_arg(args)...};
    return _puprintf(fmt, pu_args);
}

#define pudef(T, fmt, members...)                                       \
    template <>                                                         \
    struct PP<T>{                                                       \
        static                                                          \
        size_t pusnprint(char *b, size_t l, const T *a){                \
            return pusnprintf(b, l, fmt, ##members);                    \
        }                                                               \
    }

#include <pustr_defaults.hpp>
