// This is a part of the Active Template Library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Active Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Active Template Library product.
#include <Windows.h>
#include <tchar.h>
#include "mstime.h"
//#include <atldef.h>
#include <assert.h>




/////////////////////////////////////////////////////////////////////////////
// CMSTimeSpan
/////////////////////////////////////////////////////////////////////////////

CMSTimeSpan::CMSTimeSpan() throw() :
	m_timeSpan(0)
{
}

CMSTimeSpan::CMSTimeSpan( __time64_t time ) throw() :
	m_timeSpan( time )
{
}

CMSTimeSpan::CMSTimeSpan(LONG lDays, int nHours, int nMins, int nSecs) throw()
{
 	m_timeSpan = nSecs + 60* (nMins + 60* (nHours + __int64(24) * lDays));
}

LONGLONG CMSTimeSpan::GetDays() const throw()
{
	return( m_timeSpan/(24*3600) );
}

LONGLONG CMSTimeSpan::GetTotalHours() const throw()
{
	return( m_timeSpan/3600 );
}

LONG CMSTimeSpan::GetHours() const throw()
{
	return( LONG( GetTotalHours()-(GetDays()*24) ) );
}

LONGLONG CMSTimeSpan::GetTotalMinutes() const throw()
{
	return( m_timeSpan/60 );
}

LONG CMSTimeSpan::GetMinutes() const throw()
{
	return( LONG( GetTotalMinutes()-(GetTotalHours()*60) ) );
}

LONGLONG CMSTimeSpan::GetTotalSeconds() const throw()
{
	return( m_timeSpan );
}

LONG CMSTimeSpan::GetSeconds() const throw()
{
	return( LONG( GetTotalSeconds()-(GetTotalMinutes()*60) ) );
}

__time64_t CMSTimeSpan::GetTimeSpan() const throw()
{
	return( m_timeSpan );
}

CMSTimeSpan CMSTimeSpan::operator+( CMSTimeSpan span ) const throw()
{
	return( CMSTimeSpan( m_timeSpan+span.m_timeSpan ) );
}

CMSTimeSpan CMSTimeSpan::operator-( CMSTimeSpan span ) const throw()
{
	return( CMSTimeSpan( m_timeSpan-span.m_timeSpan ) );
}

CMSTimeSpan& CMSTimeSpan::operator+=( CMSTimeSpan span ) throw()
{
	m_timeSpan += span.m_timeSpan;
	return( *this );
}

CMSTimeSpan& CMSTimeSpan::operator-=( CMSTimeSpan span ) throw()
{
	m_timeSpan -= span.m_timeSpan;
	return( *this );
}

bool CMSTimeSpan::operator==( CMSTimeSpan span ) const throw()
{
	return( m_timeSpan == span.m_timeSpan );
}

bool CMSTimeSpan::operator!=( CMSTimeSpan span ) const throw()
{
	return( m_timeSpan != span.m_timeSpan );
}

bool CMSTimeSpan::operator<( CMSTimeSpan span ) const throw()
{
	return( m_timeSpan < span.m_timeSpan );
}

bool CMSTimeSpan::operator>( CMSTimeSpan span ) const throw()
{
	return( m_timeSpan > span.m_timeSpan );
}

bool CMSTimeSpan::operator<=( CMSTimeSpan span ) const throw()
{
	return( m_timeSpan <= span.m_timeSpan );
}

bool CMSTimeSpan::operator>=( CMSTimeSpan span ) const throw()
{
	return( m_timeSpan >= span.m_timeSpan );
}

/////////////////////////////////////////////////////////////////////////////
// CMSTime
/////////////////////////////////////////////////////////////////////////////

CMSTime WINAPI CMSTime::GetCurrentTime() throw()
{
	return( CMSTime( ::_time64( NULL ) ) );
}

BOOL WINAPI CMSTime::IsValidFILETIME(const FILETIME& fileTime) throw()
{
	FILETIME localTime;
	if (!FileTimeToLocalFileTime(&fileTime, &localTime))
	{
		return FALSE;
	}

	// then convert that time to system time
	SYSTEMTIME sysTime;
	if (!FileTimeToSystemTime(&localTime, &sysTime))
	{
		return FALSE;
	}

	return TRUE;
}

CMSTime::CMSTime() throw() :
	m_time(0)
{
}

CMSTime::CMSTime( __time64_t time )  throw():
	m_time( time )
{
}

