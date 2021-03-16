TESTFILES := $(wildcard *.crona)
TESTS := $(TESTFILES:.crona=.test)

.PHONY: all

all: $(TESTS)

%.test:
	@echo "Testing $*.crona"
	@touch $*.out #Creates out file to diff in case of no output
	@touch $*.err #The @ means don't show the command being invoked
	@../cronac $*.crona -n $*.out 2> $*.err ;\
	PROG_EXIT_CODE=$$?;\
	echo "diff output...";\
	OUT_EXIT_CODE=$$?;\
	echo "diff error...";\
	diff $*.err $*.err.expected;\
	ERR_EXIT_CODE=$$?;\
	exit $$PROG_EXIT_CODE && $$OUT_EXIT_CODE && $$ERR_EXIT_CODE

clean:
	rm -f *.out *.err
