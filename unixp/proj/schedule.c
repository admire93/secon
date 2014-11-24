#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

#include "sched_help.h"

int timestamp() {

  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (int) tv.tv_sec;
}

FILE *infile;

void handl(int signo) {
  switch(signo) {
    case SIGTERM:
      printf("SIGTERM");
      break;
  }
}

int start_sched(char* sched_name) {
  if(sched_name == NULL) {
    print_help(HELP_START);
    return 0;
  }

  pid_t pid = fork();

  if(pid == 0) {
    infile = fopen("start_schedules", "a+");
    printf("schedule:> %s started. (%d) \n", sched_name, getpid());
    fprintf(infile, "%s\t%d\t%d\n", sched_name, getpid(), timestamp());
    fclose(infile);

    while (1) {
      sleep(2);

      kill(getppid(), SIGCONT);

      pause();
    }
  }

  return 1;
}

int done_sched(char* sched_name) {
  char l[100];
  char* tok;
  int lin_no;
  int i;
  int s;

  if(sched_name == NULL) {
    print_help(HELP_DONE);
    return 0;
  }

  infile = fopen("start_schedules", "r");

  if(infile == NULL) {
    perror("error infile");
  } else {
    s = 0;
    lin_no = 0;
    while(fgets(l, 100, infile) != NULL) {

      i = 0;

      tok = strtok(l, "\t");
      while(tok != NULL)
      {
        if(i == 0) {
          s = (int) (strcmp(tok, sched_name) == 0);
        }

        if(i == 1) {
          if(s) {
            printf("done:> schedule(pid=%s)\n", tok);
            kill(atoi(tok), SIGTERM);
          }
          break;
        }

        tok = strtok(NULL, "\t");
        i += 1;
      }

      lin_no += 1;
    }
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
  /*
  pid_t pid = fork();

  if(pid == 0) {
    signal(SIGCONT, noth);
    pause();
    infile = fopen("pid_log.txt", "a+");
    printf("Child Starting (%d)\n",getpid());
    fprintf(infile,"Child Starting (%d)\n",getpid());
    fclose(infile);
    while (1) {
      sleep(2);

      kill(getppid(), SIGCONT);

      signal(SIGCONT, writec);

      pause();
    }
  } else {
    infile = fopen("pid_log.txt","a+");
    printf("Parent Starting (%d)\n",getpid());
    fprintf(infile,"Parent Starting (%d)\n",getpid());
    fclose(infile);
    kill(pid, SIGCONT);
    signal(SIGCONT, writep);
    pause();
    while (1) {
      sleep(2);

      kill(pid, SIGCONT);

      signal(SIGCONT, writep);

      pause();
    }
  }
  */

  return 1;
}
