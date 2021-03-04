#pragma once
#ifndef WORD_H_
#define WORD_H_

#include<string>
#include<vector>

namespace team 
{
	typedef std::vector<std::string> StringArray;


	class Word {
	private:
		enum type :unsigned;
		typedef std::string Translation;
		typedef std::vector<type> TypeArray;

		friend class WordLibrary;

		std::string m_spellingEn;	    //英文
		std::string m_pronunciationEnStyle;
		std::string m_pronunciationUsStyle;
		StringArray m_translationCn;	//中文
		TypeArray   m_types;
		StringArray m_sentenceEn;       //存储例句英文
		StringArray m_sentenceCn;       //存储例句中文
	public:

		//可能出现的词性
		enum type :unsigned
		{
			type_none,
			type_n,
			type_v,
			type_vi,
			type_vt,
			type_adj,
			type_adv,
			type_num,
			type_conj,
			type_prep,
			type_pron,
			type_art,
			type_int,
			type_abbr,
		};

		const std::string& spelling()const;
		const std::string& pronounciationEnStyle() const;
		const std::string& pronounciationUsStyle() const;
		const Translation& translation(const unsigned& index)const;
		const std::string& sentence_en(const unsigned& index)const;
		const std::string& sentence_cn(const unsigned& index)const;

                const int sizeofTran(){return m_translationCn.size();}
                const int sizeofSent(){return m_sentenceEn.size();}


		// 中文翻译数量
		const unsigned& size_trans()const;
		// 例句数量
		const unsigned& size_sents()const;
		// 是否有数据
		bool is_null()const;


	};



	inline const std::string & Word::spelling() const
	{
		return m_spellingEn;
	}

	inline const std::string & Word::pronounciationEnStyle() const
	{
		return m_pronunciationEnStyle;
	}

	inline const std::string & Word::pronounciationUsStyle() const
	{
		return m_pronunciationUsStyle;
	}

	inline const std::string & Word::translation(const unsigned & index) const
	{
		return m_translationCn[index];
	}

	inline const std::string & Word::sentence_en(const unsigned & index) const
	{
		return m_sentenceEn[index];
	}

	inline const std::string & Word::sentence_cn(const unsigned & index) const
	{
		return m_sentenceCn[index];
	}

	inline const unsigned & Word::size_trans() const
	{
		return m_translationCn.size();
	}

	inline const unsigned & Word::size_sents() const
	{
		return m_sentenceCn.size();
	}

	inline bool Word::is_null() const
	{
		return !(bool)(m_spellingEn.size());
	}





}

#endif // !WORD_H_