CMSTime::CMSTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,
	int nDST)
{
#pragma warning (push)
#pragma warning (disable: 4127)  // conditional expression constant

	assert( nYear >= 1900 );
	assert( nMonth >= 1 && nMonth <= 12 );
	assert( nDay >= 1 && nDay <= 31 );
	assert( nHour >= 0 && nHour <= 23 );
	assert( nMin >= 0 && nMin <= 59 );
	assert( nSec >= 0 && nSec <= 59 );

#pragma warning (pop)

	struct tm atm;

	atm.tm_sec = nSec;
	atm.tm_min = nMin;
	atm.tm_hour = nHour;
	atm.tm_mday = nDay;
	atm.tm_mon = nMonth - 1;        // tm_mon is 0 based
	atm.tm_year = nYear - 1900;     // tm_year is 1900 based
	atm.tm_isdst = nDST;

	m_time = _mktime64(&atm);
	assert(m_time != -1);       // indicates an illegal input time
	/*if(m_time == -1)
	{
		AtlThrow(E_INVALIDARG);
	}*/
}

CMSTime::CMSTime(WORD wDosDate, WORD wDosTime, int nDST)
{
	struct tm atm;
	atm.tm_sec = (wDosTime & ~0xFFE0) << 1;
	atm.tm_min = (wDosTime & ~0xF800) >> 5;
	atm.tm_hour = wDosTime >> 11;

	atm.tm_mday = wDosDate & ~0xFFE0;
	atm.tm_mon = ((wDosDate & ~0xFE00) >> 5) - 1;
	atm.tm_year = (wDosDate >> 9) + 80;
	atm.tm_isdst = nDST;
	m_time = _mktime64(&atm);
	assert(m_time != -1);       // indicates an illegal input time

	/*if(m_time == -1)
		AtlThrow(E_INVALIDARG);*/

}

CMSTime::CMSTime(const SYSTEMTIME& sysTime, int nDST)
{
	if (sysTime.wYear < 1900)
	{
		__time64_t time0 = 0L;
		CMSTime timeT(time0);
		*this = timeT;
	}
	else
	{
		CMSTime timeT(
			(int)sysTime.wYear, (int)sysTime.wMonth, (int)sysTime.wDay,
			(int)sysTime.wHour, (int)sysTime.wMinute, (int)sysTime.wSecond,
			nDST);
		*this = timeT;
	}
}

CMSTime::CMSTime(const FILETIME& fileTime, int nDST)
{
	// first convert file time (UTC time) to local time
	FILETIME localTime;
	if (!FileTimeToLocalFileTime(&fileTime, &localTime))
	{
		m_time = 0;
		//AtlThrow(E_INVALIDARG);
		return;
	}

	// then convert that time to system time
	SYSTEMTIME sysTime;
	if (!FileTimeToSystemTime(&localTime, &sysTime))
	{
		m_time = 0;
		//AtlThrow(E_INVALIDARG);		
		return;
	}

	// then convert the system time to a time_t (C-runtime local time)
	CMSTime timeT(sysTime, nDST);
	*this = timeT;
}

CMSTime& CMSTime::operator=( __time64_t time ) throw()
{
	m_time = time;

	return( *this );
}

CMSTime& CMSTime::operator+=( CMSTimeSpan span ) throw()
{
	m_time += span.GetTimeSpan();

	return( *this );
}

CMSTime& CMSTime::operator-=( CMSTimeSpan span ) throw()
{
	m_time -= span.GetTimeSpan();

	return( *this );
}

CMSTimeSpan CMSTime::operator-( CMSTime time ) const throw()
{
	return( CMSTimeSpan( m_time-time.m_time ) );
}

CMSTime CMSTime::operator-( CMSTimeSpan span ) const throw()
{
	return( CMSTime( m_time-span.GetTimeSpan() ) );
}

CMSTime CMSTime::operator+( CMSTimeSpan span ) const throw()
{
	return( CMSTime( m_time+span.GetTimeSpan() ) );
}

bool CMSTime::operator==( CMSTime time ) const throw()
{
	return( m_time == time.m_time );
}

bool CMSTime::operator!=( CMSTime time ) const throw()
{
	return( m_time != time.m_time );
}

bool CMSTime::operator<( CMSTime time ) const throw()
{
	return( m_time < time.m_time );
}

bool CMSTime::operator>( CMSTime time ) const throw()
{
	return( m_time > time.m_time );
}

bool CMSTime::operator<=( CMSTime time ) const throw()
{
	return( m_time <= time.m_time );
}

bool CMSTime::operator>=( CMSTime time ) const throw()
{
	return( m_time >= time.m_time );
}

struct tm* CMSTime::GetGmtTm(struct tm* ptm) const
{
	// Ensure ptm is valid
	assert( ptm != NULL );

	if (ptm != NULL)
	{
		struct tm ptmTemp;
		errno_t err = _gmtime64_s(&ptmTemp, &m_time);

		// Be sure the call succeeded
		if(err != 0) { return NULL; }

		*ptm = ptmTemp;
		return ptm;
	}

