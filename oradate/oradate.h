/* 
 * File:   ora_date.h
 * Author: alevandal
 *
 * Created on 17 Май 2012 г., 13:55
 */

#ifndef ORA_DATE_H
#define	ORA_DATE_H

#include <occi.h>
#include <time.h>

time_t oradateToUnixtime(const oracle::occi::Date &date);
void oradateToBrokendown(struct tm &tm, const oracle::occi::Date &date);
void unixtimeToOradate(oracle::occi::Date &odate, time_t udate);
void brokendownToOradate(oracle::occi::Date &odate, const struct tm &tm);

#endif	/* ORA_DATE_H */

