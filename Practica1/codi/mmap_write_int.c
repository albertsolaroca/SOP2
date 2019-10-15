#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv)
{
  struct stat st;

  int i, fd, len, num_int, *vector;

  if (argc != 3) {
    printf("%s <file> <num_int>\n", argv[0]);
    exit(1);
  }

  num_int = atoi(argv[2]);
  if (num_int <= 0) {
    printf("Not valid number of integers\n");
    exit(1);
  }

  len = num_int * sizeof(int);

  fd = open(argv[1], 
          O_RDWR | O_CREAT | O_TRUNC,
          S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

  if (fd < 0) {
    printf("Could not open file\n");
    exit(1);
  }


  /* Codi preguntat a la practica */

  lseek(fd, len-1, SEEK_SET);
  write(fd, "", 1);

  /* Fi codi */

  vector = mmap(0, len, PROT_WRITE, MAP_SHARED, fd, 0);

  if (vector == MAP_FAILED) {
    printf("Map failed. Errno = %d\n", errno);
    exit(1);
  }

  close(fd);

  printf("File has been mapped to memory\n");
 
  for(i = 0; i < num_int; i++)
    vector[i] = i;

  munmap(vector, len);
}
