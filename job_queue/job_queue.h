/* 
 * File:   job_queue.h
 * Author: alevandal
 *
 * Created on 17 Август 2012 г., 19:04
 */

#ifndef JOB_QUEUE_H
#define	JOB_QUEUE_H

#include <queue>
#include <deque>
#include <pthread.h>

typedef void * (*job_action_t)(void *);
typedef void (*job_userdata_destroy_t)(void *);

class Job {
private:
	job_action_t action;
	void *userdata;
	job_userdata_destroy_t destroy;
	
	void init(job_action_t act, void *data, job_userdata_destroy_t _destroy);
	
public:
	Job();
	Job(job_action_t act, void *data = 0, job_userdata_destroy_t _destroy = 0);
	~Job();
	
	void setData(void *data, job_userdata_destroy_t _destroy = 0);
	void * getData();
	void * doit();
	void * doit(void *data);
};

class JobQueue {
private:
	std::queue<Job *, std::deque<Job *> > jobQueue;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	
	void put(Job *job);
	Job * get();
public:
	JobQueue();
	~JobQueue();
	
	void put(job_action_t action, void *userdata, job_userdata_destroy_t dataDestroy = 0);
	void * doit();
};

#endif	/* JOB_QUEUE_H */

