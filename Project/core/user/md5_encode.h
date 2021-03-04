/*
*******************************************************
* brief: md5 encryption
* author: Monkey.Knight
*******************************************************
*/

#ifndef _MD5_ENCODE_H_
#define _MD5_ENCODE_H_

// std
#include <string>

// define
#define MD5_ENCODER_UINT2 unsigned int
#define MD5_ENCODER_BIT_OF_BYTE  8
#define MD5_ENCODER_BIT_OF_GROUP 512
#define MD5_ENCODER_SRC_DATA_LEN 64

// 四个非线性函数宏定义
#define MD5_ENCODER_DEF_F(X, Y, Z ) ((( (X) & (Y) )|((~X)&(Z))))
#define MD5_ENCODER_DEF_G(X, Y, Z)  (((X)&(Z))|((Y)&(~Z)))
#define MD5_ENCODER_DEF_H(X, Y, Z)  ((X)^(Y)^(Z))
#define MD5_ENCODER_DEF_I(X, Y, Z)  ((Y)^((X)|(~Z)))

// 求链接数函数宏定义
#define MD5_ENCODER_FF(a, b, c, d, Mj, s, ti)  (a = b + CycleMoveLeft((a + MD5_ENCODER_DEF_F(b,c,d) + Mj + ti),s));
#define MD5_ENCODER_GG(a, b, c, d, Mj, s, ti)  (a = b + CycleMoveLeft((a + MD5_ENCODER_DEF_G(b,c,d) + Mj + ti),s));
#define MD5_ENCODER_HH(a, b, c, d, Mj, s, ti)  (a = b + CycleMoveLeft((a + MD5_ENCODER_DEF_H(b,c,d) + Mj + ti),s));
#define MD5_ENCODER_II(a, b, c, d, Mj, s, ti)  (a = b + CycleMoveLeft((a + MD5_ENCODER_DEF_I(b,c,d) + Mj + ti),s));


class Md5Encode {
public:
	// 4轮循环算法
	struct ParamDynamic {
		MD5_ENCODER_UINT2 ua_;
		MD5_ENCODER_UINT2 ub_;
		MD5_ENCODER_UINT2 uc_;
		MD5_ENCODER_UINT2 ud_;
		MD5_ENCODER_UINT2 va_last_;
		MD5_ENCODER_UINT2 vb_last_;
		MD5_ENCODER_UINT2 vc_last_;
		MD5_ENCODER_UINT2 vd_last_;
	};
public:
	Md5Encode() {
	}
	std::string Encode(std::string src_info);

protected:

	MD5_ENCODER_UINT2 CycleMoveLeft(MD5_ENCODER_UINT2 src_num, int bit_num_to_move);
	MD5_ENCODER_UINT2 FillData(const char *in_data_ptr, int data_byte_len, char** out_data_ptr);
	void RoundF(char *data_512_ptr, ParamDynamic & param);
	void RoundG(char *data_512_ptr, ParamDynamic & param);
	void RoundH(char *data_512_ptr, ParamDynamic & param);
	void RoundI(char *data_512_ptr, ParamDynamic & param);
	void RotationCalculate(char *data_512_ptr, ParamDynamic & param);
	std::string GetHexStr(unsigned int num_str);

private:

};



// function: CycleMoveLeft
// @param src_num:要左移的数
// @param bit_num_to_move:要移动的bit位数
// @return  循环左移后的结果数
inline MD5_ENCODER_UINT2 Md5Encode::CycleMoveLeft(MD5_ENCODER_UINT2 src_num, int bit_num_to_move) {
	MD5_ENCODER_UINT2 src_num1 = src_num;
	MD5_ENCODER_UINT2 src_num2 = src_num;
	if (0 >= bit_num_to_move) {
		return src_num;
	}
	MD5_ENCODER_UINT2 num1 = src_num1 << bit_num_to_move;
	MD5_ENCODER_UINT2 num2 = src_num2 >> (32 - bit_num_to_move);

	return ((src_num1 << bit_num_to_move) \
		| (src_num2 >> (32 - bit_num_to_move)));
}

