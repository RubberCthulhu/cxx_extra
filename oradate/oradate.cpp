
#include <occi.h>
#include <time.h>

#include "oradate.h"

using namespace oracle::occi;

#define UNIXTIME_BEGINNING_YEAR	1900
#define YEAR_TO_UTYEAR(y)	(y - UNIXTIME_BEGINNING_YEAR)
#define UTYEAR_TO_YEAR(y)	(y + UNIXTIME_BEGINNING_YEAR)

time_t oradateToUnixtime(const Date &date)
{
	struct tm tm;
	oradateToBrokendown(tm, date);
	
	return mktime(&tm);
}

void oradateToBrokendown(struct tm &tm, const Date &date)
{
	int year;
	unsigned int month, mday, hour, min, sec;
	
	date.getDate(year, month, mday, hour, min, sec);
	
	//struct tm tm;
	tm.tm_year = YEAR_TO_UTYEAR(year);
	tm.tm_mon = month - 1;
	tm.tm_mday = mday;
	tm.tm_hour = hour;
	tm.tm_min = min;
	tm.tm_sec = sec;
	
	//return tm;
}

void unixtimeToOradate(Date &odate, time_t udate)
{
	struct tm tm;
	localtime_r(&udate, &tm);
	brokendownToOradate(odate, tm);
}

void brokendownToOradate(Date &odate, const struct tm &tm)
{
	odate.setDate(UTYEAR_TO_YEAR(tm.tm_year), 
		tm.tm_mon + 1,
		tm.tm_mday,
		tm.tm_hour,
		tm.tm_min,
		tm.tm_sec
	);
}









