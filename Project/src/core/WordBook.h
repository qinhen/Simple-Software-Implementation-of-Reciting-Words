#pragma once
// WordBook.h
// updated in 2019.7.24 by 衡
//
#ifndef WORD_BOOK_H_
#define WORD_BOOK_H_
#include<unordered_set>
#include<vector>
#include<fstream>
#include<stdlib.h>

namespace team {

	typedef std::vector<unsigned int> IndexArray;

	/*
	class WordBook
	词汇本	
	*/	
	class WordBook {
	public:

		WordBook();
		~WordBook();

		/// <summary> 加载词汇本 </summary>
		bool load(const char* path);                          // 可从标准文件中加载词库索引
		bool load(const IndexArray& index_array);             // 或者传入一个无符号整形数组
		bool load(const unsigned* index_array, unsigned size);// 重复加载则覆盖
		
		///<summary> 返回单词数量 </summary>
		unsigned size() { return m_size; }

		///<summary> 随机采样生成一组单词的索引 </summary>
		///<param name='size'> 要求的索引数量(需保证不大于总数) </param>
		///<returns> 返回索引数组 </returns>
		IndexArray sample(unsigned size);


		///<summary> 断开链接 </summary>
		void clear();

		///<summary> 下标访问 </summary>
		const unsigned& operator[](unsigned i);


		///<summary> 保存到本地文件 </summary>
		void save(const char* path);

	private:
		unsigned* m_words;//词汇本包含的词汇索引
		unsigned m_size;  //词汇本包含的词汇索引数量
		bool m_loaded;    //是否已加载

	private:
		IndexArray INTERNAL_GET_RAND_SERIAL(unsigned req, unsigned total);
	};



	inline WordBook::WordBook()
	{
		m_loaded = false;
	}

	inline WordBook::~WordBook()
	{
		clear();
	}

	inline bool WordBook::load(const char * path)
	{
		/// 32字节文件头
		union FileHeader
		{
			struct
			{
				unsigned flag; //32位文件标志
				unsigned checkCode[4]; //128wei校验码
				unsigned num_Words;
			};
			char DATA_IN_BYTE[32];
		};

		clear();
			
		std::ifstream infile(path, std::ios::binary);
		if (!infile.is_open())
			return false;

		FileHeader fh;
		infile.read((char*)&fh, sizeof(FileHeader));
		m_size = fh.num_Words;
		m_words = new unsigned[m_size];
		infile.read((char*)m_words, sizeof(unsigned) * m_size);
		m_loaded = true;
		return true;
	}

	inline bool WordBook::load(const IndexArray & index_array)
	{
		clear();

		m_size = index_array.size();
		if (m_size > 0) {
			m_words = new unsigned[m_size];
			for (int i = 0; i < m_size; i++)
				m_words[i] = index_array[i];
			m_loaded = true;
			return true;
		}
		return false;
	}

	inline bool WordBook::load(const unsigned * index_array, unsigned size)
	{
		clear();

		m_size = size;
		if (m_size > 0) {
			m_words = new unsigned[m_size];
			for (int i = 0; i < m_size; i++)
				m_words[i] = index_array[i];
			m_loaded = true;
			return true;
		}
		return false;
	}

	inline IndexArray WordBook::sample(unsigned size)
	{
		IndexArray sample;
		if (m_loaded && m_size != 0)
		{
			unsigned req_size = (size < m_size) ? size : m_size;
			auto rand_indices = INTERNAL_GET_RAND_SERIAL(req_size, m_size);
			for (auto i : rand_indices)
				sample.push_back(m_words[i]);	
		}		
		return sample;
	}

	inline void WordBook::clear()
	{
		if (m_loaded) {
			if (m_words)
				delete[] m_words;
			m_words = nullptr;
			m_size = 0;
		}
	}

	inline const unsigned & WordBook::operator[](unsigned i)
	{
		return m_words[i];
	}

	inline void WordBook::save(const char * path)
	{
		/// 32字节文件头
		union FileHeader
		{
			struct
			{
				unsigned flag; //32位文件标志
				unsigned checkCode[4]; //128wei校验码
				unsigned num_Words;
			};
			char DATA_IN_BYTE[32];
		};
		FileHeader fh;
		memset(&fh, 0, sizeof(FileHeader));
		fh.num_Words = m_size;
		std::ofstream f(path, std::ios::binary);
		f.write((char*)&fh, sizeof(FileHeader));
		f.write((char*)m_words, sizeof(unsigned) * m_size);
		f.close();
	}

	inline IndexArray WordBook::INTERNAL_GET_RAND_SERIAL(unsigned req, unsigned total)
	{
		std::unordered_set<unsigned> us;
		unsigned t_req = req;
		bool exclusion_model = false;

		int current_size = 0;
		if (2 * req > total)
		{
			t_req = total - req;
			exclusion_model = true;
		}

		for (int i = 0; i < t_req; i++)
		{
			unsigned rand_value = rand() % total;
			while (current_size == us.size())
			{
				auto i = us.insert(rand_value);
				rand_value++;
				if (rand_value == total)
					rand_value = 0;
			}
			current_size++;
		}

		std::vector<unsigned> res;
		if (exclusion_model) {
			bool* accept_array = new bool[total];
			for (int i = 0; i < total; i++)
				accept_array[i] = true;
			for (auto i : us)
				accept_array[i] = false;
			for (int i = 0; i < total; i++)
				if (accept_array[i])
					res.push_back(i);
		}
		else
		{
			for (auto i : us)
				res.push_back(i);
		}
		return res;
	}

	

}

#endif //WORD_BOOK_H_