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

  if (argc != 2) {
    printf("%s <file>\n", argv[0]);
    exit(1);
  }
  
  stat(argv[1], &st);
  len = st.st_size;
  num_int = len/sizeof(int);
  
  fd = open(argv[1], O_RDONLY);
  
  vector = mmap(0, len, PROT_READ, MAP_PRIVATE, fd, 0);

  if (vector == MAP_FAILED) {
    printf("Map failed. Errno = %d\n", errno);
    exit(1);
  }
  
  close(fd);

  printf("File has been mapped to memory\n");

  for(i = 0; i < num_int; i++)
    printf("%d\n", vector[i]);
  
  munmap(vector, len);
}
