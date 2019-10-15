#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
  int i, fd, num_times;
  char s[10];
  char *a = "so2";

  if (argc != 3) {
    printf("%s <file> <number_times>\n", argv[0]);
    exit(1);
  }

  num_times = atoi(argv[2]);
  if (num_times <= 0) {
    printf("Not valid value\n");
    exit(1);
  }
 
  fd = open(argv[1], 
        O_WRONLY | O_CREAT | O_TRUNC, 
        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

  if (fd < 0) {
    printf("Could not open file\n");
    exit(1);
  }

  for(i = 0; i < num_times; i++)
  {
    write(fd, a, strlen(a)); 
    write(fd, &i, sizeof(int));
  }

  printf("Check the file! %d items have been written.\n", num_times);
  printf("Press Enter to close the file\n");
  fgets(s, 10, stdin);

  printf("Closing the file\n");

  close(fd);

  return 0;
}
