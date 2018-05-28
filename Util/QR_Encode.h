// Date 2006/05/17	Ver. 1.22	Psytec Inc.
#pragma once

/////////////////////////////////////////////////////////////////////////////
#include <Windows.h>

//纠错等级
#define QR_LEVEL_L	0	//7%
#define QR_LEVEL_M	1	//15%
#define QR_LEVEL_Q	2	//25%
#define QR_LEVEL_H	3	//30%

//编码模式
#define QR_MODE_NUMERAL		0	//数字模式
#define QR_MODE_ALPHABET	1	//字母数字模式
#define QR_MODE_8BIT		2	//8bit数据模式
#define QR_MODE_KANJI		3	//汉字模式(汉字日文韩文)

//版本级别数字
#define QR_VRESION_S	0 // 1~9
#define QR_VRESION_M	1 // 10 ~ 26
#define QR_VRESION_L	2 // 27 ~ 40

#define MAX_ALLCODEWORD	 3706 // 数据容量//総コードワード数最大値
#define MAX_DATACODEWORD 2956 // 数据编码字最大值(最大信息码子)//データコードワード最大値(バージョン40-L)
#define MAX_CODEBLOCK	  153 // 块数据编码字数最大值(最大纠错码字)//ブロックデータコードワード数最大値(ＲＳコードワードを含む)
#define MAX_MODULESIZE	  177 // 一边模块数最大值(每边的模块数)//一辺モジュール数最大値

//位图绘制时(每个版本增加的行列数)//ビットマップ描画時マージン
#define QR_MARGIN	4


/////////////////////////////////////////////////////////////////////////////
//QR码符号各版本的纠错特效
typedef struct tagRS_BLOCKINFO
{
	int ncRSBlock;		// 纠错的块数
	int ncAllCodeWord;	// 码字总数
	int ncDataCodeWord;	// 指定纠错等级下的数据码字数

} RS_BLOCKINFO, *LPRS_BLOCKINFO;


/////////////////////////////////////////////////////////////////////////////
// QR码版本（型号）相关信息
typedef struct tagQR_VERSIONINFO
{
	int nVersionNo;	   // Ver  1~40
	int ncAllCodeWord; // 码字总数=数据码字+纠错码字

	// 指定纠错等级下的数据码字 (0 = L, 1 = M, 2 = Q, 3 = H)
	int ncDataCodeWord[4];	//数据编码字数（总代码字数- RS代码字数）

	int ncAlignPoint;	// 校正图形个数
	int nAlignPoint[6];	// 校正图形行列坐标

	RS_BLOCKINFO RS_BlockInfo1[4]; // 纠错块1
	RS_BLOCKINFO RS_BlockInfo2[4]; // 纠错块2 

} QR_VERSIONINFO, *LPQR_VERSIONINFO;


/////////////////////////////////////////////////////////////////////////////
class CQR_Encode
{
public:
	CQR_Encode();
	~CQR_Encode();

public:
	int m_nLevel;		// 纠错级别 0~3
	int m_nVersion;		// 版本==0,版本自动   版本 1~40
	BOOL m_bAutoExtent;	// 版本（型号）自动扩展指定标志//バージョン(型番)自動拡張指定フラグ
	int m_nMaskingNo;	// 掩膜 =-1：需要进行 掩模图形评估 0~7：直接指定

public:
	int m_nSymbleSize;	//指定版本的模块数
	BYTE m_byModuleData[MAX_MODULESIZE][MAX_MODULESIZE]; // [x][y]
	// bit 5：功能模块（除了对象外）标志//bit5:機能モジュール（マスキング対象外）フラグ
	// bit 4：功能模块绘制数据//bit4:機能モジュール描画データ
	// bit 1：编码数据//bit1:エンコードデータ
	// bit 0：面罩后编码绘制数据//bit0:マスク後エンコード描画データ
	// 20 h的逻辑和更功能模块判定，11 h的逻辑和更绘图（最终BOOL值化）//20hとの論理和により機能モジュール判定、11hとの論理和により描画（最終的にはBOOL値化）

private:
	int m_ncDataCodeWordBit; // 实际信息编码后的位数（仅信息），可计算出填充率=CHY_EncodeData_Len_Before/总码字 //数据编码字位长
	BYTE m_byDataCodeWord[MAX_DATACODEWORD]; //信息码字Array  信息+填充码 0b11101100 + 0b00010001// 输入数据编码区域

	int m_ncDataBlock;
	BYTE m_byBlockMode[MAX_DATACODEWORD];
	int m_nBlockLength[MAX_DATACODEWORD];

	int m_ncAllCodeWord; // 总码字Len
	BYTE m_byAllCodeWord[MAX_ALLCODEWORD]; // 总码字Array
	BYTE m_byRSWork[MAX_CODEBLOCK]; // 纠错码字Array

public:
	BOOL EncodeData(int nLevel, int nVersion, BOOL bAutoExtent, int nMaskingNo, LPCSTR lpsSource, int ncSource = 0,LPCWSTR lpDestFile = NULL);

private:
	int GetEncodeVersion(int nVersion, LPCSTR lpsSource, int ncLength);
	BOOL EncodeSourceData(LPCSTR lpsSource, int ncLength, int nVerGroup);

	int GetBitLength(BYTE nMode, int ncData, int nVerGroup);

	int SetBitStream(int nIndex, WORD wData, int ncData);

	BOOL IsNumeralData(unsigned char c);
	BOOL IsAlphabetData(unsigned char c);
	BOOL IsKanjiData(unsigned char c1, unsigned char c2);

	BYTE AlphabetToBinaly(unsigned char c);
	WORD KanjiToBinaly(WORD wc);

	void GetRSCodeWord(LPBYTE lpbyRSWork, int ncDataCodeWord, int ncRSCodeWord);

	BOOL SaveImgFile(LPCWSTR lpDestFile);
private:
	void FormatModule();

	void SetFunctionModule();
	void SetFinderPattern(int x, int y);
	void SetAlignmentPattern(int x, int y);
	void SetVersionPattern();
	void SetCodeWordPattern();
	void SetMaskingPattern(int nPatternNo);
	void SetFormatInfoPattern(int nPatternNo);
	int CountPenalty();
};
