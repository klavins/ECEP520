#Compilers
CC          := g++ -std=c++11
DGEN        := doxygen

#The Target Binary Program
TARGET      := test

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := .
INCDIR      := .
BUILDDIR    := ./build
TARGETDIR   := ./bin
SRCEXT      := c

#Flags, Libraries and Includes
CFLAGS      := # -fsanitize=address -ggdb
LIB         := -lgtest -lpthread # -lasan
INC         := -I$(INCDIR)
INCDEP      := -I$(INCDIR)

#Files
DGENCONFIG  := docs.config
HEADERS     := $(wildcard *.h)
SOURCES     := $(wildcard *.c)
OBJECTS     := $(patsubst %.c, $(BUILDDIR)/%.o, $(notdir $(SOURCES)))

#Defauilt Make
all: directories $(TARGETDIR)/$(TARGET) 

#Remake
remake: cleaner all

#Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

# Make the documentation
$(DGENCONFIG):
	$(DGEN) -g $(DGENCONFIG)

docs: $(SOURCES) $(HEADERS) $(DGENCONFIG)
	$(DGEN) $(DGENCONFIG)

#Clean only Objects
clean:
	@$(RM) -rf $(BUILDDIR)/*.o

#Full Clean, Objects and Binaries
spotless: clean
	@$(RM) -rf $(TARGETDIR)/$(TARGET) $(DGENCONFIG) *.db
	@$(RM) -rf build bin html latex

#Link
$(TARGETDIR)/$(TARGET): $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

#Compile
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(HEADERS)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.PHONY: directories remake clean cleaner apidocs $(BUILDDIR) $(TARGETDIR)