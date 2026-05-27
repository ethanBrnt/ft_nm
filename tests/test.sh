#!/bin/sh

if [ $# -ne 1 ]; then
    echo 'Too many/few arguments, expecting one' >&2
    exit 1
fi

if [ ! -f "$1" ]
  then
    echo "$1 not found"
    exit 1
fi

if [ ! -f ../ft_nm  ]
  then
    echo "../ft_nm not found"
    exit 1
fi

../ft_nm "$1" > out_ft_nm
nm "$1" > out_nm
diff out_ft_nm out_nm
diff out_ft_nm out_nm > ./diff_nms 