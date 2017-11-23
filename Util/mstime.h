//¸Ä±à×ÔÎ¢ÈíATLTIME

#pragma once

//#pragma warning(push)
//#pragma warning(disable : 4159 4127)
//
//#include <atldef.h>
//
#include <time.h>
#include <atltime.h>
//
//#ifdef _AFX
//#include <afxstr.h>
//#else
//#include <atlstr.h>
//#endif
//
//#if defined(_M_IX86)
//#pragma pack(push, 4)
//#else
//#pragma pack(push, ATL_PACKING)
//#endif
//
//#ifndef __oledb_h__
//struct tagDBTIMESTAMP;
//typedef tagDBTIMESTAMP DBTIMESTAMP;
//#endif
 
 

class CMSTimeSpan
{
public:
	CMSTimeSpan() throw();
	CMSTimeSpan( __time64_t time ) throw();
	CMSTimeSpan( LONG lDays, int nHours, int nMins, int nSecs ) throw();

	LONGLONG GetDays() const throw();
	LONGLONG GetTotalHours() const throw();
	LONG GetHours() const throw();
	LONGLONG GetTotalMinutes() const throw();
	LONG GetMinutes() const throw();
	LONGLONG GetTotalSeconds() const throw();
	LONG GetSeconds() const throw();

	__time64_t GetTimeSpan() const throw();

	CMSTimeSpan operator+( CMSTimeSpan span ) const throw();
	CMSTimeSpan operator-( CMSTimeSpan span ) const throw();
	CMSTimeSpan& operator+=( CMSTimeSpan span ) throw();
	CMSTimeSpan& operator-=( CMSTimeSpan span ) throw();
	bool operator==( CMSTimeSpan span ) const throw();
	bool operator!=( CMSTimeSpan span ) const throw();
	bool operator<( CMSTimeSpan span ) const throw();
	bool operator>( CMSTimeSpan span ) const throw();
	bool operator<=( CMSTimeSpan span ) const throw();
	bool operator>=( CMSTimeSpan span ) const throw();

//public:
//	CString Format( LPCTSTR pszFormat ) const;
//	CString Format( UINT nID ) const;
//
//#if defined(_AFX) && defined(_UNICODE)
//	// for compatibility with MFC 3.x
//	CString Format(LPCSTR pFormat) const;
//#endif
//
//#ifdef _AFX
//	CArchive& Serialize64(CArchive& ar);
//#endif

private:
	__time64_t m_timeSpan;
};

class CMSTime
{
public:
	static CMSTime WINAPI GetCurrentTime() throw();
	static BOOL WINAPI IsValidFILETIME(const FILETIME& ft) throw();
	

	CMSTime() throw();
	CMSTime( __time64_t time ) throw();
	CMSTime( int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,int nDST = -1 );
	CMSTime( WORD wDosDate, WORD wDosTime, int nDST = -1 );
	CMSTime( const SYSTEMTIME& st, int nDST = -1 );
	CMSTime( const FILETIME& ft, int nDST = -1 );
//#ifdef __oledb_h__
//	CMSTime( const DBTIMESTAMP& dbts, int nDST = -1 ) throw();
//#endif

	CMSTime& operator=( __time64_t time ) throw();

	CMSTime& operator+=( CMSTimeSpan span ) throw();
	CMSTime& operator-=( CMSTimeSpan span ) throw();

	CMSTimeSpan operator-( CMSTime time ) const throw();
	CMSTime operator-( CMSTimeSpan span ) const throw();
	CMSTime operator+( CMSTimeSpan span ) const throw();

	bool operator==( CMSTime time ) const throw();
	bool operator!=( CMSTime time ) const throw();
	bool operator<( CMSTime time ) const throw();
	bool operator>( CMSTime time ) const throw();
	bool operator<=( CMSTime time ) const throw();
	bool operator>=( CMSTime time ) const throw();

	struct tm* GetGmtTm( struct tm* ptm ) const;
	struct tm* GetLocalTm( struct tm* ptm ) const;

	bool GetAsSystemTime( SYSTEMTIME& st ) const throw();
	bool GetAsDBTIMESTAMP( DBTIMESTAMP& dbts ) const throw();

	__time64_t GetTime() const throw();

