#pragma once

// 2019/8/12   @衡

#ifndef MY_VIRTUAL_USER_H_
#define MY_VIRTUAL_USER_H_

#include<iostream>
#include<string>
#include<vector>


// 4个 ui层必须调用的类
//1. MyVirtualWord： 单词
//2. MyWordList：    单词表（STL::vector）
//3. MyMyVirtualUserInfo： 用户信息
//4. MyVirtualUserSystem： 用户系统



// 单词
class MyVirtualWord
{
public:
	// ui层调用 request（）生成单词表时决定好的数据
	unsigned int correctnWordIndex;     //正确选项单词索引值，取值为 0-22000
	unsigned int correctOptionIndex;    //正确选项索引值，取值为 0-3（冗余数据）
	unsigned int optionWordIndices[4];  //四个选项的单词索引值，取值为 0-22000
	

	// ui层根据用户选择情况 改变或填充的数据
	bool isChecked = false;                  //是否已经背诵过
    bool isCorrect = false;
	unsigned int userChoiceOptionIndex = 0;  //用户选择的选项索引值，若 (userChoiceOptionIndex == correctOptionIndex)，则选择正确


	MyVirtualWord(
		unsigned option0, // 第一个选项单词索引 0-22000
		unsigned option1, // 第二个选项单词索引 0-22000
		unsigned option2, // 第三个选项单词索引 0-22000
		unsigned option3, // 第四个选项单词索引 0-22000
        unsigned coOptionIndex // 正确选项序号 0-3
	);

};




// 单词表
typedef std::vector<MyVirtualWord>  MyWordList;




// 用户信息
class MyVirtualUserInfo
{
public:
	std::string m_userName;           // 当前登录用户名
	std::string m_userHeadImagePath;  // 当前登录用户头像文件路径
	// 其他属性...

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




// 用户系统
class MyVirtualUserSystem
{
private:

	MyVirtualUserInfo m_userInfo;

	bool m_isLogined;        // 已有用户登录
	
	bool m_isRunning;         // 正在背单词        （request();了一个单词本，并且还没有返回背诵结果）

public:

//[跟用户登录状态相关的函数：4个]
//

	//登录。
	//输入账户密码登录并加载，成功返回true
	bool login(std::string account, std::string password);

	//登出
	//
	void logout();

	//注册(重名或密码不符合要求返回false / 成功则保存用户)
	bool reg(std::string account, std::string password);


	//删除当前登陆的用户（未登录或该用户存在不可删除的状态则直接退出）
	void del();


//[跟背单词相关的函数：2个]
// 1.背单词时将不停地调用request（）直到返回单词表中没有单词，同时request（）需处理并保存本次背单词情况
// 2.当 (m_isLogined == false) 时都调用无效

	//获取一个单词本。
	//设置 m_isRunning = true ，当 (m_isRunning == true) 时，调用request（）无效，即先前已经获取单词表，在回传背单词结果前，不能再次获取
	MyWordList request();

	//回传结果。
	//设置 m_isRunning = false ，当 (m_isRunning == false) 时，调用feedback（）无效，即先前没有获取单词表，不能回传单词结果
	void feedback(MyWordList const&);




//[UI界面用到的函数：2个]

	std::string get_userName();                    // 当前用户名称
	std::string get_userHeadImagePath();           // 当前用户头像路径
	std::vector<std::string> get_all_userNames();  // 所有用户名称



private:
	//[内部使用的函数：2个]

	// 更新并保存所有数据
	void save_all();
	// 清除系统所有数据
	void clear_all();


public:

    // 初始化
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
		// 出错默认为0
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
	// TODO 确定用户密码
	// 

	if (success)
	{
		save_all();
		clear_all();
		// 加载用户信息

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

	//检查是否能够返回单词表
	//若能则生成单词表
	//若不能则返回空单词表

	//1.没有用户登录                         ：空单词表
	//2.已经给出单词表，但背诵结果还没有返回 ：空单词表
	//3.当日任务已完成                       ：空单词表
	//...
	//其他                                   ：根据用户日志生成当日单词表，或根据返回的结果生成背错的部分单词组成的单词表让用户背诵，直到没有错了为止



	m_isRunning = true;

    return debug_set_works();

	return MyWordList();
}


inline void MyVirtualUserSystem::feedback(MyWordList const &)
{
	// TODO


	//检查是否能够接受单词表
	//若能则更新数据。如查看背诵的对错情况，错的单词则背错计数加一，并且下次调用request（）时将会出现该单词
	//若不能则直接退出

	//1.没有用户登录                         
	//2.还未给出单词表
	//3.当日任务已完成
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

	if (!m_isLogined)//若没有用户登录，无需保存
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
