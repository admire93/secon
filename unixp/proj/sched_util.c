#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "sched_types.h"

struct FileExplore* find_fe(char* sched_name, int pid) {
  char l[100];
  char* tok;
  int lin_no, i, s;
  struct FileExplore* fe;
  FILE* rfile = fopen("start_schedules", "r");

  fe = (struct FileExplore *) malloc(sizeof(struct FileExplore));
  fe->schedule = (struct Schedule *) malloc(sizeof(struct Schedule));
  fe->schedule->name = NULL;

  if(rfile == NULL) {
    perror("error infile");
  } else {
    lin_no = 0;
    s = 0;
    while(fgets(l, 100, rfile) != NULL) {
      i = 0;

      tok = strtok(l, "\t");
      while(tok != NULL)
      {
        if(i == 0) {
          if(sched_name != NULL) {
            s = (int) (strcmp(tok, sched_name) == 0);
          }
          fe->schedule->name = (char *) malloc(sizeof(tok));
          strcpy(fe->schedule->name, tok);
        } else if(i == 1) {
          fe->schedule->pid = atoi(tok);
          if(pid > 0) {
            s = (int) (fe->schedule->pid == pid);
          }
        } else if(i == 2) {
          fe->schedule->started_at = atoi(tok);
        }

        tok = strtok(NULL, "\t");
        i += 1;
      }

      if(s) {
        break;
      }

      lin_no += 1;
    }
  }

  fclose(rfile);
  if(!s) {
    return NULL;
  }

  fe->line_no = lin_no;
  return fe;
}

struct Schedule* find_start_schedule_by_name(char* sched_name) {
  struct FileExplore* fe;

  fe = find_fe(sched_name, -1);

  if(fe == NULL) return NULL;

  return fe->schedule;
}

struct Schedule* find_start_schedule_by_pid(int pid) {
  struct FileExplore* fe;

  fe = find_fe(NULL, pid);

  if(fe == NULL) return NULL;

  return fe->schedule;
}

void print_noti(char* act, char* sched_name, int pid) {
  printf("%s:> Schedule - %s (pid=%d)\n", act, sched_name, pid);
}

int timestamp() {

  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (int) tv.tv_sec;
}

void copy(char* source, char* target, int skip) {
  char l[100];
  FILE* sourcef = fopen(source, "r");
  FILE* targetf = fopen(target, "w+");
  int i = 0;

  while(fgets(l, 100, sourcef) != NULL) {
    if(skip >= 0 && i == skip) {
      i += 1;
      continue;
    }
    fprintf(targetf, "%s", l);
    i += 1;
  }

  fclose(targetf);
  fclose(sourcef);
}

void clear_start_file(int line_no) {
  copy("start_schedules", "tmp", -1);
  unlink("start_schedules");
  copy("tmp", "start_schedules", line_no);
  unlink("tmp");
}
