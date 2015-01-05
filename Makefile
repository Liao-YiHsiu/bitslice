.PHONY: clean 

LDFLAGS+= # link to math library

all: bitslice run test
	./test


o3: CPPFLAGS+=-o3
o3: all

debug: CPPFLAGS+=-g
debug: all

# type make/make all to compile test_hmm
run: bitslice
	./bitslice sboxBit.h sboxArr.h

clean:
	$(RM) bitslice test   # type make clean to remove the compiled file
