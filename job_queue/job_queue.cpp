
#include <queue>
#include <deque>
#include <pthread.h>

#include "job_queue.h"

using namespace std;

Job::Job()
{
	init(0, 0, 0);
}

Job::Job(job_action_t act, void* data, job_userdata_destroy_t _destroy)
{
	init(act, data, _destroy);
}

Job::~Job()
{
	if( userdata && destroy )
		(*destroy)(userdata);
}

void Job::init(job_action_t act, void* data, job_userdata_destroy_t _destroy)
{
	action = act;
	userdata = data;
	destroy = _destroy;
}

void Job::setData(void* data, job_userdata_destroy_t _destroy)
{
	userdata = data;
	destroy = _destroy;
}

void * Job::getData()
{
	return userdata;
}

void * Job::doit()
{
	return (*action)(userdata);
}

void * Job::doit(void* data)
{
	if( data )
		return (*action)(data);
	return (*action)(userdata);
}

JobQueue::JobQueue()
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
}

JobQueue::~JobQueue()
{
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}

void JobQueue::put(Job *job)
{
	pthread_mutex_lock(&mutex);
	
	jobQueue.push(job);
	
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
}

void JobQueue::put(job_action_t action, void* userdata, job_userdata_destroy_t dataDestroy)
{
	Job *job = new Job(action, userdata, dataDestroy);
	put(job);
}

Job * JobQueue::get()
{
	Job *job;
	
	pthread_mutex_lock(&mutex);
	while( jobQueue.empty() )
		pthread_cond_wait(&cond, &mutex);
	
	job = jobQueue.front();
	jobQueue.pop();
	
	if( !jobQueue.empty() )
		pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
	
	return job;
}

void * JobQueue::doit()
{
	void *r;
	
	Job *job = get();
	if( job ) {
		r = job->doit();
		delete job;
	}
	
	return r;
}





