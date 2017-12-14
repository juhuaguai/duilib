//¸Ä±à×ÔÎ¢ÈíATLTIME

#pragma once
#include <time.h> 
#include <string>

using std::string;

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
	//bool GetAsDBTIMESTAMP( DBTIMESTAMP& dbts ) const throw();

	__time64_t GetTime() const throw();

	int GetYear() const throw();
	int GetMonth() const throw();
	int GetDay() const throw();
	int GetHour() const throw();
	int GetMinute() const throw();
	int GetSecond() const throw();
	int GetDayOfWeek() const throw();

	inline void FormatTime(char *szTime) const
	{
		sprintf_s(szTime, 20, "%04d-%02d-%02d %02d:%02d:%02d", GetYear(),GetMonth(),GetDay(), GetHour(),GetMinute(),GetSecond());
		szTime[19] = '\0';
	};

	inline void FormatTime(const char *pszFormat, char *pszTime) const
	{
		sprintf(pszTime, pszFormat, GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond());
	};

private:
	__time64_t m_time;
};

inline CMSTime ParseTimeStr(const char *pszTime) throw()//str2Time
{
	int nYear, nMonth, nDay, nHour, nMinute, nSecond;

	sscanf_s(pszTime, "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSecond);
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

	sscanf_s(pszTime, "%4d%02d%02d %d:%d:%d", &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSecond);
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

inline CMSTimeSpan ParseOnlyTimeStr(const char *pszTime) throw()//str2Time
{
	int nHour, nMinute, nSecond;

	const char *pszFind = strchr(pszTime, ':');
	if (pszFind != NULL)
	{
		sscanf(pszTime, "%d:%d:%d", &nHour, &nMinute, &nSecond);
	}
	else
	{
		sscanf(pszTime, "%02d%02d%02d", &nHour, &nMinute, &nSecond);
	}

	if (nHour < 0 || nHour > 23)
	{
		nHour = 0;
	}
	if (nMinute < 0 || nMinute > 59)
	{
		nMinute = 0;
	}
	if (nSecond < 0 || nSecond > 59)
	{
		nSecond = 0;
	}
	return CMSTimeSpan(0, nHour, nMinute, nSecond);
	/*int nTime = nHour * 10000 + nMinute * 100 + nSecond;
	return nTime;*/
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

#define MAX_TIME_LEN	64	
inline void GetPeriodTimeOfWeek(string &strStartTime)
{
	CMSTime tCurTime = CMSTime::GetCurrentTime();
	int nWeekDay = tCurTime.GetDayOfWeek();
	CMSTimeSpan tSpan = CMSTimeSpan(nWeekDay-1, 0, 0, 0);
	tCurTime -= tSpan;

	char szTime[MAX_TIME_LEN];
	_snprintf_s(szTime, _countof(szTime), _TRUNCATE, "%04d-%02d-%02d %02d:%02d:%02d", tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), 0, 0, 0);
	szTime[19] = '\0';
	strStartTime = szTime;
}

inline void GetStartTimeOfCurMonth(string &strStartTime)
{
	CMSTime tCurTime = CMSTime::GetCurrentTime();

	char szTime[MAX_TIME_LEN];
	_snprintf_s(szTime, _countof(szTime), _TRUNCATE, "%04d-%02d-%02d %02d:%02d:%02d", tCurTime.GetYear(),tCurTime.GetMonth(), 1, 0, 0, 0);
	szTime[19] = '\0';
	strStartTime = szTime;
}

inline void GetStartTimeOfCurDay(string &strStartTime)
{
	CMSTime tCurTime = CMSTime::GetCurrentTime();

	char szTime[MAX_TIME_LEN];
	_snprintf_s(szTime, _countof(szTime), _TRUNCATE, "%04d-%02d-%02d %02d:%02d:%02d", tCurTime.GetYear(),tCurTime.GetMonth(), tCurTime.GetDay(), 0, 0, 0);
	szTime[19] = '\0';
	strStartTime = szTime;
}

inline int GetDayCountOfYear(int nYear, int nMonth)
{
	switch(nMonth)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	case 2:
		{
			if((((nYear%4)==0)&&(nYear%100)!=0) || (nYear%400==0))
			{
				return 29;
			}
			else
			{
				return 28;
			}
		}
	default:
		break;
	}
	return 0;
}






