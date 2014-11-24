#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

#include "sched_help.h"

typedef struct Schedule {
  char* name;
  int pid;
  int started_at;
  int done_at;
} Schedule;

typedef struct FileExplore {
  int line_no;
  Schedule* schedule;
} FileExplore;

FileExplore* find_fe(char* sched_name, int pid) {
  char l[100];
  char* tok;
  int lin_no, i, s;
  FileExplore* fe;
  FILE* rfile = fopen("start_schedules", "r");

  fe = (FileExplore *) malloc(sizeof(FileExplore));
  fe->schedule = (Schedule *) malloc(sizeof(Schedule));
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

Schedule* find_start_schedule_by_name(char* sched_name) {
  FileExplore* fe;

  fe = find_fe(sched_name, -1);

  if(fe == NULL) return NULL;

  return fe->schedule;
}

Schedule* find_start_schedule_by_pid(int pid) {
  FileExplore* fe;

  fe = find_fe(NULL, pid);

  if(fe == NULL) return NULL;

  return fe->schedule;
}

int timestamp() {

  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (int) tv.tv_sec;
}


void logging_done_schedule(Schedule* sched) {
  FILE* logfile;
  logfile = fopen("done_schedules", "a+");

  if(logfile != NULL) {
    fprintf(
        logfile, "%s\t%d\t%d\n",
        sched->name, sched->started_at, sched->done_at);
    fclose(logfile);
  }
}

void logging_start_schedule(char* sched_name, int pid, int ts) {
  FILE* infile;
  infile = fopen("start_schedules", "a+");
  fprintf(infile, "%s\t%d\t%d\n", sched_name, pid, ts);
  fclose(infile);
}

void handl(int signo) {
  FILE* outfile;
  int pid;
  Schedule* term_sched;

  switch(signo) {
    case SIGTERM:
      pid = getpid();
      term_sched = find_start_schedule_by_pid(pid);
      if(term_sched != NULL) {
        term_sched->done_at = timestamp();
        logging_done_schedule(term_sched);
      }

      kill(pid, SIGKILL);
      break;
  }
}

void print_noti(char* act, char* sched_name, int pid) {
  printf("%s:> Schedule - %s (pid=%d)\n", act, sched_name, pid);
}

int start_sched(char* sched_name) {
  if(sched_name == NULL) {
    print_help(HELP_START);
    return 0;
  }

  pid_t pid = fork();

  if(pid == 0) {
    print_noti("start", sched_name, getpid());
    logging_start_schedule(sched_name, getpid(), timestamp());

    if(signal(SIGTERM, handl) == SIG_ERR) {
      printf("term error");
    }

    while(1) {
      sleep(1);
    }
  }

  return 0;
}

int done_sched(char* sched_name) {
  Schedule* done_schedule = find_start_schedule_by_name(sched_name);

  if(sched_name == NULL) {
    print_help(HELP_DONE);
    return 0;
  }

  if(done_schedule != NULL) {
    print_noti("done", done_schedule->name, done_schedule->pid);

    kill(done_schedule->pid, SIGTERM);
  }

  return 1;
}

int list_sched(char* arg) {
  printf("Current schedules. ------- \n");
  return 1;
}

int main(int argc, char* argv[]) {
  int help;

  if(argc == 1) {
    print_help(HELP_ALL);
    return 0;
  }

  help = get_help(argv[1]);

  if(help >= 0) {
    switch(help) {
      case HELP_START:
        start_sched(argv[2]);
        break;
      case HELP_DONE:
        done_sched(argv[2]);
        break;
      case HELP_LIST:
        list_sched(argv[2]);
        break;
    }
  } else {
    print_help(HELP_ALL);
  }

  return 1;
}
