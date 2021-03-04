#pragma once

#ifndef MY_CONFIGURATION_H_
#define MY_CONFIGURATION_H_

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>


#define ARREST(e) e

void stRemoveBlank(char** dest, int* sublen, const char* src, int len);
void stRemoveQuote(char** dest, int* sublen, const char* src, int len);
bool stGetPair(char** key, int* keylen, char** value, int* valuelen, const char* src, int len);


inline void stRemoveBlank(char** dest, int* sublen, const char* src, int len)
{
#	ifdef _DEBUG
    if (
        dest == nullptr ||
        sublen == nullptr
        )
        ARREST();
#	endif

    int start = 0, end = len - 1;
    auto f = [](char c) -> bool {return c == ' ' || c == '\t' || c == '\n'; };
    bool flag = false;
    for (int i = 0; i < len; i++)
    {
        if (!f(src[i])) {
            flag = true;
            start = i;
            break;
        }
    }

    if (!flag) {
        dest = nullptr;
        sublen = 0;
    }

    for (int i = len - 1; i >= 0; i--)
    {
        if (!f(src[i])) {
            end = i;
            break;
        }
    }

    int subStrLen = end - start + 1;

    if (subStrLen < 1) {
        dest = nullptr;
        sublen = 0;
    }

    char* res = new char[subStrLen + 1];
    strncpy_s(res, subStrLen + 1, src + start, subStrLen);
    res[subStrLen] = '\0';
    *dest = res;
    *sublen = subStrLen;
}


inline void stRemoveQuote(char** dest, int* sublen, const char* src, int len)
{
#	ifdef _DEBUG
    if (
        dest == nullptr ||
        sublen == nullptr
        )
        ARREST();
#	endif

    int start = 0, end = len - 1;
    auto f = [](char c) -> bool {return c == '\"' || c == ' ' || c == '\t' || c == '\n'; };
    bool flag = false;
    for (int i = 0; i < len; i++)
    {
        if (!f(src[i])) {
            flag = true;
            start = i;
            break;
        }
    }

    if (!flag) {
        dest = nullptr;
        sublen = 0;
    }

    for (int i = len - 1; i >= 0; i--)
    {
        if (!f(src[i])) {
            end = i;
            break;
        }
    }

    int subStrLen = end - start + 1;

    if (subStrLen < 1) {
        dest = nullptr;
        sublen = 0;
    }

    char* res = new char[subStrLen + 1];
    strncpy_s(res, subStrLen + 1, src + start, subStrLen);
    res[subStrLen] = '\0';
    *dest = res;
    *sublen = subStrLen;
}



inline bool stGetPair(char** key, int* keylen, char** value, int* valuelen, const char* src, int len)
{
#	ifdef _DEBUG
    if (
        key == nullptr ||
        keylen == nullptr ||
        value == nullptr ||
        valuelen == nullptr ||
        len < 3
        )
        ARREST();
    return false;
#	endif

    int equal_sign_index = 0;
    int have_equal_sign_flag = false;
    for (int i = 0; i < len; i++)
    {
        if (src[i] == '=') {
            have_equal_sign_flag = true;
            equal_sign_index = i;
            break;
        }
    }

    if (!have_equal_sign_flag)
        return false;

    int key_superset_len = equal_sign_index - 1;
    int val_superset_len = len - equal_sign_index - 1;

    if (
        key_superset_len == 0 ||
        val_superset_len == 0
        )
        return false;

    char* key_superset = new char[key_superset_len + 1];// new
    char* val_superset = new char[val_superset_len + 1];// new
    strncpy_s(key_superset, key_superset_len + 1, src, key_superset_len);
    strncpy_s(val_superset, val_superset_len + 1, src + equal_sign_index + 1, val_superset_len);
    key_superset[key_superset_len] = '\0';
    val_superset[val_superset_len] = '\0';

    char* res_key = nullptr;
    char* res_val = nullptr;
    int res_key_len = 0;
    int res_val_len = 0;

    stRemoveBlank(&res_key, &res_key_len, key_superset, key_superset_len);
    stRemoveBlank(&res_val, &res_val_len, val_superset, val_superset_len);

    delete[] key_superset;
    delete[] val_superset;

    if (
        res_key_len == 0 ||
        res_val_len == 0
        )
        return false;

    *key = res_key;
    *keylen = res_key_len;
    *value = res_val;
    *valuelen = res_val_len;
    return true;
}



class MyConfiguration
{
private:


    std::string m_userName;
    long long m_timeStamp;
    int m_windowWidth;
    int m_windowHeight;
    std::string m_backgroundImagePath;
    std::string m_filePath;


    enum key_type
    {
        unkonw = 0,
        windowWidth = 1,
        windowHeight = 2,
        backgroundImagePath = 3,
    };

    key_type translate(std::string const& key)
    {
        if (key == "windowWidth")
            return windowWidth;
        else if (key == "windowHeight")
            return windowHeight;
        else if (key == "backgroundImagePath")
            return backgroundImagePath;
        else
            return unkonw;
    }


    void set_attrib(std::string const& key, std::string const& val)
    {
        switch (translate(key))
        {
        case windowWidth:
            set_windowWidth(std::stoi(val));
            break;
        case windowHeight:
            set_windowHeight(std::stoi(val));
            break;
        case backgroundImagePath:
            m_backgroundImagePath = val.substr(1, val.size() - 2);
        default:
            break;
        }
    }


public:

    MyConfiguration()
    {
        defaault();
    }

    bool load(const char* path = nullptr)
    {
        std::ifstream file(path);
        m_filePath = std::string(path);
        if (!file.is_open())
            return false;
        std::string line;
        while (!file.eof())
        {
            std::getline(file, line);
            char* key;
            int key_len = 0;
            char* val;
            int val_len = 0;

            bool valid_flag = stGetPair(&key, &key_len, &val, &val_len, line.c_str(), line.size());
            if (valid_flag) {
                set_attrib(std::string(key), std::string(val));
                if (key_len)
                    delete[] key;
                if (key_len)
                    delete[] val;
            }

        }
        return true;
    }

    void save()
    {
        std::ofstream file(m_filePath, std::ios::ate);
        file << "*This is a configuration file which predefined some of the parameters used in constructor." << std::endl;
        file << "*Created by: The Configuration Manager for Team version 0.0.1" << std::endl;
        file << "*WARNING! All changes made in this file may cause damage!" << std::endl;
        file << std::endl;

        file << "\tuserName = \"" << m_userName << "\"" << std::endl;
        file << "\ttimeStamp = " << m_timeStamp << std::endl;
        file << "\twindowWidth = " << m_windowWidth << std::endl;
        file << "\twindowHeight = " << m_windowHeight << std::endl;
        file << "\tbackgroundImagePath = \"" << m_backgroundImagePath << '\"' << std::endl;
        file.close();
    }


    void defaault()
    {
        m_userName = "";
        m_timeStamp = 0;
        m_windowWidth = 1280;
        m_windowHeight = 720;
        m_backgroundImagePath = "";
    }

    void set_windowWidth(int v) { m_windowWidth = v; }
    void set_windowHeight(int v) { m_windowHeight = v; }
    void set_backgroundImagePath(std::string const& v) { m_backgroundImagePath = v; }

    int get_windowWidth() { return m_windowWidth; }
    int get_windowHeight() { return m_windowHeight; }
    std::string get_backgroundImagePath() { return m_backgroundImagePath; }
};


#endif // !MY_CONFIGURATION_H_
