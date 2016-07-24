orte_base_help_aggregate = 0
ulimit -l unlimited
i=2
j=0
while [ $j -lt 4 ];
do
        i=2
        while [ $i -lt 9 ];
        do
        k=2
        while [ $k -lt 12 ];
        do
        /opt/openmpi-1.4.3-gcc44/bin/mpirun -np 2 /nethome/rsurapaneni6/AOS/final/MPI/hybrid 50 2&
        k=$((k+1))
        sleep 1
        done
        i=$((i+1))
        done
j=$((j+1))
done

