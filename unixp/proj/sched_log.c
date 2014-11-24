#include <stdio.h>

#include "sched_types.h"

void logging_done_schedule(struct Schedule* sched) {
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

