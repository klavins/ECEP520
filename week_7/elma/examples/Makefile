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
ELMALIB		:= ../lib/libelma.a

#Files
TARGETDIR	 := ./bin
SOURCES      := $(wildcard *.cc)
TARGETS		 := $(patsubst %.cc,%,$(wildcard *.cc))
FULL_TARGETS := $(addprefix $(TARGETDIR)/, $(TARGETS))

#Default Make
all: dirs $(FULL_TARGETS)

dirs: $(TARGETDIR)
	@mkdir -p $(TARGETDIR)

#Clean only Objects
clean:
	@$(RM) -rf $(TARGETDIR)

# Compile
$(TARGETDIR)/%: %.cc $(ELMALIB)
	$(CC) $(CFLAGS) $(INCLUDE) $< $(LIBDIR) $(LIB) -o $@

# bin/basic: basic.cc
# 	$(CC) $(CFLAGS) $(INCLUDE) $<  $(LIBDIR) $(LIB) -o $@

.PHONY: directories remake clean cleaner $(BUILDDIR) $(TARGETDIR)