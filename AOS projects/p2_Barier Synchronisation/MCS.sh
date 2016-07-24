#PBS -q cs6210
#PBS -l nodes=1:fourcore
#PBS -l walltime=00:1:00
#PBS -N MCS_Stats
orte_base_help_aggregate=1
MPI_MCA_mpi_yield_when_idle=0

i="2"
while [ $i -lt 9 ]
do
/nethome/rsurapaneni6/AOS/final/MPI/MCS $i 50 &
i=$[$i+1]
done

