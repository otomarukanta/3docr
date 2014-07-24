COMPILER = g++
	CFLAGS = -std=c++0x -Wextra -g -MMD -MP -O3
	LDFLAGS = -lpthread -lopencv_highgui
	LIBS = 
	INCLUDE = -I../include
	TARGET = ../bin/test
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
