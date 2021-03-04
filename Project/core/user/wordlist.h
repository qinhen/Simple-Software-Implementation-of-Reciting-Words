#ifndef WORDLIST_H
#define WORDLIST_H
#include<string>
#include"slist.h"
#include"date.h"
//@author:��

struct Wordnode 
{
	short id;//�������к�
	enum {wrong,undone,right,done };
	enum {none,first,second,third,fourth,fifth};
	char st=undone;//����״̬
	char ti = none;//���Ĵ���
	Date da;//����
	Wordnode() {};
	Wordnode(short wid, char state, char time, Date date) :
		id(wid),
		st(state),
		ti(time),
		da(date)
	{}
	bool isEbbinghaus()
	{
		short lastdate = da.get_short();
		Date d;
		d.set_date();
		short today = d.get_short();
		if ((ti == Wordnode::first && (today - lastdate == 1)) ||
			(ti == Wordnode::second && (today - lastdate == 1)) ||
			(ti == Wordnode::third && (today - lastdate == 2)) ||
			(ti == Wordnode::fourth && (today - lastdate == 3)))
			return true;
		return false;
	}//�Ƿ���ϰ����ƿ���������
};

class WordList :public Slist<Wordnode>
{
public:
	WordList() {}
	~WordList() {}
	void addword(short id, char state, char times, short date)
	{
		Wordnode a;
		a.id = id;
		a.st = state;
		a.ti = times;
		a.da = Date(date);
		this->Slist::operator+(a);
		switch (state)
		{
		case Wordnode::undone: undone_t++; break;
		case Wordnode::done:done_t++; break;
		case Wordnode::wrong:wrong_t++; break;
		case Wordnode::right:right_t++;
		default:
			break;
		}
	}//��ӵ���
	void addword(Wordnode w)
	{

		this->Slist::operator+(w);
		switch (w.st)
		{
		case Wordnode::undone: undone_t++; break;
		case Wordnode::done:done_t++; break;
		case Wordnode::wrong:wrong_t++; break;
		case Wordnode::right:right_t++;
		default:
			break;
		}
	}
	Wordnode pop()
	{
		Wordnode word_obj = Slist::access(0)->data;
		return word_obj;
	}//��ȡ���ʱ��е�һ������
	int count_total()
	{
		return Slist::total;
	}
	int count_undone()
	{
		return undone_t;
	}
	int count_done()
	{
		return done_t;
	}
	int count_wrong()
	{
		return wrong_t;
	}
	int count_right()
	{
		return right_t;
	}
protected:
	int undone_t=0;//���컹û��ʼ����
	int done_t = 0;//�Ѿ������˵�
	int wrong_t = 0;//�����˵�
	int right_t = 0;//������ֻ������һ�ε�
private://���õĺ���
	void plusmerge() {};
	void show() {};
	void operator+() {};
};

class WordList_daily:public WordList
{
public:
	Date da;
	void check(bool correct)
	{
		Wordnode* word_obj = &(Slist::access(0)->data);
		word_obj->da.set_date();
		if (!(word_obj->da.istoday()))
		{
			word_obj->st = Wordnode::undone;

		}
		if (correct)
		{
			switch (word_obj->st)
			{
			case Wordnode::undone:
				word_obj->st = Wordnode::done;
				undone_t--;
				done_t++;
				word_obj->ti++;
				break;
			case Wordnode::done:
				break;
			case Wordnode::wrong:
				word_obj->st = Wordnode::right;
				wrong_t--;
				right_t++;
                                undone_t--;
                                done_t++;
				break;
			case Wordnode::right:
				word_obj->st = Wordnode::done;
				right_t--;
                                undone_t--;
                                done_t++;
				word_obj->ti++;
			}
		}
		else
		{
			switch (word_obj->st)
			{
			case Wordnode::undone:
				word_obj->st = Wordnode::wrong;
				undone_t--;
				wrong_t++;
				break;
			case Wordnode::done:
				break;
			case Wordnode::wrong:
				break;
			case Wordnode::right:
				word_obj->st = Wordnode::wrong;
				right_t--;
				wrong_t++;
			default:
				break;
			}
		}
		for (int i = count_total(); i > 0; i--)
		{
			put_tail();
			Wordnode w = pop();
			if (w.st != Wordnode::done)break;
		}
	}//���ʻ��벻��
	int count_new()
	{
		return new_word;
	}//�µ��ʸ���
	void addword(Wordnode w)
	{
		WordList::addword(w);
		if (w.ti == Wordnode::none)new_word++;
	}//����
private:
	int new_word = 0;//�µ��ʸ���
	void put_tail()
	{
		Snode<Wordnode>* ptr = Slist::head->next;
		Slist::head->next = ptr->next;
		Slist::tail = Slist::tail->next = ptr;
		ptr->next = nullptr;
	}//�����ʷŵ�ĩβ
};



//@author:��

#endif
