CC = gcc
DEPS = sched_help.h
OBJ = schedule.o sched_help.o sched_types.o sched_log.o sched_util.o sched_command.o
RUN_NAME = sched

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<

all: $(OBJ)
	gcc -o $(RUN_NAME) $^

clean:
	rm -rf $(OBJ)
	rm -rf $(RUN_NAME)
