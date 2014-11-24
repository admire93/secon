#include <stdio.h>

#include "sched_types.h"

/** 다 끝난 스케쥴을 기록합니다.
 *
 * 스케쥴이 종료되었을때 스케쥴을 `done_schedules` 파일에 기록합니다.
 *
 * @param struct Schedule* sched 종료된 스케쥴
 */
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

/** 시작할 스케쥴을 기록합니다.
 *
 * 스케쥴이 시작될때 스케쥴을 `start_schedules` 파일에 기록합니다.
 *
 * @param char* sched_name 시작한 스케쥴의 이름
 * @param int pid 시작된 스케쥴의 프로세스 아이디
 * @param int ts 스케쥴의 시작 시점
 */
void logging_start_schedule(char* sched_name, int pid, int ts) {
  FILE* infile;
  infile = fopen("start_schedules", "a+");

  fprintf(infile, "%s\t%d\t%d\n", sched_name, pid, ts);
  fclose(infile);
}

