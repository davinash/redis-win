#ifndef _WIN_PORT_H_
#define _WIN_PORT_H_

#define WIN32_LEAN_AND_MEAN
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <io.h>
#include <time.h>
#include <process.h>
#include <errno.h>
#include <float.h>
#include <direct.h>

#include "win_thread.h"

#define inline __inline
typedef int mode_t;
//typedef SSIZE_T ssize_t;

#define _OFF_T_DEFINED
#define off_t __int64
#define _off_t __int64


#if !defined(ssize_t)
typedef int ssize_t;
#endif

#define strcasecmp _stricmp 
#define ftruncate(x,y) _chsize((int)x, (long)y)
#define pipe(x) _pipe(x, 8192, _O_BINARY)
#define random rand
#define fseeko(x, y, z) _lseeki64(_fileno(x), (y), (z))
#define read(x, y, z) _read((x), (y), (unsigned) z)
#define usleep(x) Sleep(x)
#define write(x, y, z) _write((x), (y), (unsigned) z)
#define snprintf _snprintf
typedef unsigned __int32 u_int32_t;
#define __i386__ 1 /* for endian detection */
#define va_copy(d,s)  d = (s)
#define lseek _lseek
#define close(x) _close(x)
#define open _open
#define unlink _unlink
#define sleep Sleep
#define getpid _getpid
#define strtoll(x, y, z) strtol(x, y, z)
#define isnan _isnan
#define isfinite _finite
#define isinf(x) (!_finite(x))
#define ftello _ftelli64
#define chdir _chdir
#define getcwd _getcwd


#if !defined(fileno)
#define fileno(x) _fileno(x)
#endif

#define pid_t HANDLE

struct timezone {
  int tz_minuteswest; /* minutes W of Greenwich */
  int tz_dsttime; /* type of dst correction */
};

int gettimeofday(struct timeval *tv, struct timezone *tz);
#define waitpid(pid,statusp,options) _cwait (statusp, pid, WAIT_CHILD)
pid_t wait3(int *stat_loc, int options, void *rusage);
#define WNOHANG 1
#define LOG_LOCAL0 0

#define SIGKILL 0
int kill(pid_t pid, int sig_num);
int fsync (int fd);

struct rusage {
    struct timeval ru_utime; /* user time used */
    struct timeval ru_stime; /* system time used */
};

#define RUSAGE_SELF 0
#define RUSAGE_CHILDREN (-1)
int getrusage(int who, struct rusage * rusage);

pid_t fork();

#define WTERMSIG(x) ((x) & 0xff) /* or: SIGABRT ?? */
#define WCOREDUMP(x) 0
#define WEXITSTATUS(x) (((x) >> 8) & 0xff) /* or: (x) ?? */
#define WIFSIGNALED(x) (WTERMSIG (x) != 0) /* or: ((x) == 3) ?? */

typedef struct winsize {
  unsigned short ws_row;
  unsigned short ws_col;
};

#define O_NONBLOCK FIONBIO 

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2


#define setsockopt(a, b, c, d, e) setsockopt(a, b, c, (char*)d, e)
#define getsockopt(a, b, c, d, e) getsockopt(a, b, c, (char*)d, e) 
#define localtime(x) localtime((const time_t *)x)

#define inet_aton(a, b)  inet_pton(AF_INET, a, b )
#define sockaddr_un sockaddr_in
#define srand(x) srand( (unsigned int)x)
#define strtold(x,y) strtod((const char *)x, y)
//#define memtoll(x,y) (size_t)memtoll(x,y)

int poll(struct pollfd *ufds, unsigned int nfds, int timeout);

int strncasecmp(const char *s1, const char *s2, size_t len);
#define execve _execve

#define X_OK 00
#define access _access
#define strtoull(x, y, z) strtoul(x, y, z)
#define strdup _strdup

#endif