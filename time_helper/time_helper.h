/* 
 * File:   time_helper.h
 * Author: alevandal
 *
 * Created on 23 Август 2012 г., 12:11
 */

#ifndef TIME_HELPER_H
#define	TIME_HELPER_H

#include <time.h>

time_t time_day_beginning(struct tm tm);
time_t time_local_next_day();
int time_local_is_day_changed(time_t tm_old, time_t tm_new);

#endif	/* TIME_HELPER_H */

