#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "sched_types.h"
#include "sched_log.h"
#include "sched_util.h"
#include "sched_help.h"

/** 시그널을 핸들링하는 핸들러.
 *
 * `SIGTERM` 이 들어오면 현재 프로세스의 스케쥴 이름을 찾아와서 기록하고,
 * 진행중인 스케쥴에서 제외시킵니다. 마지막에 프로세스를 죽입니다.
 *
 * @param int signo 프로세스에 전해지는 signal SIGTERM, SIGCONT 같은
 *            signal들이 들어올수있습니다.
 */
void handl(int signo) {
  FILE* outfile;
  int pid;
  struct Schedule* term_sched;
  struct FileExplore* fe;

  switch(signo) {
    case SIGTERM:
      pid = getpid();
      term_sched = find_start_schedule_by_pid(pid);
      if(term_sched != NULL) {
        term_sched->done_at = timestamp();
        logging_done_schedule(term_sched);
        fe = find_fe(term_sched->name, -1);
        clear_start_file(fe->line_no);
      }

      kill(pid, SIGKILL);
      break;
  }
}

/** start 할때 실행되는 함수
 *
 * 인자로 스케쥴의 이름을 받아서 스케쥴 이름에 해당하는 스케쥴을 자식 프로세스로
 * 생성합니다. 시그널 핸들러로 `handl` 을 등록합니다. SIGTERM 일때 핸들러를
 * 호출합니다.
 *
 * @param char* sched_name 실행시킬 스케쥴 이름
 */
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

/** done 할때 실행되는 함수
 *
 * 인자로 스케쥴의 이름을 받아서 스케쥴 이름에 해당하는 스케쥴을 종료합니다.
 * SIGTERM을 해당 프로세스에 시그널로 전달합니다.
 *
 * @param char* sched_name 종료시킬 스케쥴 이름
 */
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

/** list 할때 실행되는 함수
 *
 * 현재 실행중인 스케쥴들을 리스팅합니다.
 *
 * @param char* arg 스케쥴 이름
 */
int list_sched(char* arg) {
  printf("Current schedules. \n");
  printf("======================================\n");
  printf("       schedule       pid        time \n");
  printf("======================================\n");
  char l[100];
  char* tok;
  FILE* rfile = fopen("start_schedules", "r");

  int i;
  if(rfile == NULL) {
    perror("schedule isn't started yet");
  } else {
    while(fgets(l, 100, rfile) != NULL) {
      i = 0;
      tok = strtok(l, "\t");
      while(tok != NULL)
      {
        if(i == 0) {
          printf("%15s\t", tok);
        } else if(i == 1) {
          printf("%10s", tok);
        } else if(i == 2) {
          printf("%10ds\n", timestamp() - atoi(tok));
        }

        tok = strtok(NULL, "\t");
        i += 1;
      }
    }
  }

  fclose(rfile);
  return 1;
}

/** 종료된 스케줄중에 가장 오래걸린 스케쥴의 시간을 찾습니다.
 *
 * @return int 가장 오래걸린 스케쥴의 시간
 *
 */
int calc_max_time() {
  int i;
  int cur;
  int max = 0;
  char* tok;
  char l[100];
  FILE* rfile = fopen("done_schedules", "r");

  if(rfile == NULL) {
    perror("schedule isn't done yet");
  } else {
    while(fgets(l, 100, rfile) != NULL) {
      tok = strtok(l, "\t");
      i = 0;
      while(tok != NULL)
      {
        if(i == 1) {
          cur = atoi(tok);
        } else if(i == 2){
          cur = atoi(tok) - cur;
          if(max < cur) {
            max = cur;
          }
        }
        tok = strtok(NULL, "\t");
        i +=1;
      }
    }
  }

  fclose(rfile);
  return max;
}


/** report 할때 실행되는 함수
 *
 * 종료된 스케쥴들을 리스팅합니다. 시간에 따라서 막대바의 크기 또한 바뀝니다.
 *
 * @param char* arg 스케쥴 이름
 */
int report_sched(char *arg) {
  char l[100];
  int i, j, cur;
  char* tok;
  char c = '-';
  int max_sec = calc_max_time();
  int big_num = 50;
  float coef = big_num / (float)max_sec;
  printf("Report accomplished schedule\n");
  printf("============================\n");

  FILE* rfile = fopen("done_schedules", "r");

  if(rfile == NULL) {
    perror("schedule isn't done yet");
  } else {
    while(fgets(l, 100, rfile) != NULL) {
      tok = strtok(l, "\t");
      i = 0;
      while(tok != NULL)
      {
        if(i == 0) {
          printf("%s run ", tok);
        } else if(i == 1) {
          cur = atoi(tok);
        } else if(i == 2){
          cur = atoi(tok) - cur;
          printf(" %d sec. \n[", cur);
          for(j = 0; j < ceil(coef * cur); j++) {
            printf("%c", c);
          }
          printf("]");
        }
        tok = strtok(NULL, "\t");
        i += 1;
      }
      printf("\n\n");
    }
  }

  fclose(rfile);
  return 1;
}
