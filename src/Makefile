COMPILER = g++
	CFLAGS = -std=c++0x -fopenmp -Wextra -g -MMD -MP -O3
	LDFLAGS = -lm -lpthread -lopencv_highgui -lopencv_core -lopencv_imgproc
	LIBS = 
	INCLUDE = -I/usr/local/include/eigen3 -I../include
	TARGET = ../bin/3docr
	OBJDIR = ./obj
ifeq "$(strip $(OBJDIR))" ""
	OBJDIR = .
endif
SOURCES = $(wildcard *.cpp)
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))
	DEPENDS = $(OBJECTS:.o=.d)
	 
$(TARGET): $(OBJECTS) $(LIBS)
	$(COMPILER) -o $@ $^ $(LDFLAGS)
	 
$(OBJDIR)/%.o: %.cpp
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)
	$(COMPILER) $(CFLAGS) $(INCLUDE) -o $@ -c $<
	 
all: clean $(TARGET)
	 
clean:
	rm -f $(OBJECTS) $(DEPENDS) $(TARGET)
	@rmdir --ignore-fail-on-non-empty `readlink -f $(OBJDIR)`
	 
#	 -include $(DEPENDS)
run:
	./$(TARGET)
