#PBS -q cs6210
#PBS -l nodes=1:fourcore
#PBS -l walltime=00:5:00
#PBS -N senseReversal_Stats
orte_base_help_aggregate=1
MPI_MCA_mpi_yield_when_idle=0

i=2
j=0
while [ $j -lt 10 ];
do
	i=2
	while [ $i -lt 9 ];
	do
	echo "$i,$j" &
	/nethome/rsurapaneni6/AOS/final/MPI/senseReversal $i 50 &
	i=$((i+1))
	sleep 1
	done
j=$((j+1))
done


