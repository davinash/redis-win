#ifndef WIN_THREAD_H_
#define WIN_THREAD_H_

#ifdef WIN32

typedef HANDLE pthread_mutex_t;
typedef struct pthread_attr_t pthread_attr_t;
struct pthread_attr_t {
	unsigned p_state;
	void *stack;
	size_t s_size;
};

typedef DWORD pthread_t;



int pthread_mutex_init(pthread_mutex_t *mutex, void *unused);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
void pthread_attr_init(pthread_attr_t *ptr);
void pthread_attr_getstacksize(pthread_attr_t *ptr, size_t *nSize);
void pthread_attr_setstacksize(pthread_attr_t *ptr, size_t nSize);
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg);
#define pthread_equal(x, y) ((x) == (y))

pthread_t pthread_self();
void pthread_detach(pthread_t id);


typedef struct {
  HANDLE signal, broadcast;
} pthread_cond_t;

int pthread_cond_init(pthread_cond_t *cv, const void *unused);
int pthread_cond_wait(pthread_cond_t *cv, pthread_mutex_t *mutex);
int pthread_cond_signal(pthread_cond_t *cv);

#endif /* WIN32 */

#endif /* WIN_THREAD_H_ */