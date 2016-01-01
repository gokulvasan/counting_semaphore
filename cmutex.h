
#include <pthreads.h>

typedef int (*entry_exit_fn)(void *);

/*
* The Following Algorithm tries to exploit the method by which mutex works.i.e.
*
* In this system an atomic increment and test operation is performed on the mutex variable in user space.
*
* If the result of the operation indicates that there was no contention on the lock, 
* the call to pthread_mutex_lock returns without ever context switching into the kernel, 
* so the operation of taking a mutex can be very fast.
*
* Only if contention was detected does a system call (called futex) and 
* context switch into the kernel occurs that puts the calling process to sleep until the mutex is released.
* 
*/

/*
* @ structure name: cmutex
* An approach to use atmost only 2 mutex for managing n homogeneous resources.
* Could be used as a counting semaphore and also for other approaches.
*
* @@ prologue : function pointer that conditions on entry protocol.
*	@@@  input : void pointer, params needed for resource management.
*	@@@  return : 0 on success and 1 on non-availability.
*	@@@ On Null works like a normal mutex.
* @@ epilogue : function pointer that conditions on crtical section exit.
*	@@@ input : pointer for configurable resource management.
*	@@@ return: less than or equals 0 on contenetion else a +ve val.
*	@@@ On Null works like a normal mutex.
* @@ res_mgr : mutex : abstract from end user implementaion manages resource.
* @@  crit_sec : mutex : abstract from end user implementaion manages
*			critical section. 
*/
struct cmutex {
	entry_exit_fn  prologue;
	entry_exit_fn  epilogue;
	pthread_mutex_t res_mgr;
	pthread_mutex_t crit_sec;
};

/*
* @ Function Name: cmutex_init
*	Initializes the struct cmutex
* @ cmutex: reference to struct cmutex.
* @ entry : the function that decides on entry.
* @ exit : The function that cleans up or called on exit.
*/
void cmutex_init(struct cmutex*, entry_exit_fn entry, 
				entry_exit_fn exit);

/*
* proberen Function
* @ On no contenetion the process enters the critical section
*   else blocks itself.
*/
void cmutex_proberen(struct cmutex *);

/*
* verhogen function
* @ exits critical section.
*/
void cmutex_verhogen(struct cmutex *);


