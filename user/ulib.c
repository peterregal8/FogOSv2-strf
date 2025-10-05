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
  int defaultstr = 0;
  int hasdecimal = 0;
  int hasfill = 0;
  int grouping = 1;
  int symbol = 1;
  //int negative = 0;
  char buf[maxsize];
  int i = 0;
  char fill = ' ';
  int numfill = 0;
  int specialfill = 0;
  //if (format[0] == '-') {
    //negative = 1;
    //i++;
  //}
  while ((i < strlen(format)) && (format[i] != '%')) {
    if (format[i] == '.') {
      hasdecimal = 1;
    }
    buf[i] = format[i];
    i++;
  }
  i++;
  if (format[i] == 'n') {
    defaultstr = 1;
  } else {
    while (i < strlen(format)) {
      if (format[i] == '^') {
        grouping = 0;
      } else if (format[i] == '!') {
        symbol = 0;
      } else if (format[i] == '=') {
        specialfill = 1;
        fill = format[i + 1];
      } else if (format[i] == '#') {
        hasfill = 1;
        numfill = format[i + 1] - '0';
      }
      i++;
    }
  }
  buf[i] = '\n';
  if (defaultstr == 1) {
    int k = 0;
    s[k] = '[';
    k++;
    //if (negative == 1) {
      //s[k] = '-';
      //k++;
    //}
    s[k] = '$';
    k++;
    int bufindex = 0;
    int l = 0;
    if (hasdecimal == 1) {
      int beforedecimal = strlen(buf) - 3;
      while (bufindex < strlen(buf)) {
        s[k] = buf[l];
        bufindex++;
        k++;
        if (bufindex < beforedecimal) {
          if ((beforedecimal - bufindex) % 3 == 0) {
            s[k] = ',';
            k++;
          }
        }
        l++;    
      }  
      s[k] = ']';
      k++;
      s[k] = '\0';
    } else {
      while (bufindex < strlen(buf)) {
        s[k] = buf[l];
        bufindex++;
        k++;
        if ((strlen(buf) - bufindex) % 3 == 0 && strlen(buf) != bufindex) {
          s[k] = ',';
          k++;
        }
        l++;
      }
      s[k] = '.';
      k++;
      s[k] = '0';
      k++;
      s[k] = '0';
      k++;
      s[k] = ']';
      k++;
      s[k] = '\0';
    }
  } else {
    int k = 0;
    s[k] = '[';
    k++;
    //if (negative == 1) {
      //s[k] = '-';
      //k++;
    //}
    int bufindex = 0;
    if (symbol == 1) {
      s[k] = '$';
      k++;
    }
    if (hasfill == 1) {
      int digits = strlen(buf) - 3;
      for (int i = 0; i < numfill - digits; i++) {
        if (specialfill == 1) {
          s[k] = fill;
        } else {
          s[k] = ' ';
        }
        k++; 
      }
    }
    if (grouping == 1) {
      int l = 0;
      if (hasdecimal == 1) {
        int beforedecimal = strlen(buf) - 3;
        while (bufindex < strlen(buf)) {
          s[k] = buf[l];
          bufindex++;
          k++;
          if (bufindex < beforedecimal) {
            if ((beforedecimal - bufindex) % 3 == 0) {
              s[k] = ',';
              k++;
            }
          }
          l++;    
        }  
        s[k] = ']';
        k++;
        s[k] = '\0';
      } else {
        while (bufindex < strlen(buf)) {
          s[k] = buf[l];
          bufindex++;
          k++;
          if ((strlen(buf) - bufindex) % 3 == 0 && strlen(buf) != bufindex) {
            s[k] = ',';
            k++;
          }
          l++;
        }
        s[k] = '.';
        k++;
        s[k] = '0';
        k++;
        s[k] = '0';
        k++;
        s[k] = ']';
        k++;
        s[k] = '\0';
      }
    } else {
      if (hasdecimal == 1) {
        while (bufindex < strlen(buf)) {
          s[k] = buf[bufindex];
          k++;
          bufindex++;
        }
        s[k] = ']';
        k++;
        s[k] = '\0';
      } else {
        while (bufindex < strlen(buf)) {
          s[k] = buf[bufindex];
          k++;
          bufindex++;
        }
        s[k] = '.';
        k++;
        s[k] = '0';
        k++;
        s[k] = '0';
        k++;
        s[k] = ']';
        k++;
        s[k] = '\0';
      }
    }
  }
  return strlen(s);
}