	return NULL;
}

struct tm* CMSTime::GetLocalTm(struct tm* ptm) const
{
	// Ensure ptm is valid
	assert( ptm != NULL );

	if (ptm != NULL)
	{
		struct tm ptmTemp;
		errno_t err = _localtime64_s(&ptmTemp, &m_time);

		if (err != 0)
		{
			return NULL;    // indicates that m_time was not initialized!
		}

		*ptm = ptmTemp;
		return ptm;
	}

	return NULL;
}

bool CMSTime::GetAsSystemTime(SYSTEMTIME& timeDest) const throw()
{
	struct tm ttm;
	struct tm* ptm;

	ptm = GetLocalTm(&ttm);

	if(!ptm) { return false; }

	timeDest.wYear = (WORD) (1900 + ptm->tm_year);
	timeDest.wMonth = (WORD) (1 + ptm->tm_mon);
	timeDest.wDayOfWeek = (WORD) ptm->tm_wday;
	timeDest.wDay = (WORD) ptm->tm_mday;
	timeDest.wHour = (WORD) ptm->tm_hour;
	timeDest.wMinute = (WORD) ptm->tm_min;
	timeDest.wSecond = (WORD) ptm->tm_sec;
	timeDest.wMilliseconds = 0;

	return true;
}

__time64_t CMSTime::GetTime() const throw()
{
	return( m_time );
}

int CMSTime::GetYear() const
{ 
	struct tm ttm;
	struct tm * ptm;

	ptm = GetLocalTm(&ttm);
	return ptm ? (ptm->tm_year) + 1900 : 0 ; 
}

int CMSTime::GetMonth() const
{ 
	struct tm ttm;
	struct tm * ptm;

	ptm = GetLocalTm(&ttm);
	return ptm ? ptm->tm_mon + 1 : 0;
}

int CMSTime::GetDay() const
{
	struct tm ttm;
	struct tm * ptm;

	ptm = GetLocalTm(&ttm);
	return ptm ? ptm->tm_mday : 0 ; 
}

int CMSTime::GetHour() const
{
	struct tm ttm;
	struct tm * ptm;

	ptm = GetLocalTm(&ttm);
	return ptm ? ptm->tm_hour : -1 ; 
}

int CMSTime::GetMinute() const
{
	struct tm ttm;
	struct tm * ptm;

	ptm = GetLocalTm(&ttm);
	return ptm ? ptm->tm_min : -1 ; 
}

int CMSTime::GetSecond() const
{ 
	struct tm ttm;
	struct tm * ptm;

	ptm = GetLocalTm(&ttm);
	return ptm ? ptm->tm_sec : -1 ;
}

int CMSTime::GetDayOfWeek() const
{ 
	struct tm ttm;
	struct tm * ptm;

	ptm = GetLocalTm(&ttm);
	return ptm ? ptm->tm_wday + 1 : 0 ;
}

/////////////////////////////////////////////////////////////////////////////
// CMSFileTimeSpan
/////////////////////////////////////////////////////////////////////////////

CMSFileTimeSpan::CMSFileTimeSpan() throw() :
	m_nSpan( 0 )
{
}

CMSFileTimeSpan::CMSFileTimeSpan( const CMSFileTimeSpan& span ) throw() :
	m_nSpan( span.m_nSpan )
{
}

CMSFileTimeSpan::CMSFileTimeSpan( LONGLONG nSpan ) throw() :
	m_nSpan( nSpan )
{
}

CMSFileTimeSpan& CMSFileTimeSpan::operator=( const CMSFileTimeSpan& span ) throw()
{
	m_nSpan = span.m_nSpan;

	return( *this );
}

CMSFileTimeSpan& CMSFileTimeSpan::operator+=( CMSFileTimeSpan span ) throw()
{
	m_nSpan += span.m_nSpan;

	return( *this );
}

CMSFileTimeSpan& CMSFileTimeSpan::operator-=( CMSFileTimeSpan span ) throw()
{
	m_nSpan -= span.m_nSpan;

	return( *this );
}

CMSFileTimeSpan CMSFileTimeSpan::operator+( CMSFileTimeSpan span ) const throw()
{
	return( CMSFileTimeSpan( m_nSpan+span.m_nSpan ) );
}

CMSFileTimeSpan CMSFileTimeSpan::operator-( CMSFileTimeSpan span ) const throw()
{
	return( CMSFileTimeSpan( m_nSpan-span.m_nSpan ) );
}

bool CMSFileTimeSpan::operator==( CMSFileTimeSpan span ) const throw()
{
	return( m_nSpan == span.m_nSpan );
}

