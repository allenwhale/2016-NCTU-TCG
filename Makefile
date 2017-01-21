PROC_NUM := $(shell grep -c ^processor /proc/cpuinfo)
THREAD_NUM := $(shell expr $(PROC_NUM) + 1)
THREAD_NUM := $(shell expr $(THREAD_NUM) / 2)
ifeq ($(shell test $(THREAD_NUM) -gt 4; echo $$?), 0)
	THREAD_NUM := 4
endif
SRC := src
INC := inc
CC := g++
CFLAGS := -O3 -std=c++14 -Wall -Wextra -I $(INC)
CFLAGS += -fopenmp -DTHREAD_NUM=$(THREAD_NUM)
EXEC := ./ai
CONVERT := ./convert
OBJS := \
	   $(SRC)/board.o \
	   $(SRC)/table.o \
	   $(SRC)/td.o \
	   $(SRC)/helper.o \
	   $(SRC)/game.o \
	   $(SRC)/ai.o \
	   $(SRC)/ab.o \
	   $(SRC)/expecti.o \
	   $(SRC)/statistics.o \
	   $(SRC)/client.o \
	   $(SRC)/main.o
	   #$(SRC)/tdlambda.o 


$(SRC)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(CONVERT): $(SRC)/convert.cpp
	$(CC) $(CFLAGS) -o $@ $<

all: $(CONVERT) $(EXEC)

clean:
	$(RM) $(EXEC) $(CONVERT) $(OBJS)	

.DEFAULT_GOAL := all
.PHONY: all
