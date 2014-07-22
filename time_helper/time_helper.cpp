
#include <time.h>
#include "time_helper.h"

#define DAY_SEC (24 * 60 *60)

time_t time_day_beginning(struct tm tm)
{
	tm.tm_sec = 0;
	tm.tm_min = 0;
	tm.tm_hour = 0;
	
	time_t tm_next_day = mktime(&tm);
	return tm_next_day;
}

time_t time_local_next_day()
{
	struct tm tm;
	time_t t = time(NULL) + DAY_SEC;
	localtime_r(&t, &tm);
	return time_day_beginning(tm);
}

int time_local_is_day_changed(time_t tm_old, time_t tm_new)
{
	struct tm tm1, tm2;
	
	if( localtime_r(&tm_old, &tm1) == NULL )
		return 0;
	
	if( localtime_r(&tm_new, &tm2) == NULL )
		return 0;
	
	int r = 0;
	if( tm2.tm_mday > tm1.tm_mday )
		r = 1;
	else if( tm2.tm_mon > tm1.tm_mon )
		r = 1;
	else if( tm2.tm_year > tm2.tm_year )
		r = 1;
	
	return r;
}





