#! usr/bin/bash

if [  -f ${HOME_DIR}"/inc/types/types_size.h" ]; then
    #/dev/null > ${HOME_DIR}"/inc/types.h"
    rm  ${HOME_DIR}"/inc/types/types_size.h"
fi

touch ${HOME_DIR}"/inc/types/types_size.h"


( ${HOME_DIR}"scripts/Build/gen_types_head.o" > ${HOME_DIR}"/inc/types/types_size.h"  ||   echo 'BIG ERROR'  ) 
