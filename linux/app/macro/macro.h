/*  #:字符串化一个宏参数,即参数名字前后加上"
    ##:拼接宏参数和另一个符号,即连接两个符号生成一个新的符号
    note:如果#/##操作的参数也是一个宏,那么这个宏将不会被继续展开,可以定义辅助宏过度一下
*/
#define MAJOR_VERSION 2
#define SUB_VERSION 3

#define MACRO_TO_STR(macro) #macro
#define VERSION_STR(major_version,sub_version) ({char *tmp = sub_version/100 ? \
                                    "api_version_"MACRO_TO_STR(major_version)"."MACRO_TO_STR(sub_version) : sub_version/10 ? \
                                    "api_version_"MACRO_TO_STR(major_version)".0"MACRO_TO_STR(sub_version) : \
                                    "api_version_"MACRO_TO_STR(major_version)".00"MACRO_TO_STR(sub_version);tmp;})

#define API_VERSION VERSION_STR(MAJOR_VERSION,SUB_VERSION)

/*函数式宏*/
#define check_condition(condition) \
({\
    int _condition = condition;\
    if(_condition > 1)\
        _condition = 1;\
    _condition;\
 })
#define _test_macro(string,condition)  \
({\
    if(condition)\
        printf("condition is true string:%s\n",string);\
 })
#define test_macro(string,condition) _test_macro(string,check_condition(condition))
