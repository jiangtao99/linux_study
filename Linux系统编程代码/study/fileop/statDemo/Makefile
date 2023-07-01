SRCS:=$(wildcard *.c)
EXES:=$(patsubst %.c,%,$(SRCS))
all:$(EXES)
%:%.c
	gcc $^ -o $@
clean:
	$(RM) $(EXES) -Wall