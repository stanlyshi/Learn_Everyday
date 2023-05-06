# Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:28 CST. 
# Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 19日 星期二 14:31:30 CST. 
#!/bin/bash

Copyright=$PWD/$0
tmpfile=/tmp/$$.cright
Date=`date | awk '{print $1 $2 $3}'`

for i in `ls`
do
    if [ -d $i ]; then
        cd $i
        $Copyright
        cd ..
    elif # C file
        [ .c = `echo $i | sed 's/.*\.c/.c/'` -o .h = `echo $i |sed 's/.*\.h/.h/'` ]
        then
        >$tmpfile
        echo "/* Copyright (c) ICT/CAS Sylincom, $Date. */" >> $tmpfile
        echo "" >> $tmpfile
        cat $i >> $tmpfile
        #cat $tmpfile | grep Copyright
        mv -f $tmpfile $i
    fi

done

exit 0