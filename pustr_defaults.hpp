#pragma once

#include <type_traits>
#include <cstdint>

template<typename T>
struct PP{
    static_assert(std::is_integral<T>::value, "No printer defined for type.");
    static constexpr
    size_t pusnprint(char *b, size_t l, const T *ap){
        const size_t max = 3 * sizeof(T);
        T a = *ap;
        const bool neg = std::is_signed<T>::value && a < 0;
        char tmp[max] = {};
        char *c = &tmp[max];
        do{
            *--c = '0' + (neg ? -(a % 10) : (a % 10));
            a /= 10;
        }while(a);

        size_t need = neg + &tmp[max] - c;
        if(need <= l){
            if(neg)
                *b = '-';
            for(char *bc = b + neg; bc != &b[need]; bc++)
                *bc = *c++;
        }

        return need;
    }
};

template<>
struct PP<void *>{
    static
    size_t pusnprint(char *b, size_t l, void *const *ap){
        const size_t max = 2 * sizeof(void *);
    
        uintptr_t a = (uintptr_t) *ap;
        char tmp[max] = {};
        char *c = &tmp[max];
        do{
            *--c = "0123456789abcdef"[a & 0xF];
            a >>= 4;
        }while(a);

        size_t need = 2 + &tmp[max] - c;
        if(need <= l){
            b[0] = '0';
            b[1] = 'x';
            for(char *bc = &b[2]; bc != &b[need]; bc++)
                *bc = *c++;
        }

        return need;
    }
};

template<>
struct PP<char *>{
    static constexpr
    size_t pusnprint(char *b, size_t l, const char *const *ap){
        const char *a = *ap;
        if(!a)
            a = "<nil>";

        size_t need = 0;
        for(char c = 0; (c = a[need]) != '\0'; need++)
            if(need < l)
                b[need] = c;
        return need;
    };
};

template<typename T>
struct PP<T *>{
    static constexpr
    size_t pusnprint(char *b, size_t l, T *const *ap){
        T *a = *ap;
        if(!a){
            const char *cpp_hates_compound_literals = "<nil>";
            return PP<char*>::pusnprint(b, l, &cpp_hates_compound_literals);
        }
        return pusnprintf(b, l, "%:&%", (void  *) a, *a);
    }
    
};

template<size_t N>
struct PP<char[N]>{
    static constexpr
    size_t pusnprint(char *b, size_t l, const char (*ap)[N]){
        return PP<char *>::pusnprint(b, l, (const char *const *) &ap);
    }
};


template<typename T, size_t N>
struct PP<T[N]>{
    static constexpr
    size_t pusnprint(char *b, size_t l, T *const (*ap)[N]){
        static_assert(sizeof(T) == -1, "Arrays not generally supported yet.");
        return 0;
    }
    
};


