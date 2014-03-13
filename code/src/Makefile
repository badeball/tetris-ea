IDIR =../include
CC=mpicc
CFLAGS=-std=gnu99 -I$(IDIR) -O1 -g

ODIR=obj
LDIR=../lib
BINDIR=../bin

LIBS=-lm

EXECUTABLES=tetris random_board analyze_board play

_DEPS = board.h feature_functions.h feature_helpers.h genotype.h options.h phenotype.h population.h random.h selection.h structs.h tetromino.h threading.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = board.o feature_functions.o feature_helpers.o genotype.o options.o phenotype.o population.o random.o selection.o tetromino.o threading.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(EXECUTABLES)

$(EXECUTABLES): $(OBJ)
	$(CC) -c -o obj/$@.o $@.c $(CFLAGS)
	$(CC) -o ../bin/$@ obj/$@.o $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BINDIR)/*
