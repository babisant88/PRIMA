#***********************************************#
# Define macro "PRINTING" as follows:		#
# NONE    - printing is disabled		#
# COMPACT - for basic information printing	#
# FULL    - thorough printing			#
#***********************************************#
CC	= g++

DATE	= `date +%d%m%Y-%H%M`

#MACROFLAGS += -DNONE=0 -DCOMPACT=1 -DFULL=2
#MACROFLAGS += -DPRINTING=FULL

ifeq ($(CC),icc)
	CFLAGS	= -O3
else ifeq ($(CC),g++)
	CFLAGS	= -O3 -Wno-unused-result
endif

#CFLAGS	+= $(MACROFLAGS)
CFLAGS	+= -fopenmp
#CFLAGS	+= -g
#CFLAGS	+= -std=c++0x
#CFLAGS	+= -static
#CFLAGS	+= -pg
#CFLAGS	+= -Wall

ROOT		= $(CURDIR)
SRCDIR		= $(ROOT)/src
HPPDIR		= $(ROOT)/hpp
OBJDIR		= $(ROOT)/obj

CHECK_CC := $(shell which $(CC))
ifeq ($(CHECK_CC),)
	$(error No $(CC) found!)
endif

.PHONY: all clean dirs

all: dirs Prima

dirs:
	mkdir -p $(OBJDIR)

Prima: $(OBJDIR)/load_input.o $(OBJDIR)/main.o $(OBJDIR)/Prima.o $(OBJDIR)/sim.o $(OBJDIR)/sys.o
	$(CC) $(OBJDIR)/load_input.o $(OBJDIR)/main.o $(OBJDIR)/Prima.o $(OBJDIR)/sim.o $(OBJDIR)/sys.o $(CFLAGS) -o Prima

$(OBJDIR)/load_input.o: $(SRCDIR)/load_input.cpp
	$(CC) -c $(SRCDIR)/load_input.cpp $(CFLAGS) -o $(OBJDIR)/load_input.o 
	
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp
	$(CC) -c $(SRCDIR)/main.cpp $(CFLAGS) -o $(OBJDIR)/main.o 

$(OBJDIR)/Prima.o: $(SRCDIR)/Prima.cpp
	$(CC) -c $(SRCDIR)/Prima.cpp $(CFLAGS) -o $(OBJDIR)/Prima.o

$(OBJDIR)/sim.o: $(SRCDIR)/sim.cpp
	$(CC) -c $(SRCDIR)/sim.cpp $(CFLAGS) -o $(OBJDIR)/sim.o

$(OBJDIR)/sys.o: $(SRCDIR)/sys.cpp
	$(CC) -c $(SRCDIR)/sys.cpp $(CFLAGS) -o $(OBJDIR)/sys.o

$(OBJDIR)/aux.o: $(SRCDIR)/aux.cpp
	$(CC) -c $(SRCDIR)/aux.cpp $(CFLAGS) -o $(OBJDIR)/aux.o

clean:
	rm -rf *~ $(SRCDIR)/*~ $(OBJDIR) Prima
