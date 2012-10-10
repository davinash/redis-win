#ifdef WIN32
#include <Windows.h>
#include "win_thread.h"

int pthread_mutex_init(pthread_mutex_t *mutex, void *unused) {
  unused = NULL;
  *mutex = CreateMutex(NULL, FALSE, NULL);
  return *mutex == NULL ? -1 : 0;
}

int pthread_mutex_lock(pthread_mutex_t *mutex) {
  return WaitForSingleObject(*mutex, INFINITE) == WAIT_OBJECT_0 ? 0 : -1;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex) {
  return ReleaseMutex(*mutex) == 0 ? -1 : 0;
}

void pthread_attr_init(pthread_attr_t *ptr) {
}

void pthread_attr_getstacksize(pthread_attr_t *ptr, size_t *nSize) {
  *nSize = 1;
}
void pthread_attr_setstacksize(pthread_attr_t *ptr, size_t nSize) {
  ptr->s_size =  nSize;
}

pthread_t pthread_self() {
  return GetCurrentThreadId();
}

void pthread_detach(pthread_t id) {
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg) {
  if (CreateThread(NULL, attr->s_size, (LPTHREAD_START_ROUTINE)start_routine, NULL, 0, NULL) == NULL ) {
    return -1;
  }
  return 0;
}

int pthread_cond_init(pthread_cond_t *cv, const void *unused) {
  unused = NULL;
  cv->signal = CreateEvent(NULL, FALSE, FALSE, NULL);
  cv->broadcast = CreateEvent(NULL, TRUE, FALSE, NULL);
  return cv->signal != NULL && cv->broadcast != NULL ? 0 : -1;
}

int pthread_cond_wait(pthread_cond_t *cv, pthread_mutex_t *mutex) {
  HANDLE handles[] = {cv->signal, cv->broadcast};
  ReleaseMutex(*mutex);
  WaitForMultipleObjects(2, handles, FALSE, INFINITE);
  return WaitForSingleObject(*mutex, INFINITE) == WAIT_OBJECT_0? 0 : -1;
}

int pthread_cond_signal(pthread_cond_t *cv) {
  return SetEvent(cv->signal) == 0 ? -1 : 0;
}


#endif