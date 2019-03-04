#Compilers
CC          := g++ -std=c++14

#The Target Library

#The Directories, Source, Includes, Objects, Binary and Resources
SRCEXT      := cc

#Flags, Libraries and Includes
CFLAGS      := -O3
LIB         := -lgtest -lpthread 
INCLUDE		:= -I..
LIBDIR		:= -L../lib

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
$(TARGETDIR)/%: %.cc
	$(CC) $(CFLAGS) $(INCLUDE) $< $(LIBDIR) $(LIB) -o $@

.PHONY: directories remake clean cleaner $(BUILDDIR) $(TARGETDIR)