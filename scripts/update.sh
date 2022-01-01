#!/bin/bash

root_dir=`pwd`
dirs=`find ${root_dir} -name .git`

for dir in ${dirs}
do
    cd ${dir}/../
    git clean -df;git checkout .;git pull --rebase;
done
cd ${root_dir}
