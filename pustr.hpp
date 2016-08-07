#pragma once
#include <type_traits>
#include <cstdio>
#include <cstddef>

#define PU_DFLT_BUF_SZ 160

template<typename T> struct PP;

struct pu_arg{
    const void *val;
    size_t (*pusnprint)(char *b, size_t l, const void *a);
};

#include <iostream>
#include <typeinfo>
template <typename T>
constexpr
pu_arg pu_arg_of(const T &val){
    using pptype = size_t (*)(char *, size_t, const void *);
    return (pu_arg){
        &val,
        (pptype) &PP<T>::pusnprint
    };
}

constexpr
size_t _pusnprintf(char *b, size_t max, const char *fmt, const pu_arg *args){
    size_t l = 0;
    for(const char *c = fmt; *c != '\0'; c++){
        if(*c == '%'){
            const pu_arg *a = args++;
            l += a->pusnprint(b + l, max > l ? max - l : 0, a->val);
        }
        else if(l++ < max)
            b[l - 1] = *c;
    }
    if(l < max)
        b[l] = '\0';
    return l;
}

int _puprintf(const char *fmt, const pu_arg *args){
    size_t need;
    size_t max = PU_DFLT_BUF_SZ;
    for(int i = 0; i < 2; i++){
        char b[max];
        need = 1 + _pusnprintf(b, max, fmt, args);
        if(max >= need)
            return fputs(b, stdout) ?: (int) need;
        max = need;
    }
    __builtin_unreachable();
}

template<typename... Ts>
constexpr
size_t pusnprintf(char *b, size_t max, const char *fmt, const Ts&... args){
    const pu_arg pu_args[] = {pu_arg_of(args)...};
    return _pusnprintf(b, max, fmt, pu_args);
}

template<typename... Ts>
size_t puprintf(const char *fmt, const Ts&... args){
    const pu_arg pu_args[] = {pu_arg_of(args)...};
    return _puprintf(fmt, pu_args);
}

#define pudef(T, fmt, members...)                                       \
    template <>                                                         \
    struct PP<T>{                                                       \
        static constexpr                                                \
        size_t pusnprint(char *b, size_t l, const T *a){                \
            return pusnprintf(b, l, fmt, ##members);                    \
        }                                                               \
    }

#include <pustr_defaults.hpp>