// function: FillData
// @param in_data_ptr:    要加密的信息数据
// @param data_byte_len: 数据的字节数
// @param out_data_ptr:  填充必要信息后的数据
// return : 填充信息后的数据长度,以字节为单位
inline MD5_ENCODER_UINT2 Md5Encode::FillData(const char *in_data_ptr, int data_byte_len, char** out_data_ptr) {
	int bit_num = data_byte_len * MD5_ENCODER_BIT_OF_BYTE;
	int grop_num = bit_num / MD5_ENCODER_BIT_OF_GROUP;
	int mod_bit_num = bit_num % MD5_ENCODER_BIT_OF_GROUP;
	int bit_need_fill = 0;
	if (mod_bit_num > (MD5_ENCODER_BIT_OF_GROUP - MD5_ENCODER_SRC_DATA_LEN)) {
		bit_need_fill = (MD5_ENCODER_BIT_OF_GROUP - mod_bit_num);
		bit_need_fill += (MD5_ENCODER_BIT_OF_GROUP - MD5_ENCODER_SRC_DATA_LEN);
	}
	else {
		bit_need_fill = (MD5_ENCODER_BIT_OF_GROUP - MD5_ENCODER_SRC_DATA_LEN) - mod_bit_num; //  这里多加了一个MD5_ENCODER_BIT_OF_GROUP，避免bit_need_fill正好等于0,暂时不加
	}
	int all_bit = bit_num + bit_need_fill;
	if (0 < bit_need_fill) {
		*out_data_ptr = new char[all_bit / MD5_ENCODER_BIT_OF_BYTE + MD5_ENCODER_SRC_DATA_LEN / MD5_ENCODER_BIT_OF_BYTE];
		memset(*out_data_ptr, 0, all_bit / MD5_ENCODER_BIT_OF_BYTE + MD5_ENCODER_SRC_DATA_LEN / MD5_ENCODER_BIT_OF_BYTE);
		// copy data
		memcpy(*out_data_ptr, in_data_ptr, data_byte_len);
		// fill rest data
		unsigned char *tmp = reinterpret_cast<unsigned char *>(*out_data_ptr);
		tmp += data_byte_len;
		// fill 1 and 0
		*tmp = 0x80;
		// fill origin data len
		unsigned long long * origin_num = (unsigned long long *)((*out_data_ptr) + ((all_bit / MD5_ENCODER_BIT_OF_BYTE)));
		*origin_num = data_byte_len * MD5_ENCODER_BIT_OF_BYTE;
	}
	return (all_bit / MD5_ENCODER_BIT_OF_BYTE + MD5_ENCODER_SRC_DATA_LEN / MD5_ENCODER_BIT_OF_BYTE);
}

inline void Md5Encode::RoundF(char *data_BIT_OF_GROUP_ptr, ParamDynamic & param) {
	MD5_ENCODER_UINT2 *M = reinterpret_cast<MD5_ENCODER_UINT2*>(data_BIT_OF_GROUP_ptr);
	int s[] = { 7, 12, 17, 22 };
	for (int i = 0; i < 16; ++i) {
		MD5_ENCODER_UINT2 ti = 4294967296 * abs(sin(i + 1));
		if (i % 4 == 0) {
			MD5_ENCODER_FF(param.ua_, param.ub_, param.uc_, param.ud_, M[i], s[i % 4], ti);
		}
		else if (i % 4 == 1) {
			MD5_ENCODER_FF(param.ud_, param.ua_, param.ub_, param.uc_, M[i], s[i % 4], ti);
		}
		else if (i % 4 == 2) {
			MD5_ENCODER_FF(param.uc_, param.ud_, param.ua_, param.ub_, M[i], s[i % 4], ti);
		}
		else if (i % 4 == 3) {
			MD5_ENCODER_FF(param.ub_, param.uc_, param.ud_, param.ua_, M[i], s[i % 4], ti);
		}
	}
}

inline void Md5Encode::RoundG(char *data_BIT_OF_GROUP_ptr, ParamDynamic & param) {
	MD5_ENCODER_UINT2 *M = reinterpret_cast<MD5_ENCODER_UINT2*>(data_BIT_OF_GROUP_ptr);
	int s[] = { 5, 9, 14, 20 };
	for (int i = 0; i < 16; ++i) {
		MD5_ENCODER_UINT2 ti = 4294967296 * abs(sin(i + 1 + 16));
		int index = (i * 5 + 1) % 16;
		if (i % 4 == 0) {
			MD5_ENCODER_GG(param.ua_, param.ub_, param.uc_, param.ud_, M[index], s[i % 4], ti);
		}
		else if (i % 4 == 1) {
			MD5_ENCODER_GG(param.ud_, param.ua_, param.ub_, param.uc_, M[index], s[i % 4], ti);
		}
		else if (i % 4 == 2) {
			MD5_ENCODER_GG(param.uc_, param.ud_, param.ua_, param.ub_, M[index], s[i % 4], ti);
		}
		else if (i % 4 == 3) {
			MD5_ENCODER_GG(param.ub_, param.uc_, param.ud_, param.ua_, M[index], s[i % 4], ti);
		}
	}
}

