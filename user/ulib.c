#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/riscv.h"
#include "kernel/vm.h"
#include "user/user.h"
#include <stddef.h>

//
// wrapper so that it's OK if main() does not call exit().
//
void
start()
{
  extern int main();
  main();
  exit(0);
}

char*
strcpy(char *s, const char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(const char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
    cdst[i] = c;
  }
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
stat(const char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, const void *vsrc, int n)
{
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  if (src > dst) {
    while(n-- > 0)
      *dst++ = *src++;
  } else {
    dst += n;
    src += n;
    while(n-- > 0)
      *--dst = *--src;
  }
  return vdst;
}

int
memcmp(const void *s1, const void *s2, uint n)
{
  const char *p1 = s1, *p2 = s2;
  while (n-- > 0) {
    if (*p1 != *p2) {
      return *p1 - *p2;
    }
    p1++;
    p2++;
  }
  return 0;
}

void *
memcpy(void *dst, const void *src, uint n)
{
  return memmove(dst, src, n);
}

char *
sbrk(int n) 
{
  return sys_sbrk(n, SBRK_EAGER);
}

char *
sbrklazy(int n) 
{
  return sys_sbrk(n, SBRK_LAZY);
}

long int
strfmon(char *restrict s, size_t maxsize, const char *restrict format, ...) 
{
  int defaultstr = 0; // default money formatting
  char buf[maxsize];
  char align[32];
  int i = 0;
  while ((i < strlen(format)) && (format[i] != '%')) {
    buf[i] = format[i];
    i++;
  }
  buf[i] = '\n';
  if (format[i] == 'n') { // check for n right after % for default formatting
    defaultstr = 1;
  } else {
    int alignnum = 0;
    while (i < strlen(format)) {
        align[alignnum] = format[i];
        alignnum++;
        i++;
    }
    align[alignnum] = '\n';
  }
  if (defaultstr == 1) {
    return 0;
  } else {
      s[0] = '[';
      int l = 1;
      for (int f = 0; f < atoi(align) - strlen(buf) - 2; f++) {
        s[l] = ' ';
        l++;
      }
      for (int k = 1; k < strlen(buf); k++) {
        s[k] = buf[k - 1];
        l++;
      }
      s[l] = ']';
      return strlen(buf);
  }
}

