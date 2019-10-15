#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  FILE *fp;
  int i, num_int;

  if (argc != 3) {
    printf("%s <file> <number_ints>\n", argv[0]);
    exit(1);
  }

  num_int = atoi(argv[2]);
  if (num_int <= 0) {
    printf("Not valid number of integers\n");
    exit(1);
  }

  fp = fopen(argv[1], "w");
  if (!fp) {
    printf("Could not open file\n");
    exit(1);
  }
  
  for(i = 0; i < num_int; i++)
    fprintf(fp, "%d\n", i);

  printf("Check the file! %d integers have been written.\n", num_int);
  printf("Closing the file\n");
  
  fclose(fp);

  return 0;
}
