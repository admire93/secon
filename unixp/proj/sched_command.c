#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "sched_types.h"
#include "sched_log.h"
#include "sched_util.h"
#include "sched_help.h"

void handl(int signo) {
  FILE* outfile;
  int pid;
  struct Schedule* term_sched;

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
  struct Schedule* done_schedule = find_start_schedule_by_name(sched_name);

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

