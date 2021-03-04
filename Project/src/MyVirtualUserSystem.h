#pragma once

// 2019/8/12   @��

#ifndef MY_VIRTUAL_USER_H_
#define MY_VIRTUAL_USER_H_

#include<iostream>
#include<string>
#include<vector>


// 4�� ui�������õ���
//1. MyVirtualWord�� ����
//2. MyWordList��    ���ʱ�STL::vector��
//3. MyMyVirtualUserInfo�� �û���Ϣ
//4. MyVirtualUserSystem�� �û�ϵͳ



// ����
class MyVirtualWord
{
public:
	// ui����� request�������ɵ��ʱ�ʱ�����õ�����
	unsigned int correctnWordIndex;     //��ȷѡ�������ֵ��ȡֵΪ 0-22000
	unsigned int correctOptionIndex;    //��ȷѡ������ֵ��ȡֵΪ 0-3���������ݣ�
	unsigned int optionWordIndices[4];  //�ĸ�ѡ��ĵ�������ֵ��ȡֵΪ 0-22000
	

	// ui������û�ѡ����� �ı����������
	bool isChecked = false;                  //�Ƿ��Ѿ����й�
    bool isCorrect = false;
	unsigned int userChoiceOptionIndex = 0;  //�û�ѡ���ѡ������ֵ���� (userChoiceOptionIndex == correctOptionIndex)����ѡ����ȷ


	MyVirtualWord(
		unsigned option0, // ��һ��ѡ������� 0-22000
		unsigned option1, // �ڶ���ѡ������� 0-22000
		unsigned option2, // ������ѡ������� 0-22000
		unsigned option3, // ���ĸ�ѡ������� 0-22000
        unsigned coOptionIndex // ��ȷѡ����� 0-3
	);

};




// ���ʱ�
typedef std::vector<MyVirtualWord>  MyWordList;




// �û���Ϣ
class MyVirtualUserInfo
{
public:
	std::string m_userName;           // ��ǰ��¼�û���
	std::string m_userHeadImagePath;  // ��ǰ��¼�û�ͷ���ļ�·��
	// ��������...

	//TODO

	void clear()
	{
		m_userName.clear();
		m_userHeadImagePath.clear();
	}

};




inline std::vector<MyVirtualWord> debug_set_works()
{

    std::vector<MyVirtualWord> works;
    for (int i = 50; i < 55; i++)
        works.push_back(MyVirtualWord(i,i+1,i+2,i+3,2));
    return works;
}

inline std::vector<std::string> debug_set_allUsersName()
{
    return {"cxk","usb"};
}




// �û�ϵͳ
class MyVirtualUserSystem
{
private:

	MyVirtualUserInfo m_userInfo;

	bool m_isLogined;        // �����û���¼
	
	bool m_isRunning;         // ���ڱ�����        ��request();��һ�����ʱ������һ�û�з��ر��н����

public:

//[���û���¼״̬��صĺ�����4��]
//

	//��¼��
	//�����˻������¼�����أ��ɹ�����true
	bool login(std::string account, std::string password);

	//�ǳ�
	//
	void logout();

	//ע��(���������벻����Ҫ�󷵻�false / �ɹ��򱣴��û�)
	bool reg(std::string account, std::string password);


	//ɾ����ǰ��½���û���δ��¼����û����ڲ���ɾ����״̬��ֱ���˳���
	void del();


//[����������صĺ�����2��]
// 1.������ʱ����ͣ�ص���request����ֱ�����ص��ʱ���û�е��ʣ�ͬʱrequest�����账�����汾�α��������
// 2.�� (m_isLogined == false) ʱ��������Ч

	//��ȡһ�����ʱ���
	//���� m_isRunning = true ���� (m_isRunning == true) ʱ������request������Ч������ǰ�Ѿ���ȡ���ʱ��ڻش������ʽ��ǰ�������ٴλ�ȡ
	MyWordList request();

	//�ش������
	//���� m_isRunning = false ���� (m_isRunning == false) ʱ������feedback������Ч������ǰû�л�ȡ���ʱ����ܻش����ʽ��
	void feedback(MyWordList const&);




//[UI�����õ��ĺ�����2��]

