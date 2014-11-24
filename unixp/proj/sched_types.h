struct Schedule {
  char* name;
  int pid;
  int started_at;
  int done_at;
};

struct FileExplore {
  int line_no;
  struct Schedule* schedule;
};