	int GetYear() const throw();
	int GetMonth() const throw();
	int GetDay() const throw();
	int GetHour() const throw();
	int GetMinute() const throw();
	int GetSecond() const throw();
	int GetDayOfWeek() const throw();

	inline void TimeStr(TCHAR *szTime)
	{
		//_stprintf_s(szTime, 19, _T("%04d-%02d-%02d %02d:%02d:%02d"), GetYear(),GetMonth(),GetDay(), GetHour(),GetMinute(),GetSecond());
		//szTime[19] = '\0';
		_stprintf(szTime,_T("%04d-%02d-%02d %02d:%02d:%02d"), GetYear(),GetMonth(),GetDay(), GetHour(),GetMinute(),GetSecond());
	};

	

//	// formatting using "C" strftime
//	CString Format( LPCTSTR pszFormat ) const;
//	CString FormatGmt( LPCTSTR pszFormat ) const;
//	CString Format( UINT nFormatID ) const;
//	CString FormatGmt( UINT nFormatID ) const;
//
//#if defined(_AFX) && defined(_UNICODE)
//	// for compatibility with MFC 3.x
//	CString Format(LPCSTR pFormat) const;
//	CString FormatGmt(LPCSTR pFormat) const;
//#endif
//
//#ifdef _AFX
//	CArchive& Serialize64(CArchive& ar);
//#endif

private:
	__time64_t m_time;
};

