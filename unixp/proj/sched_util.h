struct FileExplore* find_fe(char* sched_name, int pid);
struct Schedule* find_start_schedule_by_name(char* sched_name);
struct Schedule* find_start_schedule_by_pid(int pid);
void clear_start_file(int line_no);
void print_noti(char* act, char* sched_name, int pid);
int timestamp();
