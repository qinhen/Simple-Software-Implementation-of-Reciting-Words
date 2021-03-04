#pragma once
#ifndef WORD_LIBRARY_H_
#define WORD_LIBRARY_H_

#include"Word.h"
#include<fstream>
#include<Windows.h>

namespace team
{

        enum ErrorEnum :unsigned
        {
                NONE,             //没有错误
                WORD_LIBRARY_FAILED_TO_OPEN_FILE,//无法打开文件
                WORD_LIBRARY_INVALID_FILE,     //非可链接词库文件
                WORD_LIBRARY_NO_FOUND,    //没有在当前词库内找到目标单词的数据
                WORD_LIBRARY_NO_LIBRARY_LINKED,//没有链接任何词库
        };


        /**
        * class WordLibrary
        * brief：提供 1.链接本地词库 和 2.从中查询单词 的接口。
        **/
        class WordLibrary
        {
        public:

                ~WordLibrary();

                WordLibrary();

                // 链接本地词库
                bool link(const char* path);
                bool link(const std::string& path);


                //功能函数

                // 读入单词 LOC编码
                Word read(const char* word);
                Word read(const std::string& word);
                Word read(const unsigned int index);

                // 读入单词 UTF-8编码
                Word read_utf8(const std::string& word);
                Word read_utf8(const unsigned int index);


                // 读入单词拼写
                std::string read_spelling_at(const unsigned int index);
                // 获得单词索引
                unsigned index_at(const std::string& spelling) { return m_hashComp.hash(spelling.c_str(), spelling.size()); }

                // 词库中包含该词
                bool word_is_inside(const char* spelling, unsigned len);

                // 模糊查询
                int fuzzy_query(const std::string& text);


                // 是否已链接
                bool is_linked()const { return m_isLinked; }
                // 清除链接关系
                void clear();
                // 获得最近的一个错误
                ErrorEnum get_latest_error()const { return m_latestError; }


        private:

                // 哈希序列
                struct OpmphfComp {
                        int m_numBucket;
                        const int* m_weight0;
                        const int* m_weight1;
                        const int* m_gBucket;

                        unsigned hash(const char* str, int len) {
                                unsigned hash0 = 0, hash1 = 0;
                                for (int i = 0; i < len; i++)
                                {
                                        hash0 += m_weight0[i] * str[i];
                                        hash1 += m_weight1[i] * str[i];
                                }
                                return m_gBucket[hash0%m_numBucket] + m_gBucket[hash1%m_numBucket];
                        }
                };

        private:

                std::string   m_path;
                unsigned      m_numWord;
                unsigned      m_numHashWeight;
                unsigned      m_offsetBlockHeader;
                unsigned      m_offsetBlock;
                int*          m_serHashData;
                OpmphfComp    m_hashComp;
                bool          m_isLinked;
                ErrorEnum     m_latestError;
                std::ifstream m_dataFile;
        private:
                StringArray INTERNAL_TO_LINE(const std::string& str);
                std::string INTERNAL_GET_STREAM_LOC(const std::string& spelling);
                std::string INTERNAL_GET_STREAM_UTF8(const std::string& spelling);
                Word::type  INTERNAL_GET_TYPE(const std::string& trans);
                Word        INTERNAL_INTERPRET(const std::string& str);
                char*       INTERNAL_UTF8_2_LOC(const char* utf8);

        };



        //------------------------------------------------------------------//
        //                       以下为函数实现                             //

        inline StringArray WordLibrary::INTERNAL_TO_LINE(const std::string & str) {
                StringArray lines;
                int separation = 0;
                for (int i = 1; i < str.size(); i++) {
                        if (str[i] == '\n') {
                                std::string substring = str.substr(separation, i - separation - 1);
                                lines.emplace_back(substring);
                                separation = i + 1;
                        }
                }
                return lines;
        }

        inline std::string WordLibrary::INTERNAL_GET_STREAM_LOC(const std::string & spelling)
        {
                if (!m_isLinked) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_NO_LIBRARY_LINKED;
                        return std::string();
                }

