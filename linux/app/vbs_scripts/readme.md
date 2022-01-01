1.初始VBS、常用的关键字

初始VBS
dim　　　　声明变量：批量名称声明，多个变量之间用逗号分隔
rem　　　　注释， 同  '
const　　　声明常量；也可用Private或者Public来改变变量的公有和私有属性；
set　　　　赋值
常用的关键字
inputbox, msgbox， const, dim, rem,

2.流程控制语句---条件判断

(1)if ...else语句

这段程序让你输入两个值, 必须都大于10, 只要有一个不大于, 就输出错误提示
dim a,b
a=inputbox("输入一个数 >10")
b=inputbox("输入另一个数 >10")
a=int(a)
b=int(b)
if a>10 and b>10 then
    msgbox "正确"
elseif a<=10 or b<=10 then
　　 msgbox "至少有一个数不大于10"
else
    msgbox "错误"
end if

(2)select.. case的语法结构

dim a
a=inputbox("输入一个1--3的值")
a=int(a) 　　　　　　'处理inputbox返回字符串的问题
select case a　　　　'a为变量名
case 1　　　　　　　　 '1为变量值
    msgbox "壹"　　　　'语句
case 2
    msgbox "贰"
case 3
    msgbox "叁"
case else
    msgbox "输入错误"
end select
注：select 用于定值判断

3.流程控制语句---循环结构

(1)do（until）……loop

当程序执行到 do 之前，首先判断 until 条件是否成立，如成立则不执行下面代码，如不成立，则执行一遍，再判断条件。在do..loop结构中还有一个语句:exit do, 这个语句将终止循环, 跳到loop后面的语句继续执行。
dim input="",ctr　　　　'注意:常量不需要在dim里面声明,否则会引发错误
ctr=0 '设置计数器
const pass="pas123_"   '这是一个字符串 请用""包裹起来. 设定密码为常量, 不可变更
do until input=pass
    input=inputbox("请输入密码")
    if ctr=3 then
        msgbox "已经达到认证上限, 认证程序关闭"
    exit do
    end if
    ctr = ctr + 1'注意：这一句是赋值句，要从右往左读，即每出错一次就把ctr加上1，然后再放回ctr里面，使得这个常量加1
loop
msgbox "成功"

要在do...loop加上验证的功能,也可以用while，while可以放在do或者是loop后面, 然后再接一个表达式, 当表达式的值为true的时候(表达式成立),才运行循环体
dim a,ctr
ctr=0
const pass="pas123_"
do while ctr<3
    a=inputbox("请输入密码")
    if a=pass then
        msgbox "认证成功"
        msgbox "(你可以在这里加一段成功后得到的信息)"
        exit do
    else
        ctr=ctr+1 '如果密码出错就增加一次错误认证计数
        msgbox "认证出错, 请检查密码"
    end if
loop

(2)for....next, 这种循环结构是基于计数的

dim i,j
for i=1 to 9
    for j=1 to 9
        str=str & i * j & " "                 '&是和并字符串的符号
    next '每个next对应一个for
    str=str & vbCrlf  'vbCrlf相当于键盘上的回车键,因为你不能在键盘上输入,所以系统定义了一个默认的常量
next
msgbox (str)

4.子程序（过程）与函数（将sub替换成function）

区别：sub无返回值，function有返回值
Dim yname
yname=inputbox("请输入你的名字:")
who(yname)
sub who(yname)　　'who为子程序名或函数名，（）中为参数列表，用逗号隔开
    msgbox "你好" + yname    '&和+ 都可以用来拼接字符串
    msgbox "感谢你阅读我的课程"
    msgbox "这是基础部分的最后一课"
end sub
