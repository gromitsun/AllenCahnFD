CC = clang++
CFLAGS = -Wall -O3 -std=c++11
LFLAGS = -framework OpenCL -shared
EXE = libclcommon.so
SRC_FILES = $(wildcard *.cpp)
OBJ_FILES = $(SRC_FILES:.cpp=.o)

$(EXE): $(OBJ_FILES)
	$(CC) -o $(EXE) $(OBJ_FILES) $(LFLAGS)
$(OBJ_FILES): $(SRC_FILES)
	$(CC) -c $(SRC_FILES) $(CFLAGS)
clean:
	rm -fv $(EXE)
	rm -fv *.o
	rm -fv *.gch