	std::string get_userName();                    // ��ǰ�û�����
	std::string get_userHeadImagePath();           // ��ǰ�û�ͷ��·��
	std::vector<std::string> get_all_userNames();  // �����û�����



private:
	//[�ڲ�ʹ�õĺ�����2��]

	// ���²�������������
	void save_all();
	// ���ϵͳ��������
	void clear_all();


public:

    // ��ʼ��
    void init();

	bool is_logined() { return m_isLogined; }
	bool is_running() { return m_isRunning; }



public:

    bool debug_login()
    {
        m_isLogined = true;
        return true;
    }

    std::string debug_get_userName()
    {
        return "cxk";
    }

};




//


inline MyVirtualWord::MyVirtualWord(
	unsigned option0,
	unsigned option1,
	unsigned option2,
	unsigned option3,

    unsigned coOptionIndex
) {
	// 1
    correctOptionIndex = coOptionIndex;
	// 2
    switch (coOptionIndex)
	{
	case 0:correctnWordIndex = option0; break;
	case 1:correctnWordIndex = option1; break;
	case 2:correctnWordIndex = option2; break;
	case 3:correctnWordIndex = option3; break;
	default:
		// ����Ĭ��Ϊ0
		correctnWordIndex = 0;
	}
	// 3
	optionWordIndices[0] = option0;
	optionWordIndices[1] = option1;
	optionWordIndices[2] = option2;
	optionWordIndices[3] = option3;
	// 4
	isChecked = false;
	// 5
	userChoiceOptionIndex = 0;
}



inline bool MyVirtualUserSystem::login(std::string account, std::string password)
{

    return debug_login();

	bool success = false;

	// 
	// TODO ȷ���û�����
	// 

	if (success)
	{
		save_all();
		clear_all();
		// �����û���Ϣ

		//TODO
	}
	else
	{
		//TODO
	}

	return success;
}

inline void MyVirtualUserSystem::logout()
{
	save_all();
	clear_all();
}

inline bool MyVirtualUserSystem::reg(std::string account, std::string password)
{



	//TODO
	return false;
}

inline void MyVirtualUserSystem::del()
{

	//TODO
}

inline MyWordList MyVirtualUserSystem::request()
{
	// TODO

	//����Ƿ��ܹ����ص��ʱ�
	//���������ɵ��ʱ�
	//�������򷵻ؿյ��ʱ�

	//1.û���û���¼                         ���յ��ʱ�
	//2.�Ѿ��������ʱ������н����û�з��� ���յ��ʱ�
	//3.�������������                       ���յ��ʱ�
	//...
	//����                                   �������û���־���ɵ��յ��ʱ�����ݷ��صĽ�����ɱ���Ĳ��ֵ�����ɵĵ��ʱ����û����У�ֱ��û�д���Ϊֹ



	m_isRunning = true;

    return debug_set_works();

	return MyWordList();
}


inline void MyVirtualUserSystem::feedback(MyWordList const &)
{
	// TODO


	//����Ƿ��ܹ����ܵ��ʱ�
	//������������ݡ���鿴���еĶԴ��������ĵ����򱳴������һ�������´ε���request����ʱ������ָõ���
	//��������ֱ���˳�

	//1.û���û���¼                         
	//2.��δ�������ʱ�
	//3.�������������
	//...

	m_isRunning = false;
}







inline std::string MyVirtualUserSystem::get_userName()
{
    return debug_get_userName();//

	if (m_isLogined)
		return m_userInfo.m_userName;
	else
		return std::string();
}

inline std::string MyVirtualUserSystem::get_userHeadImagePath()
{
	if (m_isLogined)
		return m_userInfo.m_userHeadImagePath;
	else
		return std::string();
}

inline std::vector<std::string> MyVirtualUserSystem::get_all_userNames()
{
	// TODO

    return debug_set_allUsersName();

	return std::vector<std::string>();
}







inline void MyVirtualUserSystem::init()
{
	//TODO

}

inline void MyVirtualUserSystem::save_all()
{

	if (!m_isLogined)//��û���û���¼�����豣��
	{

		// TODO
	}
	else
	{
		//TODO

	}

}

inline void MyVirtualUserSystem::clear_all()
{
	m_userInfo.clear();
	m_isLogined = false;
	m_isRunning = false;
	// TODO


}



#endif // !MY_VIRTUAL_USER_H_
