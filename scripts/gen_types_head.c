#include  <stdio.h>



int main(int argc, char **argv)
{

        printf("#ifndef     __s_types_sizes_h__    \n\n");
        printf("#define     __s_types_sizes_h__    \n\n");

        printf("/* This file is auto generated*/ \n\n");

        printf("#define __S_SIZE_CHAR__     %d\n", (int)sizeof(char));
        printf("#define __S_SIZE_INT__      %d\n", (int)sizeof(int));
        printf("#define __S_SIZE_uINT__     %d\n", (int)sizeof(unsigned int));
        printf("#define __S_SIZE_LONG__     %d\n", (int)sizeof(long));
        printf("#define __S_SIZE_uLONG__    %d\n", (int)sizeof(unsigned long));
        printf("#define __S_SIZE_DLONG__    %d\n", (int)sizeof(long long));
        printf("#define __S_SIZE_uDLONG__   %d\n", (int)sizeof(unsigned long long));

        printf("\n\n");
        printf("#endif");

        return 0;
}




