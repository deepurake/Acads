OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall $(OMPFLAGS)
LDFLAGS = -g -Wall 
LDLIBS = $(OMPLIBS)

MPICC = /opt/openmpi-1.4.3-gcc44/bin/mpicc
MPICH = /nethome/pw19/hpcx-v1.2.0-255-gcc-MLNX_OFED_LINUX-2.3-1.5.0-redhat6.5/ompi-mellanox-v1.8#/opt/openmpi-1.4.3-gcc44/bin#/usr/lib64/openmpi/1.4.3-gcc
CFLAGS = -I$(MPICH)/include

all: mpiBarrier dissemination Tournament hybrid senseReversal MCS OMPBarrier
	
mpiBarrier: mpiBarrier.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS) -lm
	
dissemination: dissemination.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS) -lm
	
Tournament: Tournament.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS) -lm
	
hybrid: hybrid.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS) -lm

MCS: MCS.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) -lm

OMPBarrier: OMPBarrier.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) -lm
	
senseReversal: senseReversal.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) -lm
	
clean:
	rm -f *.o mpiBarrier dissemination Tournament hybrid MCS OMPBarrier senseReversal
