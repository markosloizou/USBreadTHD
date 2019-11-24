CC = g++ # This is the main compiler
# CC = clang --analyze # and comment out the linker last line for sanity
SRCDIR = src
BUILDDIR = build
TARGET = bin/runner

SRCEXT = cc
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS = -g -std=c++17 -O3
LIB =  -pthread -fopenmp -lexplain #-L lib 
INC = -I include `pkg-config --cflags eigen3`


OPENGL_LIBS = -lglut -lGL -lGLU -fopenmp -lexplain
BOOST_LIBS = -lboost 

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo "  $(CC) $(LIB) $^ -o $(TARGET)  "; $(CC) $^ -o $(TARGET)  $(LIB) 

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<  $(OPENGL_LIBS) "; $(CC) $(CFLAGS) $(INC) -c -o $@ $< 

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

# Tests
tester:
	$(CC) $(CFLAGS) $(LIB) test/tester.cc $(INC)  -o bin/tester

# Spikes
ticket:
	$(CC) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY: clean
