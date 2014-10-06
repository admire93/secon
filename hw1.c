#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFFSIZE 8195

int main(void) {
  int n, fd, fd2, outfd;
  char buf[BUFFSIZE];

  //미리 생성된 inputa, inputb 라는 txt파일 필요
  fd = open("inputa", O_RDONLY);
  fd2 = open("inputb", O_RDONLY);
  outfd = open("result", O_RDWR | O_CREAT, 0777);

  while((n = read(fd, buf, BUFFSIZE)) > 0) {
    if(write(outfd, buf, n) != n) {
      printf("write error \n");
    }
  }

  lseek(outfd, 10, SEEK_SET);
  write(outfd, "fighting!\n", 10);

  while((n = read(fd2, buf, BUFFSIZE)) > 0) {
    if(write(outfd, buf, n) != n) {
      printf("write error \n");
    }
  }

  lseek(outfd, 5, SEEK_CUR);
  write(outfd, "fighting!\n", 10);



  if(n < 0) {
    printf("read error \n");
  }

  close(fd);
  close(fd2);
  close(outfd);
  exit(0);
  return 0;
}
