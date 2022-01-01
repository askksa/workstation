#!/bin/bash

path=$1

if [ ! -d "${path}" ];then
    echo "database path is invalid !!!"
    exit
else
    if [ -f "${path}/index" ];then
        ids=($(awk -F "|" '{print $2}' ${path}/index))
    else
        echo "database index file is invalid !!!"
        exit
    fi
fi

if [ ${#ids[*]} ];then
    echo "****************clear unneeded database******************"
    cnt=0
    left_cnt=0
    for file in `ls ${path}`
    do
        if [ "${file}" != "index" ];then
            i=0
            while [ ${i} -lt ${#ids[*]} ]
            do
                if [[ "${file}" =~ "${ids[$i]}" ]];then
                    break
                fi
                let i++
            done
            if [ ${i} == ${#ids[*]} ];then
                rm -rf ${path}/${file}
                let cnt++
            else
                let left_cnt++
            fi
        fi
    done
    echo "clear database file cnt => [${cnt}]"
    echo "left database file cnt => [${left_cnt}]"
    echo "*************************done****************************"
else
    echo "there is no cscope database !!!"
fi
