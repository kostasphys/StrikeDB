#! usr/bin/bash

HOME_DIR=/home/kostasph/StrikeDB/

cd ${HOME_DIR} 


 rm -R ../backup_StrikeDB/backup_StrikeDB4

 mv   ../backup_StrikeDB/backup_StrikeDB3    ../backup_StrikeDB/backup_StrikeDB4
 mv   ../backup_StrikeDB/backup_StrikeDB2    ../backup_StrikeDB/backup_StrikeDB3
 mv   ../backup_StrikeDB/backup_StrikeDB1    ../backup_StrikeDB/backup_StrikeDB2

 
 mkdir  ../backup_StrikeDB/backup_StrikeDB1
 mkdir  ../backup_StrikeDB/backup_StrikeDB1/StrikeDB

 cp  -R  ../StrikeDB      ../backup_StrikeDB/backup_StrikeDB1


#( mv -r  ../backup_StrikeDB/backup_StrikeDB2 ../backup_StrikeDB &&  \
#  cp -R ../backup_StrikeDB/backup_StrikeDB1 ../backup_StrikeDB  && \
#  cp -R  ../StrikeDB ../backup_StrikeDB/backup_StrikeDB1 || cp -R ../Strike  )