                if (spelling.size() == 0) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_NO_FOUND;
                        return std::string();
                }

                if (!m_dataFile.is_open()) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_FAILED_TO_OPEN_FILE;
                        return std::string();
                }

                if (!(spelling.size() < m_numHashWeight)) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_NO_FOUND;
                        return std::string();
                }
                unsigned index = m_hashComp.hash(spelling.c_str(), spelling.size());
                if (!(index < m_numWord)) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_NO_FOUND;
                        return std::string();
                }

                int offset_size[2];
                m_dataFile.seekg(m_offsetBlockHeader + index * sizeof(int) * 2, std::ios::beg);
                m_dataFile.read((char*)offset_size, sizeof(int) * 2);
                m_dataFile.seekg(m_offsetBlock + offset_size[0], std::ios::beg);


                char* data = new char[offset_size[1] + 1];
                m_dataFile.read(data, offset_size[1] + 1);
                data[offset_size[1]] = '\0';
                char* test = new char[spelling.size() + 1]();
                strncpy_s(test, spelling.size() + 1, data + 2, spelling.size());//每一份数据里的第0字节为'#',第1字节为' '.
                //test[word.size()] = '\0';

                if (!(test == spelling)) {
                        delete[] data;
                        delete[] test;
                        m_latestError = ErrorEnum::WORD_LIBRARY_NO_FOUND;
                        return std::string();
                }

                auto data_GB = INTERNAL_UTF8_2_LOC(data);
                std::string result(data_GB);//转为string
                delete data_GB;

                delete[] test;
                delete[] data;

                return result;
        }

        inline std::string WordLibrary::INTERNAL_GET_STREAM_UTF8(const std::string & spelling)
        {
                if (!m_isLinked) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_NO_LIBRARY_LINKED;
                        return std::string();
                }

                if (spelling.size() == 0) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_NO_FOUND;
                        return std::string();
                }

                if (!m_dataFile.is_open()) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_FAILED_TO_OPEN_FILE;
                        return std::string();
                }

                if (!(spelling.size() < m_numHashWeight)) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_NO_FOUND;
                        return std::string();
                }
                unsigned index = m_hashComp.hash(spelling.c_str(), spelling.size());
                if (!(index < m_numWord)) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_NO_FOUND;
                        return std::string();
                }

                int offset_size[2];
                m_dataFile.seekg(m_offsetBlockHeader + index * sizeof(int) * 2, std::ios::beg);
                m_dataFile.read((char*)offset_size, sizeof(int) * 2);
                m_dataFile.seekg(m_offsetBlock + offset_size[0], std::ios::beg);



                char* data = new char[offset_size[1] + 1];
                m_dataFile.read(data, offset_size[1] + 1);
                data[offset_size[1]] = '\0';
                char* test = new char[spelling.size() + 1]();
                strncpy_s(test, spelling.size() + 1, data + 2, spelling.size());//每一份数据里的第0字节为'#',第1字节为' '.
                //test[word.size()] = '\0';

                if (!(test == spelling)) {
                        delete[] data;
                        delete[] test;
                        m_latestError = ErrorEnum::WORD_LIBRARY_NO_FOUND;
                        return std::string();
                }
                std::string result(data);//转为string
                delete[] test;
                delete[] data;
                return result;
        }


        inline Word::type WordLibrary::INTERNAL_GET_TYPE(const std::string & trans)
        {
                switch (trans[2])
                {
                case 'n': {
                        if (trans[3] == '.')
                                return(Word::type_n);
                        else if (trans[3] == 'u'&&trans[4] == 'm')
                                return(Word::type_num);
                        else
                                return(Word::type_none);
                        break;
                }
                case 'v': {
                        if (trans[3] == '.')
                                return(Word::type_v);
                        else if (trans[3] == 'i')
                                return(Word::type_vi);
                        else if (trans[3] == 't')
                                return(Word::type_vt);
                        else
                                return(Word::type_none);
                        break;
                }
                case 'a': {
                        if (trans[3] == 'd'&&trans[4] == 'j')
                                return(Word::type_adj);
                        else if (trans[3] == 'd'&&trans[4] == 'v')
                                return(Word::type_adv);
                        else if (trans[3] == 'r'&&trans[4] == 't')
                                return(Word::type_art);
                        else if (trans[3] == 'b'&&trans[4] == 'b'&&trans[5] == 'r')
                                return(Word::type_abbr);
                        else
                                return(Word::type_none);
                        break;
                }
                case 'c': {
                        return(Word::type_conj);
                        break;
                }
                case 'p': {
                        if (trans[3] == 'r'&&trans[4] == 'e'&&trans[5] == 'p')
                                return(Word::type_prep);
                        else if (trans[3] == 'r'&&trans[4] == 'o'&&trans[5] == 'n')
                                return(Word::type_pron);
                        else
                                return(Word::type_none);
                        break;
                }
                case 'i': {
                        return(Word::type_int);
                        break;
                default:
                        return(Word::type_none);
                }

                }
        }



        inline Word WordLibrary::INTERNAL_INTERPRET(const std::string & str) {
                Word word;
                StringArray&& lines = INTERNAL_TO_LINE(str);
                for (int i = 0; i < lines.size(); i++)
                {
                        if (lines[i][0] == '#') {
                                word.m_spellingEn = lines[i].substr(2, lines[i].size() - 2);
                        }
                        else if (lines[i][0] == 'T') {
                                word.m_types.push_back(INTERNAL_GET_TYPE(lines[i]));
                                word.m_translationCn.emplace_back(lines[i].substr(2, lines[i].size() - 2));
                        }
                        else if (lines[i][0] == 'E') {
                                if (i + 1 < lines.size() && lines[i + 1][0] == 'C') {
                                        word.m_sentenceEn.emplace_back(lines[i].substr(2, lines[i].size() - 2));
                                        word.m_sentenceCn.emplace_back(lines[i + 1].substr(2, lines[i + 1].size() - 2));
                                        i++;
                                }
                        }
                        else if (lines[i][0] == 'P') {
                                if (i + 1 < lines.size() && lines[i + 1][0] == 'P') {
                                        word.m_pronunciationEnStyle = lines[i].substr(2, lines[i].size() - 2);
                                        word.m_pronunciationUsStyle = lines[i + 1].substr(2, lines[i + 1].size() - 2);
                                        i++;
                                }
                        }
                }
                return word;
        }

        inline char* WordLibrary::INTERNAL_UTF8_2_LOC(const char * utf8) {
                int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
                wchar_t* wstr = new wchar_t[len + 1];
                memset(wstr, 0, len + 1);
                MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
                len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
                char* str = new char[len + 1];
                memset(str, 0, len + 1);
                WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
                if (wstr) delete[] wstr;
                return str;
        }



        inline WordLibrary::~WordLibrary()
        {
                clear();
        }

        inline WordLibrary::WordLibrary()
        {
                m_numWord = 0;
                m_numHashWeight = 0;
                m_offsetBlockHeader = 0;
                m_offsetBlock = 0;
                m_serHashData = nullptr;
                m_hashComp.m_numBucket = 0;
                m_hashComp.m_weight0 = nullptr;
                m_hashComp.m_weight1 = nullptr;
                m_hashComp.m_gBucket = nullptr;
                m_isLinked = false;
        }


        inline bool WordLibrary::link(const char* path)
        {
                if (is_linked())
                        clear();

                m_dataFile.open(path, std::ios::binary);
                if (!m_dataFile.is_open()) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_FAILED_TO_OPEN_FILE;
                        return false;
                }

                int fheader[16];
                m_dataFile.read((char*)fheader, 64);

                //
                // 校验文件
                //

                m_path = path;

                m_numWord = fheader[5];                //单词数量
                m_numHashWeight = fheader[6];          //最大单词长度
                m_hashComp.m_numBucket = fheader[7];   //哈希函数分散数量
                int offsetSerializeData = fheader[8];  //哈希函数序列偏移值
                m_offsetBlockHeader = fheader[9];      //数据块标头在文件中偏移值
                m_offsetBlock = fheader[10];           //数据块在文件中偏移值
                char* serializedData = new char[fheader[11]];//哈希函数序列化数据
                m_dataFile.read(serializedData, fheader[11]);

                m_hashComp.m_weight0 = ((int*)serializedData);
                m_hashComp.m_weight1 = ((int*)serializedData) + m_numHashWeight;
                m_hashComp.m_gBucket = ((int*)serializedData) + m_numHashWeight * 2;


                m_isLinked = true;

                return true;
        }


        inline bool WordLibrary::link(const std::string& path)
        {
                if (is_linked())
                        clear();

                m_dataFile.open(path, std::ios::binary);
                if (!m_dataFile.is_open()) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_FAILED_TO_OPEN_FILE;
                        return false;
                }

                int fheader[16];
                m_dataFile.read((char*)fheader, 64);

                //
                // 校验文件
                //

                m_path = path;

                m_numWord = fheader[5];      //单词数量
                m_numHashWeight = fheader[6];//最大单词长度
                m_hashComp.m_numBucket = fheader[7]; //哈希函数分散数量
                int offsetSerializeData = fheader[8];//哈希函数序列偏移值
                m_offsetBlockHeader = fheader[9];//数据块标头在文件中偏移值
                m_offsetBlock = fheader[10];     //数据块在文件中偏移值
                char* serializedData = new char[fheader[11]];//哈希函数序列化数据
                m_dataFile.read(serializedData, fheader[11]);

                m_hashComp.m_weight0 = ((int*)serializedData);
                m_hashComp.m_weight1 = ((int*)serializedData) + m_numHashWeight;
                m_hashComp.m_gBucket = ((int*)serializedData) + m_numHashWeight * 2;

                m_isLinked = true;
                return true;
        }


        inline Word WordLibrary::read(const char * word)
        {
                std::string&& data_stream = INTERNAL_GET_STREAM_LOC(word);
                if (data_stream.size() > 0)
                        return INTERNAL_INTERPRET(data_stream);
                else
                        return Word();
        }


        inline Word WordLibrary::read(const std::string& word)
        {
                std::string&& data_stream = INTERNAL_GET_STREAM_LOC(word);
                if (data_stream.size() > 0)
                        return INTERNAL_INTERPRET(data_stream);
                else
                        return Word();
        }

        inline Word WordLibrary::read(const unsigned int index)
        {
                if (!m_isLinked) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_NO_LIBRARY_LINKED;
                        return Word();
                }

                if (!m_dataFile.is_open()) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_FAILED_TO_OPEN_FILE;
                        return Word();
                }

                if (!(index < m_numWord)) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_NO_FOUND;
                        return Word();
                }

                int offset_size[2];
                m_dataFile.seekg(m_offsetBlockHeader + index * sizeof(int) * 2, std::ios::beg);
                m_dataFile.read((char*)offset_size, sizeof(int) * 2);
                m_dataFile.seekg(m_offsetBlock + offset_size[0], std::ios::beg);

