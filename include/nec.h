#define nec_size_null(a) (*((int*)a-1))
#define nec_size(a) (a?nec_size_null(a):0)
#define nec_add(a) (a=(void*)((int*)realloc((int*)a-(a?1:0),sizeof(int)+sizeof(*a)*(nec_size(a)+1))+1),++nec_size_null(a))
#define nec_push(a, v) (nec_add(a),a[nec_size(a)-1]=v)
#define nec_free(a) free((int*)a-(a?1:0))

#define nec_remove_at(t, a, i)\
({\
    if(i >= 0 && i < nec_size(a))\
    {\
        if(nec_size(a) == 1)\
        {\
            nec_free(a);\
            (a) = 0;\
        }\
        else if(i == nec_size(a) - 1) nec_realloc(a, nec_size(a) - 1);\
        else\
        {\
            void* ___nec_b = (a);\
            (a) = 0;\
            nec_realloc(a, nec_size(___nec_b) - 1);\
            int off = 0;\
            for(int it = 0; it < nec_size(___nec_b); it++)\
            {\
                if(it != i) (a)[it - off] = ((t*)(___nec_b))[it];\
                else off++;\
            }\
            nec_free(___nec_b);\
        }\
    }\
})

#define nec_remove(t, a, v)\
({\
    for(int i = 0; i < nec_size(a); i++)\
    {\
        if((a)[i] == v)\
        {\
            nec_remove_at(t, a, i);\
            break;\
        }\
    }\
})

#define nec_contains(__nec_a, __nec_v)\
({\
    char __nec_c = 0;\
    for(int __nec_i = 0; __nec_i < nec_size(__nec_a); __nec_i++)\
    {\
        if(__nec_v == __nec_a[__nec_i])\
        {\
            __nec_c = 1;\
            break;\
        }\
    }\
    __nec_c;\
})

/*
TODO:
add nec_push_norealloc
rename all one letter parameters into something similar to __nec_b
*/
