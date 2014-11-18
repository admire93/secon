#include <stdio.h>
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

void noth() {

}

void writec() {
  infile = fopen("pid_log.txt","a+");
  fprintf(infile, "Child (%d)\n", getpid());
  printf("Child (%d)\n", getpid());
  fclose(infile);
}

void writep() {
  infile = fopen("pid_log.txt","a+");
  fprintf(infile, "Parent (%d)\n", getpid());
  printf("Parent (%d)\n", getpid());
  fclose(infile);
}

int start_sched(char* sched_name) {
  printf("start_sched called \n");
  if(sched_name == NULL) {
    print_help(HELP_START);
    return 0;
  }

  return 1;
}

int done_sched(char* sched_name) {
  if(sched_name == NULL) {
    print_help(HELP_DONE);
    return 0;
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
