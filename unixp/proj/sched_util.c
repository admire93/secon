#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "sched_types.h"

/** `start_schedules` 로부터 특정 스케쥴을 찾는 함수
 *
 * 주어진 스케쥴 이름이나 프로세스 아이디로 시작된 스케쥴중에 하나를 골라냅니다.
 *
 * @param char* sched_name 찾을 스케쥴의 이름. 만약 프로세스 아이디로 찾고싶다면
 *                         `NULL` 로 두면 됩니다.
 * @param int pid 찾을 스케쥴의 프로세스 아이디. 만약 스케쥴 이름으로 찾고싶다면
 *                         `-1` 로 두면 됩니다.
 * @return struct FileExplore* 찾은 스케쥴의 정보를 담고있는 구조체
 *
 */
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

/** 현재 진행중인 스케쥴중의 하나를 이름으로 찾아옵니다
 *
 * @param char* sched_name 찾을 스케쥴의 이름.
 * @return struct Schedule* 찾은 스케쥴의 정보를 담고있는 구조체
 *
 */
struct Schedule* find_start_schedule_by_name(char* sched_name) {
  struct FileExplore* fe;

  fe = find_fe(sched_name, -1);

  if(fe == NULL) return NULL;

  return fe->schedule;
}

/** 현재 진행중인 스케쥴중의 하나를 프로세스 아이디로 찾아옵니다
 *
 * @param int pid 찾을 스케쥴의 프로세스 아이디.
 * @return struct Schedule* 찾은 스케쥴의 정보를 담고있는 구조체
 *
 */
struct Schedule* find_start_schedule_by_pid(int pid) {
  struct FileExplore* fe;

  fe = find_fe(NULL, pid);

  if(fe == NULL) return NULL;

  return fe->schedule;
}

/** 스케쥴이 시작됬는지 종료됬는지 출력하는 함수.
 *
 * @param char* act 스케쥴의 상태 `start` 나 `end`
 * @param char* sched_name 스케쥴의 이름
 * @param int pid 스케쥴의 프로세스 아이디
 *
 */
void print_noti(char* act, char* sched_name, int pid) {
  printf("%s:> Schedule - %s (pid=%d)\n", act, sched_name, pid);
}

/** 현재 시각의 타임스탬프를 초로 반환하는 함수
 *
 * @return int 현재 시각의 타임스탬프를 초로 나타낸 정수값
 *
 */
int timestamp() {

  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (int) tv.tv_sec;
}

/** 파일을 카피하는 함수
 *
 * @param char* source 카피할 파일의 이름
 * @param char* target 카피될 파일의 이름
 * @param int skip 카피할떄 생략하고싶은 라인의 번호
 */
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

/** 주어진 라인 번호에 스케쥴을 생략하는 함수
 *
 * @param int line_no 생략할 라인 번호
 */
void clear_start_file(int line_no) {
  copy("start_schedules", "tmp", -1);
  unlink("start_schedules");
  copy("tmp", "start_schedules", line_no);
  unlink("tmp");
}
