#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HELP_ALL -1
#define HELP_START 0
#define HELP_DONE 1
#define HELP_LIST 2
#define HELP_REPORT 3

/** 파일을 출력합니다.
 *
 * 인자로 파일이름을 받아서 파일을 출력합니다
 *
 * @param char* fname 출력할 파일이름
 */
void print_file(char *fname) {
  FILE* help_file = fopen(fname, "r");
  char m[80];
  while(fgets(m, 80, help_file)) {
    printf("%s", m);
  }
}

/** 도움말을 출력합니다
 *
 * 인자로 받은 도움말 종류중 하나를 출력합니다
 *
 * @param int help 출력할 도움말 `HELP_ALL` , `HELP_START`, `HELP_DONE` ,
 *                 `HELP_LIST` , `HELP_REPORT` 중 하나
 */
void print_help(int help) {
  switch(help) {
    case HELP_ALL:
      print_file("./docs/help.md");
      break;
    case HELP_START:
      print_file("./docs/help_start.md");
      break;
    case HELP_DONE:
      print_file("./docs/help_done.md");
      break;
    default:
      printf("unrecognize option for HELP");
  }
}

/** 도움말을 출력할때 필요한 도움말의 종류를 가져옵니다.
 *
 * @param char* help_str 도움말의 종류를 스트링으로 표현한 형태
 * @return int 도움말 종류 `HELP_ALL` , `HELP_START`, `HELP_DONE` ,
 *             `HELP_LIST` , `HELP_REPORT` 중 하나
 */
int get_help(char *help_str) {
  int r = HELP_ALL;

  if(strcmp(help_str, "start") == 0) {
     r = HELP_START;
  } else if(strcmp(help_str, "done") == 0) {
    r = HELP_DONE;
  } else if(strcmp(help_str, "list") == 0) {
    r = HELP_LIST;
  } else if(strcmp(help_str, "report") == 0) {
    r = HELP_REPORT;
  }

  return r;
}
