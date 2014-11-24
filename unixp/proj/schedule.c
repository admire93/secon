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

FileExplore* find(char* sched_name, int pid) {
  char l[100];
  char* tok;
  int lin_no, i, s;
  FileExplore* fe;
  Schedule* find_sched;
  FILE* rfile = fopen("start_schedules", "r");

  find_sched = (Schedule *) malloc(sizeof(Schedule));
  fe = (FileExplore *) malloc(sizeof(FileExplore));

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
          find_sched->name = (char *) malloc(sizeof(tok));
          strcpy(find_sched->name, sched_name);
        } else if(i == 1) {
          find_sched->pid = atoi(tok);
          if(pid > 0) {
            s = (int) (find_sched->pid == pid);
          }
        } else if(i == 2) {
          find_sched->started_at = atoi(tok);
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

  if(!s) {
    return NULL;
  }

  fe->schedule = find_sched;
  fe->line_no = lin_no;

  return fe;
}
Schedule* find_start_schedule_by_name(char* sched_name) {
  char l[100];
  char* tok;
  int lin_no, i, s;
  Schedule* find_sched;
  FILE* rfile = fopen("start_schedules", "r");

  find_sched = (Schedule *) malloc(sizeof(Schedule));

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
          s = (int) (strcmp(tok, sched_name) == 0);
          find_sched->name = (char *) malloc(sizeof(tok));
          strcpy(find_sched->name, sched_name);
        } else if(i == 1) {
          find_sched->pid = atoi(tok);
        } else if(i == 2) {
          find_sched->started_at = atoi(tok);
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

  if(!s) {
    return NULL;
  }

  return find_sched;
}

Schedule* find_start_schedule_by_pid(int pid) {
  char l[100];
  char* tok;
  int lin_no, i, s;
  Schedule* find_sched;
  FILE* rfile = fopen("start_schedules", "r");

  find_sched = (Schedule *) malloc(sizeof(Schedule));

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
          find_sched->name = tok;
          find_sched->name = (char *) malloc(sizeof(tok));
          strcpy(find_sched->name, tok);
        } else if(i == 1) {
          find_sched->pid = atoi(tok);
          s = (int) (find_sched->pid == pid);
        } else if(i == 2) {
          find_sched->started_at = atoi(tok);
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

  if(!s) {
    return NULL;
  }

  return find_sched;
}

int timestamp() {

  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (int) tv.tv_sec;
}


void logging_done_schedule(char* name, int started_at, int done_at) {
  FILE* logfile;
  logfile = fopen("done_schedules", "a+");

  if(logfile != NULL) {
    fprintf(logfile, "%s\t%d\t%d\n", name, started_at, done_at);
    fclose(logfile);
  }
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
        logging_done_schedule(
            term_sched->name, term_sched->started_at, term_sched->done_at);
      }

      kill(pid, SIGKILL);
      break;
  }
}

void print_noti(char* act, char* sched_name, int pid) {
  printf("%s:> Schedule - %s (pid=%d)\n", act, sched_name, pid);
}

int start_sched(char* sched_name) {
  FILE *infile;
  if(sched_name == NULL) {
    print_help(HELP_START);
    return 0;
  }

  pid_t pid = fork();

  if(pid == 0) {
    print_noti("start", sched_name, getpid());
    infile = fopen("start_schedules", "a+");
    fprintf(infile, "%s\t%d\t%d\n", sched_name, getpid(), timestamp());
    fclose(infile);

    if(signal(SIGTERM, handl) == SIG_ERR) {
      printf("term error");
    }

    while(1) {
      sleep(1);
    }
  }

  return 0;
}

int find_pid_from_file(char* sched_name) {
  char l[100];
  char* tok;
  int lin_no;
  int i;
  int s;
  FILE* rfile = fopen("start_schedules", "r");

  if(rfile == NULL) {
    perror("error infile");
  } else {
    s = 0;
    lin_no = 0;
    while(fgets(l, 100, rfile) != NULL) {

      i = 0;

      tok = strtok(l, "\t");
      while(tok != NULL)
      {
        if(i == 0) {
          s = (int) (strcmp(tok, sched_name) == 0);
        }

        if(i == 1) {
          if(s) {
            return atoi(tok);
          }
        }

        tok = strtok(NULL, "\t");
        i += 1;
      }

      lin_no += 1;
    }
  }

  return -1;
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

  Schedule *d;

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
