#pragma once


#define MY_LIBRARY_PATH  "./bin/data0"
#define MY_CONF_PATH     "conf"
#define MY_VERSON        "learnEn (dev-V0.7)"
#define MY_CONTACT_EMAIL "xxxxxxxxxxx.com"



#define MY_LANGUAGE_CHINESE 0
#define MY_LANGUAGE_ENGLISH 1

#define MY_RELEASE_LANGUAGE 1

#if defined MY_LANGUAGE_ENGLISH


#define		MY_STRING_0 "Account"
#define		MY_STRING_1 "Start"
#define		MY_STRING_2 "Dictionary"
#define		MY_STRING_3 "Setting"

#define		MY_STRING_4 "首页"


#elif defined MY_LANGUAGE_CHINESE


#define		MY_STRING_0 "用户"
#define		MY_STRING_1 "开始"
#define		MY_STRING_2 "字典"
#define		MY_STRING_3 "设置"


#define		MY_STRING_4 "首页"

#endif // MY_LANGUAGE_ENGLISH






