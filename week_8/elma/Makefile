#Compilers
CC          := g++ -std=c++14
DGEN        := doxygen

#The Target Binary Program
TARGET      := libelma.a

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := .
INCDIR      := .
BUILDDIR    := ./build
TARGETDIR   := ./lib
SRCEXT      := cc

#Flags, Libraries and Includes
CFLAGS      := -fsanitize=address -ggdb
LIB         := -lgtest -lpthread -lasan
INC         := -I$(INCDIR)
INCDEP      := -I$(INCDIR)

#Files
DGENCONFIG  := docs.config
HEADERS     := $(wildcard *.h)
SOURCES     := $(wildcard *.cc)
OBJECTS     := $(patsubst %.cc, $(BUILDDIR)/%.o, $(notdir $(SOURCES)))

#Defauilt Make
all: directories $(TARGETDIR)/$(TARGET) tests example

#Remake
remake: cleaner all

#Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

tests:
	cd test && $(MAKE)

example:
	cd examples && $(MAKE)

docs: $(SOURCES) $(HEADERS)
	$(DGEN) $(DGENCONFIG)

publish: 
	rm -r ../../docs/
	cp -r html ../../docs

#Clean only Objects
clean:
	@$(RM) -rf $(BUILDDIR)/*.o
	cd test && $(MAKE) clean
	cd examples && $(MAKE) clean

#Full Clean, Objects and Binaries
spotless: clean
	@$(RM) -rf $(TARGETDIR)/$(TARGET) *.db
	@$(RM) -rf build lib html latex
	cd test && $(MAKE) spotless

#Link
$(TARGETDIR)/$(TARGET): $(OBJECTS) $(HEADERS)
	ar rvs $@ $(OBJECTS)
	
#Compile
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(HEADERS)
	$(CC) $(CFLAGS) $(INC) -c -fPIC -o $@ $<

.PHONY: directories remake clean cleaner apidocs $(BUILDDIR) $(TARGETDIR)