#ifndef DATE_H
#define DATE_H
//author:霄
#include<time.h>
#include"slist.h"
#define DATE_DAY 86400

class Date 
{
public:
	Date(short da = 0) :date(da) {};
	void set_date()
	{
		time_t t;
		t = time(0) + 28800;
		t /= DATE_DAY;
		date = t;
	}//取现在时间作为日期
	char* get_date()
	{
		time_t t;
		struct tm p;
		t = date * DATE_DAY;
		gmtime_s(&p, &t);
		static char s[11];//11个字符，其中包括空字符
		strftime(s, 11, "%Y-%m-%d", &p);
		return s;
	}//格式："YYYY-MM-DD"
	bool istoday()
	{
		short date_today;
		date_today = (time(0) + 28800) / DATE_DAY;
		return date_today == date;
	}
	short get_short() { return date; }
private:
	short date = 0;//仅包含日期，不包含具体时间的时间戳
	//char* get_date_now();
	//char* get_time_now();
};



//char* Date::get_date_now()
//{
//	time_t t;
//	struct tm *p;
//	t = time(0)+28800;
//	p = gmtime(&t);
//	char *s=new char[11];//11个字符，其中包括空字符
//	strftime(s, sizeof(s), "%Y-%m-%d", p);
//}
//
//char* Date::get_time_now()
//{
//	time_t t;
//	struct tm *p;
//	t = time(0) + 28800;
//	p = gmtime(&t);
//	char *s = new char[20];//20个字符，其中包括空字符
//	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", p);
//
//}

#endif // !DATE_H