#ifdef _DEBUG
                //std::cout << "spelling:" << word << std::endl;
                std::cout << "index   :" << index << std::endl;
                std::cout << "offset  :" << offset_size[0] << std::endl;
                std::cout << "size    :" << offset_size[1] << std::endl;
#endif // DEBUG

                char* data = new char[offset_size[1] + 1];
                m_dataFile.read(data, offset_size[1] + 1);
                data[offset_size[1]] = '\0';

                auto data_GB = INTERNAL_UTF8_2_LOC(data);
                std::string result(data_GB);//转为string
                delete data_GB;

                delete[] data;
                return INTERNAL_INTERPRET(result);
        }

        inline Word WordLibrary::read_utf8(const std::string & word)
        {
                std::string&& data_stream = INTERNAL_GET_STREAM_UTF8(word);
                if (data_stream.size() > 0)
                        return INTERNAL_INTERPRET(data_stream);
                else
                        return Word();
        }

        inline Word WordLibrary::read_utf8(const unsigned int index)
        {
                if (!m_isLinked) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_NO_LIBRARY_LINKED;
                        return Word();
                }
                m_dataFile.seekg(0, std::ios::beg);
                if (!m_dataFile.is_open()) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_FAILED_TO_OPEN_FILE;
                        return Word();
                }

                if (!(index < m_numWord)) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_NO_FOUND;
                        return Word();
                }

                int offset_size[2];
                m_dataFile.seekg(m_offsetBlockHeader + index * sizeof(int) * 2, std::ios::beg);
                m_dataFile.read((char*)offset_size, sizeof(int) * 2);
                m_dataFile.seekg(m_offsetBlock + offset_size[0], std::ios::beg);

