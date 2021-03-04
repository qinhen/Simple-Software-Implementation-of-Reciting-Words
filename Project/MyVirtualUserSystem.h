#pragma once

// 2019/8/12   @��

#ifndef MY_VIRTUAL_USER_H_
#define MY_VIRTUAL_USER_H_

#include<iostream>
#include<string>
#include<vector>


#include"account.h"

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
    bool isNull = true;

    MyVirtualWord()
    {
        isNull = true;
    }

    MyVirtualWord(
        unsigned option0, // ��һ��ѡ������� 0-22000
        unsigned option1, // �ڶ���ѡ������� 0-22000
        unsigned option2, // ������ѡ������� 0-22000
        unsigned option3, // ���ĸ�ѡ������� 0-22000
        unsigned coOptionIndex // ��ȷѡ����� 0-3
    );

};

//һ.��¼
//0ʧ�� 1�ɹ�
//
//		��.��ȡwordlist
//		0ʧ�� 1�ɹ�
//
//				��.��������


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



    MyVirtualWord request_w()
    {
        //����Ƿ����û���¼
        if(!m_isLogined)
            return MyVirtualWord();//���ؿ�����

        //����Ƿ��ڻش�����״̬
        if (m_isRunning)
            return MyVirtualWord();//���ؿ�����

        if(AccountManageSystem::ar == nullptr)
            return MyVirtualWord();//���ؿ�����

        int      correct_option_num = rand() % 4;

        if(AccountManageSystem::ar->wl_daily.count_total()==AccountManageSystem::ar->wl_daily.count_done())
            return MyVirtualWord();//���ؿ�����

        unsigned correct_option_index = AccountManageSystem::ar->wl_daily.pop().id;
        unsigned int options_index[4] = { correct_option_index,correct_option_index,correct_option_index,correct_option_index };

        for (int k = 0; k < 4; k++)
        {
            options_index[k] += (k - correct_option_num);
        }

        MyVirtualWord word
            (
                options_index[0],
                options_index[1],
                options_index[2],
                options_index[3],
                correct_option_num
            );

        m_isRunning = true;
        return word;
    }

    bool feedback_w(const MyVirtualWord& word)
    {
        if(!m_isRunning)
            return false;

        if(AccountManageSystem::ar == nullptr)
            return false;

        AccountManageSystem::ar->wl_daily.check(word.isCorrect);

        if(AccountManageSystem::ar->wl_daily.count_total()==AccountManageSystem::ar->wl_daily.count_done())
            AccountManageSystem::ar->complete();


        m_isRunning = false;
        return true;
    }





//[UI�����õ��ĺ�����2��]

    std::string get_userName();                    // ��ǰ�û�����
    std::string get_userHeadImagePath();           // ��ǰ�û�ͷ��·��
    std::vector<std::string> get_all_userNames();  // �����û�����


public:
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
    //
    isNull = false;
}



inline bool MyVirtualUserSystem::login(std::string account, std::string password)
{


    if (AccountManageSystem::sign_in(account,password))
    {
        m_isLogined = true;
        AccountManageSystem::ar->create_daily_wordlist();
        return true;
    }
    else
    {
        m_isLogined = false;
        return false;
    }

}

inline void MyVirtualUserSystem::logout()
{
    save_all();
    clear_all();
    AccountManageSystem::sign_out();
}

inline bool MyVirtualUserSystem::reg(std::string account, std::string password)
{
    return AccountManageSystem::new_account(account, password);
}

inline void MyVirtualUserSystem::del()
{
    AccountManageSystem::delete_current_account();
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

    //����Ƿ����û���¼
    if(!m_isLogined)
        return MyWordList();//���ؿ�����

    //����Ƿ��ڻش�����״̬
    if (m_isRunning)
        return MyWordList();//���ؿ�����

    if(AccountManageSystem::ar == nullptr)
        return MyWordList();//���ؿ�����

    if(AccountManageSystem::ar->wl_daily.count_total()<=0)
    {
        AccountManageSystem::ar->create_daily_wordlist();
    }


    if(AccountManageSystem::ar->wl_daily.count_total()<=0)
    {
        return MyWordList();//���ؿ�����
    }

    std::cout<<"���յ�����������Ϊ"<<AccountManageSystem::ar->wl_daily.count_total()<<std::endl;
    std::cout<<"���ʱ�����������Ϊ"<<AccountManageSystem::ar->wl_new.count_total()<<std::endl;
    std::cout<<"ÿ��������������Ϊ"<<AccountManageSystem::ar->go<<std::endl;

    MyWordList my_word_list;

    for (int i = 0; i < AccountManageSystem::ar->wl_daily.count_total(); i++)
    {

        int      correct_option_num = rand() % 4;
        unsigned correct_option_index = AccountManageSystem::ar->wl_daily.access(i)->data.id;
        unsigned int options_index[4] = { correct_option_index,correct_option_index,correct_option_index,correct_option_index };

        for (int k = 0; k < 4; k++)
        {
            options_index[k] += (k - correct_option_num);
        }

        MyVirtualWord word
            (
                options_index[0],
                options_index[1],
                options_index[2],
                options_index[3],
                correct_option_num
            );
        my_word_list.push_back(word);
    }

    m_isRunning = true;//��Ϊ�ش�����״̬

    return my_word_list;
}


inline void MyVirtualUserSystem::feedback(MyWordList const & my_word_list)
{
    // TODO

    //����Ƿ��ܹ����ܵ��ʱ�
    //������������ݡ���鿴���еĶԴ��������ĵ����򱳴������һ�������´ε���request����ʱ������ָõ���
    //��������ֱ���˳�

    //1.û���û���¼
    //2.��δ�������ʱ�
    //3.�������������
    //...

    //����Ƿ����û���¼
    if (!m_isLogined)
        return;

    //����Ƿ��ڻش�����״̬
    if (!m_isRunning)
        return;

    m_isRunning = false;

    if (AccountManageSystem::ar == nullptr)
        return;

    for (int i = 0; i < my_word_list.size(); i++)
    {
        AccountManageSystem::ar->wl_daily.check(my_word_list[i].isCorrect);
    }

}



inline std::string MyVirtualUserSystem::get_userName()
{
    return AccountManageSystem::get_current_user_name();
}

inline std::string MyVirtualUserSystem::get_userHeadImagePath()
{
    // TODO
    return std::string();
}

inline std::vector<std::string> MyVirtualUserSystem::get_all_userNames()
{
    return AccountManageSystem::get_all_users_name();
}



inline void MyVirtualUserSystem::init()
{
    AccountManageSystem::init();
    //TODO
}

inline void MyVirtualUserSystem::save_all()
{

    AccountManageSystem::save();



    if (!m_isLogined)//��û���û���¼�����豣��
    {

        // TODO
    }
    else
    {
        // TODO
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
