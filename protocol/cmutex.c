#include "cmutex.h"

void cmutex_init(struct cmutex* mutex,
		entry_exit_fn prologue,
		entry_exit_fn epilogue) {

	if(!mutex)
		return;

   	pthread_mutex_init(&mutex->res_mgr, NULL);
   	pthread_mutex_init(&mutex->crit_sec, NULL);
	mutex->prologue = entry;
	mutex->epilogue = exit;
	
	return;
}

void cmutex_proberen(struct cmutex* mutex, void *params) {

	while(!pthread_mutex_lock(&mutex->res_mgr)) {
		/*
		* if the entry and exit is NULL then
		* just use a single mutex with normal 
		* functionality. 
		*/
		if(!mutex->prologue)
			break;

		pthread_mutex_lock(&mutex->crit_sec);
		if(!mutex->prologue(params)) {
			pthread_mutex_unlock(&mutex->crit_sec);	
		}
		else {
			pthread_mutex_unlock(&mutex->crit_sec);
			continue;
		}
		pthread_mutex_unlock(&mutex->res_mgr);
		break;
	}
}

void cmutex_verhogen(struct cmutex* mutex, void *params) 
{
	/*
	* If entry is null then it should act like
	* a normal mutex functionality with wrapper.
	*/
	if(!mutex->prologue) {
		pthread_mutex_unlock(&mutex->res_mgr);
		if(mutex->epilogue)
			mutex->exit(params);
	}

	pthread_mutex_lock(&mutex->crit_sec);

	if(mutex->epilogue) {
		if(mutex->epilogue(params) <= 0)
			pthread_mutex_unlock(&mutex->res_mgr);
	}
	
	pthread_mutex_unlock(&mutex->crit_sec);
}

