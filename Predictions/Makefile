GCC = g++
LD = g++
CXXFLAGS = $(shell root-config --cflags) 
LDFLAGS=-g $(shell root-config --ldflags)
LIBS = $(shell root-config --libs)
SRCS = ComputeVdepEvolution.C
OBJS = ComputeVdepEvolution.o
PROGRAM = Exec_ComputeVdepEvolution


all:	compile link
perlayer:	compile link_perlayer

compile:	$(OBJS)

link:	$(OBJS)
	@echo "Linking object files ..."
	$(LD) $(LDFLAGS) $(LIBS) $(OBJS) -o $(PROGRAM)

link_perlayer: ComputeVdepEvolution_perlayer.C
	$(GCC) $(CXXFLAGS) -c -o ComputeVdepEvolution_perlayer.o ComputeVdepEvolution_perlayer.C
	@echo "Linking object files ..."
	$(LD) $(LDFLAGS) $(LIBS) ComputeVdepEvolution_perlayer.o -o Exec_ComputeVdepEvolution_perlayer

clean:; @rm -f $(OBJS) $(PROGRAM) *~ */*~

