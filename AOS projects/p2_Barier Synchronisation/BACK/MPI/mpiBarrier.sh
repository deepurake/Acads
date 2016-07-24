#PBS -q cs6210
#PBS -l nodes=12:sixcore
#PBS -l walltime=00:5:00
#PBS -N mpiBarrier_Stats
orte_base_help_aggregate=1
MPI_MCA_mpi_yield_when_idle=0

i=2
j=0
while [ $j -lt 8 ];
do
	i=2
	while [ $i -lt 13 ];
	do
	/opt/openmpi-1.4.3-gcc44/bin/mpirun -np $i /nethome/rsurapaneni6/AOS/final/MPI/mpiBarrier 50 &
	i=$((i+1))
	sleep 1
	done
j=$((j+1))
done


