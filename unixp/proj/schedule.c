#include "sched_help.h"
#include "sched_command.h"

/** 스케쥴 명령어를 받아서 처리합니다.
 *
 * @param int argc 받아온 argument의 갯수
 * @param char* [] argv 받아온 argument 들
 *
 */
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
      case HELP_REPORT:
        report_sched(argv[2]);
        break;
    }
  } else {
    print_help(HELP_ALL);
  }

  return 1;
}