#ifdef _DEBUG
                //std::cout << "spelling:" << word << std::endl;
                std::cout << "index   :" << index << std::endl;
                std::cout << "offset  :" << offset_size[0] << std::endl;
                std::cout << "size    :" << offset_size[1] << std::endl;
#endif // DEBUG

                char* data = new char[offset_size[1] + 1];
                m_dataFile.read(data, offset_size[1] + 1);
                data[offset_size[1]] = '\0';
                std::string result(data);//转为string

                delete[] data;
                return INTERNAL_INTERPRET(result);
        }

        inline std::string WordLibrary::read_spelling_at(const unsigned int index)
        {

                if (!m_isLinked) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_NO_LIBRARY_LINKED;
                        return std::string();
                }

                if (!m_dataFile.is_open()) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_FAILED_TO_OPEN_FILE;
                        return std::string();
                }

                if (!(index < m_numWord)) {
                        m_latestError = ErrorEnum::WORD_LIBRARY_NO_FOUND;
                        return std::string();
                }

                int offset;
                m_dataFile.seekg(m_offsetBlockHeader + index * sizeof(int) * 2, std::ios::beg);
                m_dataFile.read((char*)&offset, sizeof(int));
                m_dataFile.seekg(m_offsetBlock + offset + 2, std::ios::beg);

                char* data = new char[this->m_numHashWeight + 2];
                m_dataFile.read(data, this->m_numHashWeight + 1);
                data[this->m_numHashWeight + 1] = '\0';
                int start = 0;
                int end = this->m_numHashWeight + 1;
                for (int i = 0; i < this->m_numHashWeight + 1; i++)
                {
                        if (isalpha(data[i]))
                        {
                                start = i;
                                break;
                        }
                }
                for (int i = start; i < this->m_numHashWeight + 1; i++)
                {
                        if (!isalpha(data[i]))
                        {
                                end = i;
                                break;
                        }
                }

                int subdata_size = end - start;
                if (subdata_size <= 0)
                {
                        delete[] data;
                        return std::string();
                }

                char* subdata = new char[subdata_size + 1];
                strncpy_s(subdata, subdata_size + 1, data + start, subdata_size);
                std::string result(subdata);

                delete[] data;
                delete[] subdata;

                return result;
        }

        inline bool WordLibrary::word_is_inside(const char * spelling, unsigned len)
        {
                unsigned index = m_hashComp.hash(spelling, len);
                if (!(index < m_numWord))
                        return false;
                std::string spe = read_spelling_at(index);
                if (!(len == spe.size()))
                        return false;
                for (int i = 0; i < len; i++)
                {
                        if (!(spelling[i] == spe[i]))
                        {
                                return false;
                        }
                }
                return true;
        }


        inline int WordLibrary::fuzzy_query(const std::string & text)
        {
                int sub_string_max_len = 0;
                // 最长子串匹配
                for (int i = m_numHashWeight > text.size() ? text.size() : m_numHashWeight; i > 0; i--)
                {
                        if (word_is_inside(text.c_str(), i))
                        {
                                sub_string_max_len = i;
                                break;
                        }
                }
                if (sub_string_max_len == 0)
                {
                        // 模糊搜索

                        // TODO

                }
                else
                {
                        // 返回最小串索引
                        return m_hashComp.hash(text.c_str(), sub_string_max_len);
                }
                return -1;
        }


        inline void WordLibrary::clear()
        {
                m_path.clear();
                m_dataFile.close();
                m_numWord = 0;
                m_numHashWeight = 0;
                m_offsetBlockHeader = 0;
                m_offsetBlock = 0;
                if (m_serHashData)
                        delete m_serHashData;
                m_hashComp.m_numBucket = 0;
                m_hashComp.m_weight0 = nullptr;
                m_hashComp.m_weight1 = nullptr;
                m_hashComp.m_gBucket = nullptr;
                m_isLinked = false;
        }



}

#endif // !WORD_LIBRARY_H_
