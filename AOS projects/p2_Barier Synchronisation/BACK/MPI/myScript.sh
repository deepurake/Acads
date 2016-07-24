#PBS -q cs6210
#PBS -l nodes=4:sixcore
#PBS -l walltime=00:10:00
#PBS -N MPI1
orte_base_help_aggregate=1
MPI_MCA_mpi_yield_when_idle=0

/opt/openmpi-1.4.3-gcc44/bin/mpirun -np 4 /nethome/rsurapaneni6/AOS/final/MPI/hybrid 10

#PBS -N MPI2
/opt/openmpi-1.4.3-gcc44/bin/mpirun -np 4 /nethome/rsurapaneni6/AOS/final/MPI/MCS 10
