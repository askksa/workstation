#!/bin/bash
#toolchain_config.sh

toolchain_version=$1
root_dir=`pwd`

ckeck_param()
{
    if [ -z $toolchain_version ];then
        echo "please input toolchain version !!!"
        return 1
    else
        return 0
    fi
}

toolchain_search()
{
    echo "toolchain search"
    for path in `ls $root_dir`
    do
        [ -d "$path/bin" ] || continue
        cd $path/bin && path=`pwd`
        toolchain=`ls $path|grep 'gcc$'`
        if [ -n "$toolchain" ];then
            version_num=`$path/$toolchain --version|grep "gcc"|sed 's/.*\([0-9]\.[0-9]\.[0-9]\).*/\1/'`
            if [ ${version_num} = ${toolchain_version} ];then
                echo "toolchain ${version_num} is found" && toolchain_dir=$path
                cd $root_dir && break;
            else
                cd $root_dir && continue
            fi
        else
            cd $root_dir && continue
        fi
        cd $root_dir
    done
    echo "toolchain search done"
}

ckeck_param
if [ $? != "1" ];then
    toolchain_search
    if [ -z $toolchain_dir ];then
        echo "toolchain not found !!!"
    else
        echo "config toolchain"
        priority=`echo ${version_num:0:1}`
        for tool in $(ls $toolchain_dir)
        do
            if [ -e ~/bin/$tool ];then
                sudo update-alternatives --remove-all $tool
            fi
            sudo update-alternatives --install ~/bin/$tool $tool $toolchain_dir/$tool $priority
        done
        echo "config toolchain done"
    fi
fi
