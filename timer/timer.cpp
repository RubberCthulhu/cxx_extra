
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/epoll.h>
#include <string>
#include <list>
#include <algorithm>

#include "timer.h"

using namespace std;

#define EPOLL_TIMEOUT(tm) ((tm) == -1 ? -1 : (tm) * 1000)

Timer::Timer()
{
	init = false;
	
	pthread_mutex_init(&lock, NULL);
	
	epfd = epoll_create(10);
	if( epfd == -1 ) {
		pthread_mutex_destroy(&lock);
		throw TimerException(errno, strerror(errno));
	}
	
	int fds[2];
	if( pipe(fds) == -1 ) {
		close(epfd);
		pthread_mutex_destroy(&lock);
		throw TimerException(errno, strerror(errno));
	}
	
	int flags;
	flags = fcntl(fds[0], F_GETFL, 0);
	fcntl(fds[0], F_SETFL, flags|O_NONBLOCK);
	//flags = fcntl(fds[1], F_GETFL, 0);
	//fcntl(fds[1], F_SETFL, flags|O_NONBLOCK);
	
	pIn = fds[0];
	pOut = fds[1];
	
	struct epoll_event ev;
	ev.data.fd = pIn;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, pIn, &ev);
	
	runLoop = false;
	deadline = 0;
	
	init = true;
}

Timer::~Timer()
{
	if( init ) {
		pthread_mutex_destroy(&lock);
		close(epfd);
		close(pIn);
		close(pOut);
	}
}

bool Timer::needSchedule()
{
	if( queue.empty() ) {
		if( deadline != 0 )
			return true;
	}
	else {
		TimerAction *action = queue.top();
		if( deadline > 0 )
			return action->tmDeadline < deadline;
		else
			return true;
	}
}

time_t Timer::schedule()
{
	time_t timeout;
	
	if( queue.empty() ) {
		deadline = 0;
		timeout = -1;
	}
	else {
		time_t tm_now = time(NULL);
		TimerAction *action = queue.top();
		deadline = action->tmDeadline;
		
		if( deadline <= tm_now ) {
			timeout = 0;
		}
		else {
			timeout = deadline - tm_now;
		}
	}
	
	return timeout;
}

void Timer::run()
{
	struct epoll_event events[1];
	int timeout;
	bool loop;
	
	pthread_mutex_lock(&lock);
	runLoop = true;
	loop = true;
	timeout = EPOLL_TIMEOUT(schedule());
	pthread_mutex_unlock(&lock);
	
	while( loop ) {
		int n = epoll_wait(epfd, events, 1, timeout);
		if( n == -1 ) {
			throw TimerException(errno, strerror(errno));
		}
		else if( n == 0 ) {
			doit();
			pthread_mutex_lock(&lock);
			timeout = EPOLL_TIMEOUT(schedule());
			pthread_mutex_unlock(&lock);
		}
		else {
			for( int i = 0 ; i < n ; i++ ) {
				int fd = events[i].data.fd;
				if( fd == pIn ) {
					pthread_mutex_lock(&lock);
					int len;
					try {
						len = cleanSignal();
					}
					catch( TimerException &e ) {
						pthread_mutex_unlock(&lock);
						throw e;
						continue;
					}
					
					if( len > 0 ) {
						if( !runLoop )
							loop = false;
						else
							timeout = EPOLL_TIMEOUT(schedule());
					}
					pthread_mutex_unlock(&lock);
				}
			}
		}
	}
}

int Timer::cleanSignal()
{
	int n, total = 0;
	char tmp[10];
	
	bool loop = true;
	while( loop ) {
		n = read(pIn, tmp, 10);
		if( n == -1 ) {
			if( errno != EAGAIN ) {
				throw TimerException(errno, strerror(errno));
			}
			loop = false;
		}
		else if( n == 0 ) { // oops! pipe has been closed!
			throw TimerException(0, "Pipe has been closed");
			loop = false;
		}
		else
			total += n;
	}
	
	return total;
}

int Timer::doit()
{
	list<TimerAction *> ready;
	
	pthread_mutex_lock(&lock);
	time_t tm_now = time(NULL);
	bool loop = true;
	while( loop ) {
		if( !queue.empty() ) {
			TimerAction *action = queue.top();
			if( action->tmDeadline <= tm_now ) {
				queue.pop();
				ready.push_back(action);
			}
			else {
				loop = false;
			}
		}
		else
			loop = false;
	}
	pthread_mutex_unlock(&lock);
	
	for_each(ready.begin(), ready.end(), DoIt());
	
	return ready.size();
}

void Timer::signal()
{
	char ch = 0xff;
	char *ptr = &ch;
	
	int n;
	while( (ptr < &ch+sizeof(ch)) && (n = write(pOut, ptr, &ch+sizeof(ch)-ptr)) >= 0 ) {
		ptr += n;
	}
	
	if( n == -1 )
		throw TimerException(errno, strerror(errno));
}

void Timer::breakloop()
{
	pthread_mutex_lock(&lock);
	runLoop = false;
	try {
		signal();
	}
	catch( TimerException &e ) {
		pthread_mutex_unlock(&lock);
		throw e;
		return;
	}
	pthread_mutex_unlock(&lock);
}

void Timer::add(time_t tmDeadline, timer_action_t cb, void* userdata)
{
	pthread_mutex_lock(&lock);
	TimerAction *action = new TimerAction(tmDeadline, cb, userdata);
	queue.push(action);
	
	if( needSchedule() ) {
		try {
			signal();
		}
		catch( TimerException &e ) {
			pthread_mutex_unlock(&lock);
			throw e;
			return;
		}
	}
	pthread_mutex_unlock(&lock);
}





