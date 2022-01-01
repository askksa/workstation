#!/bin/bash
#test.sh

test_1()
{
    #拼接字符串
    str='this is string'
    myname='hello ${str}'
    hername='hello '${str}''
    youname="hello ${str}"
    hisname="hello "${str}""
    echo ${str}
    echo ${myname}
    echo ${hername}
    echo ${youname}
    echo ${hisname}
    echo $str
    echo $myname
    echo $hername
    echo $youname
    echo $hisname
}

test_2()
{
    #获取字符串长度
    string="abcd796"
    echo ${#string}
}

test_3()
{
    #提取子字符串
    string="my name is xxx!!!"
    echo ${string:3:4}
}

test_4()
{
    #查找子字符串
    string="hello world!!!"
    echo `expr index "${string}" dl`
}

test_5()
{
    #数组
    array_name=(a b c d)
    array_name[0]="hello"
    array_name[1]="world"
    value="${array_name[0]} ${array_name[1]} ${array_name[2]} ${array_name[3]}"
    echo ${array_name[0]}
    echo ${array_name[1]}
    echo ${array_name[2]}
    echo ${array_name[3]}
    echo ${value}
    echo "array all member:${array_name[@]}"
    echo "array all member:${array_name[*]}"
    echo "array member cnt:${#array_name[@]}"
    echo "array member cnt:${#array_name[*]}"
}

test_6()
{
    #传递参数
    echo "test script:$0"
    echo "param_1: $1"
    echo "param_2: $2"
    echo "param_3: $3"
    echo "param num: $#"
    echo "process pid: $$"
    echo "script exit status: $?"
    echo "all param:"
    for i in "$*";do
        echo ${i}
    done
    for i in "$@";do
        echo ${i}
    done
}

test_7()
{
    #算术运算符
    #expr是一款表达式计算工具,使用它能完成表达式的求值操作
    a=30
    b=20
    value[0]=`expr ${a} + ${b}`
    value[1]=`expr ${a} - ${b}`
    value[2]=`expr ${a} \* ${b}`
    value[3]=`expr ${a} / ${b}`
    value[4]=`expr ${a} % ${b}`
    echo ${value[*]}
    a=${b}
    echo $[${a} == ${b}]
    echo $[${a} != ${b}]
}

test_8()
{
    #关系运算符
    #只支持数字,除非字符串的值是数字
    a=20
    b=30
    echo "a:${a} b:${b}"
    if [ ${a} -eq ${b} ];then
        echo "a == b"
    else
        echo "a != b"
    fi
    if [ ${a} -ne ${b} ];then
        echo "a != b"
    else
        echo "a == b"
    fi
    if [ ${a} -gt ${b} ];then
        echo "a > b"
    else
        echo "a <= b"
    fi
    if [ ${a} -lt ${b} ];then
        echo "a < b"
    else
        echo "a >= b"
    fi
    if [ ${a} -ge ${b} ];then
        echo "a >= b"
    else
        echo "a < b"
    fi
    if [ ${a} -le ${b} ];then
        echo "a <= b"
    else
        echo "a > b"
    fi
}

test_9()
{
    #布尔运算符
    a=10
    b=20
    if [ ${a} != {b} ];then
        echo "a != b"
    else
        echo "a == b"
    fi
    if [ ${a} -lt 100 -a ${b} -gt 15 ];then
        echo "a < 100 && b > 15 return true"
    else
        echo "a < 100 && b > 15 return false"
    fi
    if [ ${a} -lt 100 -o ${b} -gt 100 ];then
        echo "a < 100 || b > 100 return true"
    else
        echo "a < 100 || b > 100 return false"
    fi
    if [ ${a} -lt 5 -o ${b} -gt 100 ];then
        echo "a < 5 || b > 100 return true"
    else
        echo "a < 5 || b > 100 return false"
    fi
}

test_10()
{
    #逻辑运算符
    a=10
    b=20
    if [[ ${a} -lt 100 && ${b} -gt 100 ]];then
        echo "return true"
    else
        echo "return false"
    fi
    if [[ ${a} -lt 100 || ${b} -gt 100 ]];then
        echo "return true"
    else
        echo "return false"
    fi
}

test_11()
{
    #字符串运算符
    a="abc"
    b="efg"
    if [ ${a} = ${b} ];then
        echo "a = b"
    else
        echo "a != b"
    fi
    if [ ${a} != ${b} ];then
        echo "a != b"
    else
        echo "a = b"
    fi
    if [ -z ${a} ];then
        echo "length of a is 0"
    else
        echo "length of a is not 0"
    fi
    if [ -n ${a} ];then
        echo "length of a is not 0"
    else
        echo "length of a is 0"
    fi
    if [ ${a} ];then
        echo "a is not null"
    else
        echo "a is null"
    fi
}

test_12()
{
    #文件测试运算
    file="/home/wenshuai.xi/github/learning/shell/test.sh"
    if [ -r ${file} ];then
        echo "file is readable"
    else
        echo "file is not readable"
    fi
    if [ -w ${file} ];then
        echo "file is writeable"
    else
        echo "file is not writeable"
    fi
    if [ -x ${file} ];then
        echo "file is executable"
    else
        echo "file is not executable"
    fi
    if [ -f ${file} ];then
        echo "file is ordinary file"
    else
        echo "file is not ordinary file"
    fi
    if [ -d ${file} ];then
        echo "file is directory"
    else
        echo "file is not directory"
    fi
    if [ -s ${file} ];then
        echo "file is not null"
    else
        echo "file is null"
    fi
    if [ -e ${file} ];then
        echo "file is exist"
    else
        echo "file is not exist"
    fi
}

test_13()
{
    #echo 命令
    echo "this is test"
    echo "\"this is test\""
    read name
    echo "my name is ${name}"
    echo -e "\nmy name is ${name}\n" #-e 开启转义 \n 换行
    echo -e "ok !!! \c" #-e 开启转义 \c 不换行
    echo "my name is ${name}"
    echo "my name is ${name}" > myfile
    echo '$name\"'
    echo `date`
}

test_14()
{
    #printf 命令
    printf "%-10s%-9s%-4s\n" 姓名 性别 体重
    printf "%-10s%-9s%-4s\n" 郭靖 男 66.1
    printf "%-10s%-9s%-4s\n" 杨过 男 65.2
    printf "%-10s%-9s%-4s\n" 郭芙 女 48.2
}

test_15()
{
    #test 命令
    num1=100
    num2=100
    if test ${num1} -eq ${num2}
    then
        echo '两个数相等！'
    else
        echo '两个数不相等！'
    fi

    a=5
    b=6
    result=$[a+b] #[]执行基本的算数运算
    echo "result：$result"

    num1="ru1noob"
    num2="runoob"
    if test $num1 = $num2
    then
        echo '两个字符串相等!'
    else
        echo '两个字符串不相等!'
    fi

    if test -e ./bash
    then
        echo '文件已存在!'
    else
        echo '文件不存在!'
    fi
    if test -e ./notFile -o -e ./bash
    then
        echo '至少有一个文件存在!'
    else
        echo '两个文件都不存在'
    fi
}

test_16()
{
    #流程控制
    #条件控制
    a=10
    b=20
    if [ $a == $b ]
    then
        echo "a 等于 b"
    elif [ $a -gt $b ]
    then
        echo "a 大于 b"
    elif [ $a -lt $b ]
    then
        echo "a 小于 b"
    else
        echo "没有符合的条件"
    fi

    num1=$[2*3]
    num2=$[1+5]
    if test ${num1} -eq ${num2}
    then
        echo '两个数字相等!'
    else
        echo '两个数字不相等!'
    fi

    #循环控制
    for loop in 1 2 3 4 5
    do
        echo  "The value is: $loop"
    done

    for str in "This" "is" "a" "string"
    do
        printf "$str "
    done
    printf "\n"

    int=1
    while [ $int -le 5 ]
    do
        echo -n "$int "
        let int++ #let用于执行一个或多个表达式,变量计算中不需要加上$来表示变量
    done
    echo

    echo -n '输入你的名字:'
    while read name && [ $name != "a" ]
    do
        echo "my name is ${name}"
    done

    a=0
    until [ ! $a -lt 10 ]
    do
        echo $a
        a=`expr $a + 1`
    done

    #while true;do cat /proc/version;sleep 1;done

    #多选择语句
    echo '输入 1 到 4 之间的数字:'
    echo '你输入的数字为:'
    read num
    case ${num} in
        1)
            echo '你选择了 1'
            ;;
        2)
            echo '你选择了 2'
            ;;
        3)
            echo '你选择了 3'
            ;;
        4)
            echo '你选择了 4'
            ;;
        *)
            echo '你没有输入 1 到 4 之间的数字'
            ;;
    esac

    #跳出循环
    while true;do
        echo -n "输入 1 到 5 之间的数字:"
        read num
        case $num in
            1|2|3|4|5)
                echo "你输入的数字为 $num!"
                ;;
            *)
                echo "你输入的数字不是 1 到 5 之间的! 游戏结束"
                break
                ;;
        esac
    done

    while true;do
        echo -n "输入 1 到 6 之间的数字: "
        read num
        case $num in
            1|2|3|4|5)
                echo "你输入的数字为 $num!"
                ;;
            *)
                echo "你输入的数字不是 1 到 5 之间的!"
                continue
                echo "游戏结束"
                ;;
        esac
    done
}

test_17()
{
    funWithReturn()
    {
        echo "对输入的两个数字相加"
        read a
        read b
        return $[${a}+${b}]
    }
    funWithReturn
    echo "输入的两个数字之和:$?"
}

test_18()
{
    funWithParam(){
        echo "第一个参数为 $1 !"
        echo "第二个参数为 $2 !"
        echo "第十个参数为 $10 !"
        echo "第十个参数为 ${10} !" #当n>=10时,需要使用${n}来获取参数
        echo "第十一个参数为 ${11} !"
        echo "参数总数有$#个!"
        echo "作为一个字符串输出所有参数 $* !"
    }
    funWithParam 1 2 3 4 5 6 7 8 9 34 73
}

test_19()
{
    #文件包含
    if [ -e ./test1.sh -a -e ./test2.sh ];then
        . ./test1.sh
        source ./test2.sh
        echo "my name is ${myname},your name is ${yourname}"
    else
        echo "do nothing"
    fi
}

test_20()
{
    #输入输出重定向
    who > users
    echo "hello world" >> users
    wc -l < users
    wc -l < users > ofile
    cat ./users ./tmp.txt >log 2>&1
    cat ./tmp.txt >/dev/null 2>&1
}
