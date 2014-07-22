/* 
 * File:   timer.h
 * Author: alevandal
 *
 * Created on 22 Август 2012 г., 12:40
 */

#ifndef TIMER_H
#define	TIMER_H

#include <pthread.h>
#include <time.h>
#include <string>
#include <queue>
#include <vector>

class TimerException {
private:
	int err;
	std::string text;
public:
	TimerException(int e, std::string t): err(e), text(t) {}
	int getError() { return err; }
	std::string getMessage() { return text; }
};

typedef void (*timer_action_t)(void *);

class Timer {
protected:
	struct TimerAction {
		time_t tmDeadline;
		timer_action_t callback;
		void *userdata;
		TimerAction(time_t tm, timer_action_t cb, void *data): tmDeadline(tm), callback(cb), userdata(data) {}
	};
	
	struct CompareAction {
		bool operator ()(TimerAction *a, TimerAction *b)
		{
			return a->tmDeadline > b->tmDeadline;
		}
	};
	
	struct DoIt {
		void operator ()(TimerAction *action)
		{
			(*action->callback)(action->userdata);
			delete action;
		}
	};
	
	std::priority_queue <TimerAction *, std::vector<TimerAction *>, CompareAction> queue;
	
	int epfd;
	int pIn;
	int pOut;
	bool init;
	
	pthread_mutex_t lock;
	
	bool runLoop;
	
	time_t deadline;
	bool needSchedule();
	time_t schedule();
	
	int doit();
	void signal();
	int cleanSignal();
	
public:
	Timer();
	~Timer();
	
	void run();
	void breakloop();
	void add(time_t tmDeadline, timer_action_t cb, void *userdata);
};

#endif	/* TIMER_H */

