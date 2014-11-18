CC = gcc
DEPS = sched_help.h
OBJ = schedule.o sched_help.o
RUN_NAME = sched

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<

all: $(OBJ)
	gcc -o $(RUN_NAME) $^

clean:
	rm -rf $(OBJ)
	rm -rf $(RUN_NAME)
