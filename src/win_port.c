#include "win_port.h"

#define DELTA_EPOCH_IN_MICROSECS 11644473600000000Ui64

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
  FILETIME ft;
  unsigned __int64 tmpres = 0;
  static int tzflag;

  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);

    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;

    /*converting file time to unix epoch*/
    tmpres -= DELTA_EPOCH_IN_MICROSECS;
    tmpres /= 10; /*convert into microseconds*/
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }

  if (NULL != tz)
  {
    if (!tzflag)
    {
      _tzset();
      tzflag++;
    }
    tz->tz_minuteswest = _timezone / 60;
    tz->tz_dsttime = _daylight;
  }

  return 0;
}

pid_t wait3(int *stat_loc, int options, void *rusage) {
    return (pid_t) waitpid((intptr_t) -1, 0, WAIT_FLAGS);
}

int kill(pid_t pid, int sig_num) {
  (void) TerminateProcess(pid, sig_num);
  (void) CloseHandle(pid);
  return 0;
}

int fsync (int fd) {
    HANDLE h = (HANDLE) _get_osfhandle(fd);
    DWORD err;
    if (h == INVALID_HANDLE_VALUE) {
        errno = EBADF;
        return -1;
    }
    if (!FlushFileBuffers(h)) {
        /* Windows error -> Unix */
        err = GetLastError();
        switch (err) {
            case ERROR_INVALID_HANDLE:
            errno = EINVAL;
            break;

            default:
            errno = EIO;
        }
        return -1;
    }
    return 0;
}

int getrusage(int who, struct rusage * r) {

   FILETIME starttime, exittime, kerneltime, usertime;
   ULARGE_INTEGER li;

   if (r == NULL) {
       errno = EFAULT;
       return -1;
   }

   memset(r, 0, sizeof(struct rusage));

   if (who == RUSAGE_SELF) {
     if (!GetProcessTimes(GetCurrentProcess(),
                        &starttime,
                        &exittime,
                        &kerneltime,
                        &usertime))
     {
         errno = EFAULT;
         return -1;
     }
   }

   if (who == RUSAGE_CHILDREN) {
        /* Childless on windows */
        starttime.dwLowDateTime   = 0;
        starttime.dwHighDateTime  = 0;
        exittime.dwLowDateTime    = 0;
        exittime.dwHighDateTime   = 0;
        kerneltime.dwLowDateTime  = 0;
        kerneltime.dwHighDateTime = 0;
        usertime.dwLowDateTime    = 0;
        usertime.dwHighDateTime   = 0;
   }
    memcpy(&li, &kerneltime, sizeof(FILETIME));
    li.QuadPart /= 10L;
    r->ru_stime.tv_sec  = (long)(li.QuadPart / 1000000L);
    r->ru_stime.tv_usec = (long)(li.QuadPart % 1000000L);

    memcpy(&li, &usertime, sizeof(FILETIME));
    li.QuadPart /= 10L;
    r->ru_utime.tv_sec  = (long)(li.QuadPart / 1000000L);
    r->ru_utime.tv_usec = (long)(li.QuadPart % 1000000L);

    return 0;
}

pid_t fork() {
  return NULL;
}

int poll(struct pollfd *ufds, unsigned int nfds, int timeout) {
    SOCKET fd;
    unsigned int idx;
    int r;
    fd_set readfds, writefds, exceptfds;
    struct timeval _timeout;

    _timeout.tv_sec = timeout/1000;
    _timeout.tv_usec = (timeout%1000)*1000;

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);

    for (idx = 0; idx < nfds; ++idx)
    {
        fd = ufds[idx].fd;
        if (fd == (SOCKET) -1)
            continue;
        if (ufds[idx].events & POLLIN)
            FD_SET(fd, &readfds);
        if (ufds[idx].events & POLLOUT)
            FD_SET(fd, &writefds);
        FD_SET(fd, &exceptfds);
    }
    r = select(nfds, &readfds, &writefds, &exceptfds,
            timeout == -1 ? NULL : &_timeout);
    if (r < 0)
        return r;
    r = 0;
    for (idx = 0; idx < nfds; ++idx) {
        fd = ufds[idx].fd;
        ufds[idx].revents = 0;
        if (fd == (SOCKET) -1)
            continue;
        if (FD_ISSET(fd, &readfds))
            ufds[idx].revents |= POLLIN;
        if (FD_ISSET(fd, &writefds))
            ufds[idx].revents |= POLLOUT;
        if (FD_ISSET(fd, &exceptfds))
            ufds[idx].revents |= POLLERR;
        if (ufds[idx].revents)
            ++r;
    }
    return r;
}


static int lowercase(const char *s) {
  return tolower(* (const unsigned char *) s);
}

int strncasecmp(const char *s1, const char *s2, size_t len) {
  int diff = 0;

  if (len > 0)
    do {
      diff = lowercase(s1++) - lowercase(s2++);
    } while (diff == 0 && s1[-1] != '\0' && --len > 0);

  return diff;
}
