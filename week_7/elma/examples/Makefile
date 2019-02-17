#Compilers
CC          := g++ -std=c++14

#The Target Library

#The Directories, Source, Includes, Objects, Binary and Resources
SRCEXT      := cc

#Flags, Libraries and Includes
CFLAGS      := -fsanitize=address -ggdb
LIB         := -lpthread -lasan -lelma
INCLUDE		:= -I..
LIBDIR		:= -L../lib

#Files

SOURCES     := $(wildcard *.cc)
TARGETS		:= $(patsubst %.cc,%,$(wildcard *.cc))

#Defauilt Make
all: $(TARGETS)

#Clean only Objects
clean:
	@$(RM) -rf $(TARGETS)

#Compile
$(TARGETS): %: %.cc
	$(CC) $(CFLAGS) $(INCLUDE) $<  $(LIBDIR) $(LIB) -o $@

#Compile

.PHONY: directories remake clean cleaner apidocs $(BUILDDIR) $(TARGETDIR)