#ifndef      __s_compiler_h__
#define      __s_compiler_h__


#ifndef   __STDC__
#define   __STDC__     0
#endif

/*Check if atomics are supported*/
#if !__STDC__ || !__STDC_VERSION__ || __STDC_VERSION__ < 201112L || __STDC_NO_ATOMICS__ == 1

    //#error "NO ATOMICS SUPPORTED ON THIS PLATFORM!"
#else

    #define HAS_ATOMIC 

#endif


#endif
