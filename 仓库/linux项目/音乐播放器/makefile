APP=mp3
SRC=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRC))
INC = ./
CFLAGS +=-I$(INC) -Wall -O0 -g
all:$(APP)
$(APP):$(OBJS)
	$(CC) $^ -o $@  -g

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	$(RM) -rf *.o $(APP) *.~ *.bak
.PHONY:clean


