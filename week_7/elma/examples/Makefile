#Compilers
CC          := g++ -std=c++14

#The Target Library

#The Directories, Source, Includes, Objects, Binary and Resources
SRCEXT      := cc

#Flags, Libraries and Includes
CFLAGS      := -fsanitize=address -ggdb
LIB         := -lgtest -lpthread -lasan -lelma
INCLUDE		:= -I..
LIBDIR		:= -L../lib

#Files
TARGETDIR	:= ./bin
SOURCES     := $(wildcard *.cc)
TARGETS		:= $(patsubst %.cc,%,$(wildcard *.cc))

#Default Make
all: dirs $(TARGETS) 

dirs:
	@mkdir -p $(TARGETDIR)

#Clean only Objects
clean:
	@$(RM) -rf $(TARGETS)

#Compile
$(TARGETS): %: %.cc dirs
	$(CC) $(CFLAGS) $(INCLUDE) $<  $(LIBDIR) $(LIB) -o $(TARGETDIR)/$@

#Compile

.PHONY: directories remake clean cleaner apidocs $(BUILDDIR) $(TARGETDIR)