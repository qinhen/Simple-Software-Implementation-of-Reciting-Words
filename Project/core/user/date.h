#ifndef DATE_H
#define DATE_H
//author:��
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
	}//ȡ����ʱ����Ϊ����
	char* get_date()
	{
		time_t t;
		struct tm p;
		t = date * DATE_DAY;
		gmtime_s(&p, &t);
		static char s[11];//11���ַ������а������ַ�
		strftime(s, 11, "%Y-%m-%d", &p);
		return s;
	}//��ʽ��"YYYY-MM-DD"
	bool istoday()
	{
		short date_today;
		date_today = (time(0) + 28800) / DATE_DAY;
		return date_today == date;
	}
	short get_short() { return date; }
private:
	short date = 0;//���������ڣ�����������ʱ���ʱ���
	//char* get_date_now();
	//char* get_time_now();
};



//char* Date::get_date_now()
//{
//	time_t t;
//	struct tm *p;
//	t = time(0)+28800;
//	p = gmtime(&t);
//	char *s=new char[11];//11���ַ������а������ַ�
//	strftime(s, sizeof(s), "%Y-%m-%d", p);
//}
//
//char* Date::get_time_now()
//{
//	time_t t;
//	struct tm *p;
//	t = time(0) + 28800;
//	p = gmtime(&t);
//	char *s = new char[20];//20���ַ������а������ַ�
//	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", p);
//
//}

#endif // !DATE_H
