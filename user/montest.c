#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char buf[64];
  strfmon(buf, 64, "132.45%n");
  printf("%s\n", buf);
  return 1;
}
