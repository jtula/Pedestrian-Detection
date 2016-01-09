INC_FLAGS = -I/usr/local/include/opencv2
LD_FLAGS = -L/usr/lib -lpthread -ldl -lm -std=c++11 -std=gnu++0x -std=c++0x -lopencv_core -lopencv_ml -lopencv_highgui -lopencv_imgproc -lopencv_video -lopencv_objdetect
DEBUG_FLAG = -g
SVMLIGHT_CODE = src/svmlight
CC = g++
GC = gcc
CXXFLAGS = $(DEBUG_FLAG) -Wall -O2
CFLAGS = -Wall -O2
COMPILE = $(CC) $(INC_FLAGS) $(FLAGS) -c
OBJFILES = $(patsubst %.cpp,%.o,$(wildcard src/*.cpp))

# defining implicit rules
%.o : %.cpp
	$(CC) -c $(INC_FLAGS) $< -o $@ $(LD_FLAGS)

all: calculator

calculator: $(OBJFILES)
	$(CC) $(INC_FLAGS) -o pedestre $(OBJFILES) $(LD_FLAGS)



clean:
	rm -f Makefile~ component  *.c~ *.cpp~ *.h~ $(OBJFILES)

var:
	@echo $(PKG_CONFIG_PATH)
	@echo $(OBJFILES)
	@echo $(INC_FLAGS)
	@echo $(COMPILE)
