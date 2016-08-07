#include <pustr.hpp>

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
