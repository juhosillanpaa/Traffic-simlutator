CC=g++
CPPFLAGS=-c -g -std=c++0x -Wall -Wextra -pedantic 
src = $(wildcard */*.cpp) \
	$(wildcard *.cpp)
obj = $(src:.cpp=.o)
dep = $(obj:.o=.d)  # one dependency file for each source

LDFLAGS = -lm

all: traffic_simulator

debug: CPPFLAGS += -DDEBUG_MSG
debug: all

traffic_simulator: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

-include $(dep)   # include all dep files in the makefile

# rule to generate a dep file by using the C preprocessor
# (see man cpp for details on the -MM and -MT options)
%.d: %.cpp
	@$(CC) $(CPPFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(obj) traffic_simulator

.PHONY: cleandep
cleandep:
	rm -f $(dep)

run: traffic_simulator
	./traffic_simulator