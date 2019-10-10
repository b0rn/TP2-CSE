.SUFFIXES:

CC=gcc

# uncomment to compile in 32bits mode (require gcc-*-multilib packages
# on Debian/Ubuntu)
#HOST32= -m32

CFLAGS= $(HOST32) -Wall -Werror -std=c99 -g -DMEMORY_SIZE=4500
CFLAGS+= -DDEBUG
LDFLAGS= $(HOST32)
INCLUDE=./include
SRC=./src
TEST_SRC=./tests
TARGET=./bin
OBJ=./obj
LIB=./lib

TESTS+=test_init test_base test_cheese test_fusion test_realloc
PROGRAMS=memshell
LIBS=$(LIB)/libmalloc.so

.PHONY: clean all test test_ls


all: $(PROGRAMS) $(TESTS) $(LIBS)

test: $(TESTS)
	for file in $(TESTS);do $(TARGET)/$$file;done

# dépendances des binaires
DEPS=$(OBJ)/mem.o $(OBJ)/common.o

%: $(DEPS) $(OBJ)/%.o | $(TARGET)
		$(CC) -g -o $(TARGET)/$@ $^ $(CFLAGS)

$(OBJ)/test_%.o: $(TEST_SRC)/test_%.c | $(OBJ)
		$(CC) -I$(INCLUDE)/ -o $@ -c $< $(CFLAGS)

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
		$(CC) -fPIC -I$(INCLUDE)/ -o $@ -c $< $(CFLAGS)

# test avec des programmes existant
# création d'une librairie partagée
$(LIB)/libmalloc.so: $(OBJ)/malloc_stub.o $(OBJ)/mem.o $(OBJ)/common.o | $(LIB)
	$(CC)  -shared $^ -Wl,-soname,libmalloc.so -o $@

#test avec ls
test_ls: $(LIB)/libmalloc.so
	LD_PRELOAD=$(LIB)/libmalloc.so ls

$(TARGET):
	mkdir $(TARGET)
$(OBJ):
	mkdir $(OBJ)
$(LIB):
	mkdir $(LIB)

# nettoyage
clean:
	rm -f $(TARGET)/* $(OBJ)/* $(LIB)/*
