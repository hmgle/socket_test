CFLAGS := -Wall -g -O0

SRCS    :=  $(wildcard *.c)
OBJS    :=  $(SRCS:%.c=%.o)

TARGET := socket_test_server socket_test
.PHONY : clean all

all: $(TARGET)

socket_test_server: $($@:%=%.o)

socket_test: $($@:%=%.o)

clean:
	-rm -f $(TARGET)
	-rm -f $(OBJS)