inline void Md5Encode::RoundH(char *data_BIT_OF_GROUP_ptr, ParamDynamic & param) {
	MD5_ENCODER_UINT2 *M = reinterpret_cast<MD5_ENCODER_UINT2*>(data_BIT_OF_GROUP_ptr);
	int s[] = { 4, 11, 16, 23 };
	for (int i = 0; i < 16; ++i) {
		MD5_ENCODER_UINT2 ti = 4294967296 * abs(sin(i + 1 + 32));
		int index = (i * 3 + 5) % 16;
		if (i % 4 == 0) {
			MD5_ENCODER_HH(param.ua_, param.ub_, param.uc_, param.ud_, M[index], s[i % 4], ti);
		}
		else if (i % 4 == 1) {
			MD5_ENCODER_HH(param.ud_, param.ua_, param.ub_, param.uc_, M[index], s[i % 4], ti);
		}
		else if (i % 4 == 2) {
			MD5_ENCODER_HH(param.uc_, param.ud_, param.ua_, param.ub_, M[index], s[i % 4], ti);
		}
		else if (i % 4 == 3) {
			MD5_ENCODER_HH(param.ub_, param.uc_, param.ud_, param.ua_, M[index], s[i % 4], ti);
		}
	}
}

inline void Md5Encode::RoundI(char *data_BIT_OF_GROUP_ptr, ParamDynamic & param) {
	MD5_ENCODER_UINT2 *M = reinterpret_cast<MD5_ENCODER_UINT2*>(data_BIT_OF_GROUP_ptr);
	int s[] = { 6, 10, 15, 21 };
	for (int i = 0; i < 16; ++i) {
		MD5_ENCODER_UINT2 ti = 4294967296 * abs(sin(i + 1 + 48));
		int index = (i * 7 + 0) % 16;
		if (i % 4 == 0) {
			MD5_ENCODER_II(param.ua_, param.ub_, param.uc_, param.ud_, M[index], s[i % 4], ti);
		}
		else if (i % 4 == 1) {
			MD5_ENCODER_II(param.ud_, param.ua_, param.ub_, param.uc_, M[index], s[i % 4], ti);
		}
		else if (i % 4 == 2) {
			MD5_ENCODER_II(param.uc_, param.ud_, param.ua_, param.ub_, M[index], s[i % 4], ti);
		}
		else if (i % 4 == 3) {
			MD5_ENCODER_II(param.ub_, param.uc_, param.ud_, param.ua_, M[index], s[i % 4], ti);
		}
	}
}

inline void Md5Encode::RotationCalculate(char *data_512_ptr, ParamDynamic & param) {
	if (NULL == data_512_ptr) {
		return;
	}
	RoundF(data_512_ptr, param);
	RoundG(data_512_ptr, param);
	RoundH(data_512_ptr, param);
	RoundI(data_512_ptr, param);
	param.ua_ = param.va_last_ + param.ua_;
	param.ub_ = param.vb_last_ + param.ub_;
	param.uc_ = param.vc_last_ + param.uc_;
	param.ud_ = param.vd_last_ + param.ud_;

	param.va_last_ = param.ua_;
	param.vb_last_ = param.ub_;
	param.vc_last_ = param.uc_;
	param.vd_last_ = param.ud_;
}

// 转换成十六进制字符串输出
inline std::string Md5Encode::GetHexStr(unsigned int num_str) {
	std::string hexstr = "";
	char szch[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	unsigned char *tmptr = (unsigned char *)&num_str;
	int len = sizeof(num_str);
	// 小端字节序，逆序打印
	for (int i = 0; i < len; i++) {
		unsigned char ch = tmptr[i] & 0xF0;
		ch = ch >> 4;
		hexstr.append(1, szch[ch]);
		ch = tmptr[i] & 0x0F;
		hexstr.append(1, szch[ch]);
	}
	return hexstr;
}

// function: Encode
// @param src_info:要加密的信息
// return :加密后的MD5值
inline std::string Md5Encode::Encode(std::string src_info) {
	ParamDynamic param;
	param.ua_ = 0x67452301;
	param.ub_ = 0xefcdab89;
	param.uc_ = 0x98badcfe;
	param.ud_ = 0x10325476;
	param.va_last_ = 0x67452301;
	param.vb_last_ = 0xefcdab89;
	param.vc_last_ = 0x98badcfe;
	param.vd_last_ = 0x10325476;

	std::string result;
	const char *src_data = src_info.c_str();
	char *out_data_ptr = NULL;
	int total_byte = FillData(src_data, strlen(src_data), &out_data_ptr);
	//char * data_BIT_OF_GROUP = out_data_ptr;
	for (int i = 0; i < total_byte / (MD5_ENCODER_BIT_OF_GROUP / MD5_ENCODER_BIT_OF_BYTE); ++i) {
		char * data_BIT_OF_GROUP = out_data_ptr;
		data_BIT_OF_GROUP += i * (MD5_ENCODER_BIT_OF_GROUP / MD5_ENCODER_BIT_OF_BYTE);
		RotationCalculate(data_BIT_OF_GROUP, param);
	}
	if (NULL != out_data_ptr) {
		delete[] out_data_ptr, out_data_ptr = NULL;
	}
	result.append(GetHexStr(param.ua_));
	result.append(GetHexStr(param.ub_));
	result.append(GetHexStr(param.uc_));
	result.append(GetHexStr(param.ud_));
	return result;
}



#endif