bool CMSFileTimeSpan::operator!=( CMSFileTimeSpan span ) const throw()
{
	return( m_nSpan != span.m_nSpan );
}

bool CMSFileTimeSpan::operator<( CMSFileTimeSpan span ) const throw()
{
	return( m_nSpan < span.m_nSpan );
}

bool CMSFileTimeSpan::operator>( CMSFileTimeSpan span ) const throw()
{
	return( m_nSpan > span.m_nSpan );
}

bool CMSFileTimeSpan::operator<=( CMSFileTimeSpan span ) const throw()
{
	return( m_nSpan <= span.m_nSpan );
}

bool CMSFileTimeSpan::operator>=( CMSFileTimeSpan span ) const throw()
{
	return( m_nSpan >= span.m_nSpan );
}

LONGLONG CMSFileTimeSpan::GetTimeSpan() const throw()
{
	return( m_nSpan );
}

void CMSFileTimeSpan::SetTimeSpan( LONGLONG nSpan ) throw()
{
	m_nSpan = nSpan;
}


/////////////////////////////////////////////////////////////////////////////
// CMSFileTime
/////////////////////////////////////////////////////////////////////////////

CMSFileTime::CMSFileTime() throw()
{
	dwLowDateTime = 0;
	dwHighDateTime = 0;
}

CMSFileTime::CMSFileTime( const FILETIME& ft ) throw()
{
	dwLowDateTime = ft.dwLowDateTime;
	dwHighDateTime = ft.dwHighDateTime;
}

CMSFileTime::CMSFileTime( ULONGLONG nTime ) throw()
{
	dwLowDateTime = DWORD( nTime );
	dwHighDateTime = DWORD( nTime>>32 );
}

CMSFileTime& CMSFileTime::operator=( const FILETIME& ft ) throw()
{
	dwLowDateTime = ft.dwLowDateTime;
	dwHighDateTime = ft.dwHighDateTime;

	return( *this );
}

CMSFileTime WINAPI CMSFileTime::GetCurrentTime() throw()
{
	CMSFileTime ft;
	GetSystemTimeAsFileTime(&ft);
	return ft;
}

CMSFileTime& CMSFileTime::operator+=( CMSFileTimeSpan span ) throw()
{
	SetTime( GetTime()+span.GetTimeSpan() );

	return( *this );
}

CMSFileTime& CMSFileTime::operator-=( CMSFileTimeSpan span ) throw()
{
	SetTime( GetTime()-span.GetTimeSpan() );

	return( *this );
}

CMSFileTime CMSFileTime::operator+( CMSFileTimeSpan span ) const throw()
{
	return( CMSFileTime( GetTime()+span.GetTimeSpan() ) );
}

CMSFileTime CMSFileTime::operator-( CMSFileTimeSpan span ) const throw()
{
	return( CMSFileTime( GetTime()-span.GetTimeSpan() ) );
}

CMSFileTimeSpan CMSFileTime::operator-( CMSFileTime ft ) const throw()
{
	return( CMSFileTimeSpan( GetTime()-ft.GetTime() ) );
}

bool CMSFileTime::operator==( CMSFileTime ft ) const throw()
{
	return( GetTime() == ft.GetTime() );
}

bool CMSFileTime::operator!=( CMSFileTime ft ) const throw()
{
	return( GetTime() != ft.GetTime() );
}

bool CMSFileTime::operator<( CMSFileTime ft ) const throw()
{
	return( GetTime() < ft.GetTime() );
}

bool CMSFileTime::operator>( CMSFileTime ft ) const throw()
{
	return( GetTime() > ft.GetTime() );
}

bool CMSFileTime::operator<=( CMSFileTime ft ) const throw()
{
	return( GetTime() <= ft.GetTime() );
}

bool CMSFileTime::operator>=( CMSFileTime ft ) const throw()
{
	return( GetTime() >= ft.GetTime() );
}

ULONGLONG CMSFileTime::GetTime() const throw()
{
	return( (ULONGLONG( dwHighDateTime )<<32)|dwLowDateTime );
}

void CMSFileTime::SetTime( ULONGLONG nTime ) throw()
{
	dwLowDateTime = DWORD( nTime );
	dwHighDateTime = DWORD( nTime>>32 );
}

CMSFileTime CMSFileTime::UTCToLocal() const throw()
{
	CMSFileTime ftLocal;

	::FileTimeToLocalFileTime( this, &ftLocal );

	return( ftLocal );
}

CMSFileTime CMSFileTime::LocalToUTC() const throw()
{
	CMSFileTime ftUTC;

	::LocalFileTimeToFileTime( this, &ftUTC );

	return( ftUTC );
}