inline CMSTime str2Time(const char *szTime) throw()
{
	int nYear, nMonth, nDay, nHour, nMinute, nSecond;

    sscanf_s(szTime,"%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSecond);
	if( nHour < 0 || nHour > 23 )
	{
		nHour = 0;
	}
	if( nMinute < 0 || nMinute > 59 )
	{
		nMinute = 0;
	}
	if( nSecond < 0 || nSecond > 59 )
	{
		nSecond = 0;
	}
	if(( nYear >= 1900 ) && ( nMonth >= 1 && nMonth <= 12 ) && ( nDay >= 1 && nDay <= 31 ))// && ( nHour >= 0 && nHour <= 23 ) && ( nMinute >= 0 && nMinute <= 59 ) && ( nSecond >= 0 && nSecond <= 59 )
	{
		return ( CMSTime(nYear, nMonth, nDay, nHour, nMinute, nSecond) );
	}

	sscanf_s(szTime,"%4d%02d%02d %d:%d:%d", &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSecond);
	if( nHour < 0 || nHour > 23 )
	{
		nHour = 0;
	}
	if( nMinute < 0 || nMinute > 59 )
	{
		nMinute = 0;
	}
	if( nSecond < 0 || nSecond > 59 )
	{
		nSecond = 0;
	}
	if(( nYear >= 1900 ) && ( nMonth >= 1 && nMonth <= 12 ) && ( nDay >= 1 && nDay <= 31 ))
	{
		return ( CMSTime(nYear, nMonth, nDay, nHour, nMinute, nSecond) );
	}
	return (CMSTime());	
}






class CMSFileTimeSpan
{
public:
	CMSFileTimeSpan() throw();
	CMSFileTimeSpan( const CMSFileTimeSpan& span ) throw();
	CMSFileTimeSpan( LONGLONG nSpan ) throw();

	CMSFileTimeSpan& operator=( const CMSFileTimeSpan& span ) throw();

	CMSFileTimeSpan& operator+=( CMSFileTimeSpan span ) throw();
	CMSFileTimeSpan& operator-=( CMSFileTimeSpan span ) throw();

	CMSFileTimeSpan operator+( CMSFileTimeSpan span ) const throw();
	CMSFileTimeSpan operator-( CMSFileTimeSpan span ) const throw();

	bool operator==( CMSFileTimeSpan span ) const throw();
	bool operator!=( CMSFileTimeSpan span ) const throw();
	bool operator<( CMSFileTimeSpan span ) const throw();
	bool operator>( CMSFileTimeSpan span ) const throw();
	bool operator<=( CMSFileTimeSpan span ) const throw();
	bool operator>=( CMSFileTimeSpan span ) const throw();

	LONGLONG GetTimeSpan() const throw();
	void SetTimeSpan( LONGLONG nSpan ) throw();

protected:
	LONGLONG m_nSpan;
};

class CMSFileTime :
	public FILETIME
{
public:
	CMSFileTime() throw();
	CMSFileTime( const FILETIME& ft ) throw();
	CMSFileTime( ULONGLONG nTime ) throw();

	static CMSFileTime WINAPI GetCurrentTime() throw();

	CMSFileTime& operator=( const FILETIME& ft ) throw();

	CMSFileTime& operator+=( CMSFileTimeSpan span ) throw();
	CMSFileTime& operator-=( CMSFileTimeSpan span ) throw();

	CMSFileTime operator+( CMSFileTimeSpan span ) const throw();
	CMSFileTime operator-( CMSFileTimeSpan span ) const throw();
	CMSFileTimeSpan operator-( CMSFileTime ft ) const throw();

	bool operator==( CMSFileTime ft ) const throw();
	bool operator!=( CMSFileTime ft ) const throw();
	bool operator<( CMSFileTime ft ) const throw();
	bool operator>( CMSFileTime ft ) const throw();
	bool operator<=( CMSFileTime ft ) const throw();
	bool operator>=( CMSFileTime ft ) const throw();

	ULONGLONG GetTime() const throw();
	void SetTime( ULONGLONG nTime ) throw();

	CMSFileTime UTCToLocal() const throw();
	CMSFileTime LocalToUTC() const throw();

	static const ULONGLONG Millisecond = 10000;
	static const ULONGLONG Second = Millisecond * static_cast<ULONGLONG>(1000);
	static const ULONGLONG Minute = Second * static_cast<ULONGLONG>(60);
	static const ULONGLONG Hour = Minute * static_cast<ULONGLONG>(60);
	static const ULONGLONG Day = Hour * static_cast<ULONGLONG>(24);
	static const ULONGLONG Week = Day * static_cast<ULONGLONG>(7);
};

// Used only if these strings could not be found in resources.
extern __declspec(selectany) const TCHAR * const szInvalidDateTime = _T("Invalid DateTime");
extern __declspec(selectany) const TCHAR * const szInvalidDateTimeSpan = _T("Invalid DateTimeSpan");

const int maxTimeBufferSize = 128;
const long maxDaysInSpan  =	3615897L;




// 
//
//#ifndef _DEBUG
//#define ATLTIME_INLINE inline
//#include <atltime.inl>
//#endif
//
//
// 
//namespace ATL
//{
//
//enum _CMSTimeSpanFORMATSTEP
//{
//	_CTFS_NONE   = 0,	
//	_CTFS_FORMAT = 1,
//	_CTFS_NZ     = 2
//};
//#define _CMSTimeSpanFORMATS 3
//
//inline CString CMSTimeSpan::Format(LPCTSTR pFormat) const
//// formatting timespans is a little trickier than formatting CTimes
////  * we are only interested in relative time formats, ie. it is illegal
////      to format anything dealing with absolute time (i.e. years, months,
////         day of week, day of year, timezones, ...)
////  * the only valid formats:
////      %D - # of days
////      %H - hour in 24 hour format
////      %M - minute (0-59)
////      %S - seconds (0-59)
////      %% - percent sign
////	%#<any_of_mods> - skip leading zeros
//{
//	ATLASSERT( pFormat != NULL );
//	if( pFormat == NULL )
//		AtlThrow( E_INVALIDARG );
//
//	CString strBuffer;
//	CString hmsFormats [_CMSTimeSpanFORMATS] = {_T("%c"),_T("%02ld"),_T("%d")};
//	CString dayFormats [_CMSTimeSpanFORMATS] = {_T("%c"),_T("%I64d"),_T("%I64d")};
//	strBuffer.Preallocate(maxTimeBufferSize);
//	TCHAR ch;
//
//	while ((ch = *pFormat++) != _T('\0'))
//	{
//		enum _CMSTimeSpanFORMATSTEP formatstep = _CTFS_NONE;
//		if(ch == _T('%'))
//		{
//			formatstep = _CTFS_FORMAT;
//			ch = *pFormat++;
//			if(ch == _T('#'))
//			{
//					formatstep = _CTFS_NZ;
//					ch = *pFormat++;
//			}
//		}
//		switch (ch)
//		{
//			case '%':
//				strBuffer += ch;
//				break;
//			case 'D':
//				strBuffer.AppendFormat(dayFormats[formatstep], formatstep ? GetDays()    : ch);
//				break;
//			case 'H':
//				strBuffer.AppendFormat(hmsFormats[formatstep], formatstep ? GetHours()   : ch);
//				break;
//			case 'M':
//				strBuffer.AppendFormat(hmsFormats[formatstep], formatstep ? GetMinutes() : ch);
//				break;
//			case 'S':
//				strBuffer.AppendFormat(hmsFormats[formatstep], formatstep ? GetSeconds() : ch);
//				break;
//			default:
//				if(formatstep)
//				{
//#pragma warning (push)
//#pragma warning (disable: 4127)  // conditional expression constant
//					ATLENSURE(FALSE);      // probably a bad format character
//#pragma warning (pop)
//				}
//				else
//				{
//					strBuffer += ch;
//#ifdef _MBCS
//					if (_istlead(ch))
//					{
//						strBuffer += *pFormat++;
//					}
//#endif
//				}
//				break;
//			}
//	}
//
//	return strBuffer;
//}
//
//inline CString CMSTimeSpan::Format(UINT nFormatID) const
//{
//	CString strFormat;
//	ATLENSURE(strFormat.LoadString(nFormatID));
//	return Format(strFormat);
//}
//
//#if defined(_AFX) && defined(_UNICODE)
//inline CString CMSTimeSpan::Format(LPCSTR pFormat) const
//{
//	return Format(CString(pFormat));
//}
//#endif
//
//#ifdef __oledb_h__
//inline CMSTime::CMSTime( const DBTIMESTAMP& dbts, int nDST ) throw()
//{
//	struct tm atm;
//	atm.tm_sec = dbts.second;
//	atm.tm_min = dbts.minute;
//	atm.tm_hour = dbts.hour;
//	atm.tm_mday = dbts.day;
//	atm.tm_mon = dbts.month - 1;        // tm_mon is 0 based
//	ATLASSERT(dbts.year >= 1900);
//	atm.tm_year = dbts.year - 1900;     // tm_year is 1900 based
//	atm.tm_isdst = nDST;
//	m_time = _mktime64(&atm);
//	ATLASSUME(m_time != -1);       // indicates an illegal input time
//}
//#endif
//
//inline CString CMSTime::Format(LPCTSTR pFormat) const
//{
//	if(pFormat == NULL)
//	{
//		return pFormat;
//	}
//
//	TCHAR szBuffer[maxTimeBufferSize];
//
//	struct tm ptmTemp;
//	errno_t err = _localtime64_s(&ptmTemp, &m_time);
//	if (err != 0 || !_tcsftime(szBuffer, maxTimeBufferSize, pFormat, &ptmTemp))
//	{
//		szBuffer[0] = '\0';
//	}
//
//	return szBuffer;
//}
//
//inline CString CMSTime::FormatGmt(LPCTSTR pFormat) const
//{
//	if(pFormat == NULL)
//	{
//		return pFormat;
//	}
//
//	TCHAR szBuffer[maxTimeBufferSize];
//
//	struct tm ptmTemp;
//	errno_t err = _gmtime64_s(&ptmTemp, &m_time);
//	if (err != 0 || !_tcsftime(szBuffer, maxTimeBufferSize, pFormat, &ptmTemp))
//	{
//		szBuffer[0] = '\0';
//	}
//
//	return szBuffer;
//}
//
//inline CString CMSTime::Format(UINT nFormatID) const
//{
//	CString strFormat;
//	ATLENSURE(strFormat.LoadString(nFormatID));
//	return Format(strFormat);
//}
//
//inline CString CMSTime::FormatGmt(UINT nFormatID) const
//{
//	CString strFormat;
//	ATLENSURE(strFormat.LoadString(nFormatID));
//	return FormatGmt(strFormat);
//}
//
//#if defined (_AFX) && defined(_UNICODE)
//inline CString CMSTime::Format(LPCSTR pFormat) const
//{
//	return Format(CString(pFormat));
//}
//
//inline CString CMSTime::FormatGmt(LPCSTR pFormat) const
//{
//	return FormatGmt(CString(pFormat));
//}
//#endif // _AFX && _UNICODE
//
//}	// namespace ATL
//#pragma pack(pop)
//
//#pragma warning(pop)
//
//#endif  // __ATLTIME_H__
