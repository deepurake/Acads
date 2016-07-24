Compilation:
	make
Clean the folder:
	make clean

OMPBarrier.c:
	./OMPBarrier <num_threads> <num_iterations>
senseReversal.c:
	./senseReversal <num_threads> <num_iterations>
MCS.c:
	./MCS <num_threads> <num_iterations>
mpiBarrier.c:
	mpirun -np <num_procs> mpiBarrier <num_iterations>
dissemination.c
	mpirun -np <num_procs> dissemination <num_iterations>
Tournament.c:
	mpirun -np <num_procs> Tournament <num_iterations>
hybrid.c:(Combined Barrier)
	mpirun -np <num_procs> hybrid <num_iterations> <num_threads>
