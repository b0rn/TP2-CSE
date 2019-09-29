.SUFFIXES:

CC=gcc

# uncomment to compile in 32bits mode (require gcc-*-multilib packages
# on Debian/Ubuntu)
#HOST32= -m32

CFLAGS= $(HOST32) -Wall -Werror -std=c99 -g -DMEMORY_SIZE=128000
CFLAGS+= -DDEBUG
LDFLAGS= $(HOST32)
INCLUDE=./include
SRC=./src
TEST_SRC=./tests
TARGET=./bin
OBJ=./obj
LIB=./lib

TESTS+=test_init test_base test_cheese test_fusion
PROGRAMS=memshell

.PHONY: clean all test test_ls

all: $(PROGRAMS) $(TESTS)

test: $(TESTS)
	for file in $(TESTS);do $(TARGET)/$$file;done

# dépendances des binaires
DEPS=$(OBJ)/mem.o $(OBJ)/common.o

%: $(DEPS) $(OBJ)/%.o
		$(CC) -o $(TARGET)/$@ $^ $(CFLAGS)

$(OBJ)/test_%.o: $(TEST_SRC)/test_%.c
		$(CC) -I$(INCLUDE)/ -o $@ -c $< $(CFLAGS)

$(OBJ)/%.o: $(SRC)/%.c
		$(CC) -I$(INCLUDE)/ -o $@ -c $< $(CFLAGS)

# test avec des programmes existant
# création d'une librairie partagée
libmalloc.so: malloc_stub.o mem.o
	$(CC) -shared -Wl,-soname,$(OBJ)/$@ $^ -o $(LIB)/$@

#test avec ls
test_ls: libmalloc.so
	LD_PRELOAD=$(LIB)/libmalloc.so ls

# nettoyage
clean:
	rm -f $(TARGET)/* $(OBJ)/* $(LIB)/*
