//---------------data£º2014/1/17------------------//
//---------------author:syl---------------------//
//---------------email:jxjgssylsg@126.com ,jxjgssylsg@gmail.com ----------//

// This source code is provided 'as-is', without any express or implied
//	warranty. In no event will the author be held liable for any damages
//	arising from the use of this software.
#ifndef XXTEA_H 
#define XXTEA_H 
//#include "stdafx.h"
#include <stddef.h> /* for size_t & NULL declarations */ 
#include <string>
namespace Xxtea
{
	//#include "WinNls.h"
	std::string base64_encode(unsigned char const* , unsigned int len);
	std::string base64_decode(std::string const& s);
	//////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER) 
	typedef unsigned __int32 xxtea_uint; 
#else 
#if defined(__FreeBSD__) && __FreeBSD__ < 5 
#include <inttypes.h> 
#else 
#include <stdint.h> 
#endif 
	typedef uint32_t xxtea_uint; 
#endif 
#define XXTEA_MX (z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (k[p & 3 ^ e] ^ z) 
#define XXTEA_DELTA 0x9E3779B9 
	unsigned char *xxtea_encrypt(const unsigned char *data, xxtea_uint data_len, const unsigned char *key, xxtea_uint key_len, xxtea_uint *ret_length);
	unsigned char *xxtea_decrypt(const unsigned char *data, xxtea_uint data_len, const unsigned char *key, xxtea_uint key_len, xxtea_uint *ret_length);
	std::string xxtea_encrypt(std::string data,  std::string key); 
	std::string xxtea_decrypt(std::string data,  std::string key); 
#endif 
};
