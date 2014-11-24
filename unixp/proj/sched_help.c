#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HELP_ALL -1
#define HELP_START 0
#define HELP_DONE 1
#define HELP_LIST 2
#define HELP_REPORT 3

void print_file(char *fname) {
  FILE* help_file = fopen(fname, "r");
  char m[80];
  while(fgets(m, 80, help_file)) {
    printf("%s", m);
  }
}

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
