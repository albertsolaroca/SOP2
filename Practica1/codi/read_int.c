#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
  int i, fd;

  if (argc != 2) {
    printf("%s <file>\n", argv[0]);
    exit(1);
  }

  fd = open(argv[1], O_RDONLY);

  if (fd < 0) {
    printf("Could not open file\n");
    exit(1);
  }

  while (read(fd, &i, sizeof(int)))
    printf("%d\n", i); 

  close(fd);

  return 0;
}
