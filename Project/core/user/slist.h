#ifndef SLIST_H
#define SLIST_H
//@author:��
#include <iostream>
#include <time.h>
using namespace std;

template<typename Type>
struct Snode
{
	Type data;
	Snode* next = nullptr;
};

template<typename Type>
class SnodeArray //��Žڵ�ָ�������
{
public:
	SnodeArray(int);
	~SnodeArray();
	Snode<Type>**head;
};

template<typename Type>
SnodeArray<Type>::SnodeArray(int count) 
{
	head = new Snode<Type>*[count];
}

template<typename Type>
SnodeArray<Type>::~SnodeArray()
{
	delete head;
}

template<typename Type>
class Slist
{
protected:
	Snode<Type>* head;//ͷָ��
	Snode<Type>* tail;//βָ��
	//friend ostream& operator<<(ostream&, Slist<Type>);//��Ԫ������ostream�ĺ�����Ȩ��������//lnk2019��ʱ������
	int total = 0;	
public:
	Slist();
	Slist(Type*first, Type*last);
	~Slist();
	Snode<Type>* access(int n);//û�б��������ʵ�n+1������
	void plusmerge(Slist a, Slist b);
	void insert(Type n, int position);
	void insert(Snode<Type>*, Snode<Type>*, Type n);
	void show();//�������ÿ��Ԫ��
	void delete_node(int n);//ɾ����n+1��Ԫ��
	void shuffle();//ϴ�ƣ���������
	int count();//Ԫ������
	Snode<Type> pop();//��ȡ���һ��Ԫ��
	Slist& operator+(Type a);//ĩλ�ӵ���
};

template<typename Type>
void Slist<Type>::insert(Snode<Type>*a, Snode<Type>*b, Type n) //���������ڽڵ��м�����½ڵ�
{
	a = a->next = new Snode<Type>;
	a->data = n;
	a->next = b;
	total++;
}

template<typename Type>
void Slist<Type>::insert (Type n, int position)//��һ��������Ҫ�����ֵ���ڶ�����������λ��,���Ѳ���Ԫ����Ϊ��position��Ԫ��
{//�޷��ʿսڵ㱨��
	Snode<Type>*p1 = this->head;//ǰָ��ָ���ڱ�λ
	Snode<Type>*p2 = p1->next;//��ָ��
	for (int i = 0; i < position - 1; i++, p2 = p2->next, p1 = p1->next);//
	p1 = p1->next = new Snode<Type>;
	p1->data = n;
	p1->next = p2;
	total++;
}

template<typename Type>
void Slist<Type>::plusmerge(Slist a, Slist b) //�����������������Ϊһ�������������������������ӵ�ĩβ
{//ֱ���������ã�������
	Snode<Type>*p1 = a.head->next;//ָ���һ��Ԫ��
	Snode<Type>*p2 = b.head->next;
	for (;;)
	{
		if (p1->data >= p2->data)
		{
			*this + p2->data;
			if (p2->next == nullptr)
			{
				for (; p1 != nullptr; p1 = p1->next) { *this + p1->data; }
				break;
			}
			else p2 = p2->next;
		}
		if (p1->data < p2->data)
		{
			*this + p1->data;
			if (p1->next == nullptr)
			{
				for (; p2 != nullptr; p2 = p2->next) { *this + p2->data; }
				break;
			}
			else p1 = p1->next;
		}
	}
}

//template<typename type>
//ostream& operator<<(ostream&a, Slist<type> b)//����cout���������ȫ����
//{
//	Snode<type>*p = b.head->next;//ֱ��ָ���һ��Ԫ�أ��ޱ�����
//	a << (p->data);
//	for (; p->next != nullptr;)
//	{
//		p = p->next;
//		a <<' '<< (p->data) ;
//	}
//	return a;
//}

template<typename type>
void Slist<type>::show()
{
	Snode<type>*p = this->head->next;//ֱ��ָ���һ��Ԫ�أ��ޱ�����
	cout << (p->data);
	for (; p->next != nullptr;)
	{
		p = p->next;
		cout << ' ' << (p->data);
	}
}

template<typename Type>
Slist<Type>::Slist()
{
	this->head = new Snode<Type>;
	this->head->next = nullptr;
	tail = this->head;
}

template<typename Type>
Slist<Type>::Slist(Type*first, Type*last)
{
	Type*p1 = first;
	this->head = new Snode<Type>;//�����ڱ�λ
	Snode<Type>*p2 = this->head->next = new Snode<Type>;//p2ָ�������һ��Ԫ��
	for (;; p1++)
	{
		if (p1 == last)break;
		else
		{
			p2->data = *p1;
			p2 = p2->next = new Snode<Type>;
			total++;
		}
	}
	tail = p2;
}

template<typename Type>
Snode<Type>* Slist<Type>::access(int n)
{
	Snode<Type>*p = this->head->next;
	for (int i = 0; i < n; i++, p = p->next);
	return p;
}

template<typename Type>
Slist<Type>& Slist<Type>::operator+(Type a)//ĩλ����ڵ�
{
	Snode<Type>*p = this->head;
	for (; p->next != nullptr; p = p->next);//����ĩλ�ڵ�
	p->next = new Snode<Type>;
	total++;
	p = p->next;
	p->data = a;
	p->next = nullptr;
	tail = p;
	return *this;
}

template<typename Type>
Slist<Type>::~Slist()
{
	Snode<Type>*p;
	while (head->next != nullptr)
	{
		p = head->next;
		head->next = head->next->next;
		delete p;
	}
}

template<typename Type>
void Slist<Type>::delete_node(int n)
{
	Snode<Type>*ptr_fast=this->head->next;
	Snode<Type>*ptr_slow = this->head;
	for (int i = 0; i < n; i++, ptr_slow = ptr_fast, ptr_fast = ptr_fast->next);
	ptr_slow->next = ptr_fast->next;
	delete ptr_fast;
	total--;
}

template<typename Type>
void Slist<Type>::shuffle()
{
	SnodeArray<Type> a(total);
	Snode<Type>*ptr = head->next;
	for (int i = 0; i < total; i++) 
	{
		a.head[i] = ptr;
		ptr = ptr->next;
	}
	srand((unsigned)time(0));
	Snode<Type>*mid = nullptr;
	int rad = 0;
	for (int i = 0; i < total - 3; i++) //����ϴ��
	{
		rad = rand() % (total - i - 2);
		mid = a.head[rad];
		a.head[rad] = a.head[total - i - 1];
		a.head[total - i - 1] = mid;		
	}
	ptr = head;
	for (int i = 0; i < total; i++) 
	{
		ptr = ptr->next = a.head[i];
		
		if (i == total - 1) 
		{
			tail = a.head[i];
			a.head[i]->next = nullptr;
		}
	}
}

template<typename Type>
int Slist<Type>::count() 
{
	return total;
}


template<typename Type>
Snode<Type> Slist<Type>::pop()
{
	return *tail;
}

#endif // !SLIST_H