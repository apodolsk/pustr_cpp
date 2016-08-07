#pragma once
#include <type_traits>
#include <cstdio>
#include <cstddef>

#define PU_DFLT_BUF_SZ 160

template<typename T> struct PP;

struct pu_arg{
    const void *val;
    size_t (*pusnprint)(char *b, size_t l, const void *a);

    template <typename T>
    pu_arg(const T &val) :
        val(&val),
        pusnprint(&PP<T>::pusnprint)
        {}
    /*     using pptype = size_t (*)(char *, size_t, const void *); */
    /*     return (pu_arg){&val, (pptype) &PP<T>::pusnprint}; */
    /* } */

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
