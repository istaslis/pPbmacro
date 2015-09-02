NPAR=10
echo Running $NPAR processes
for i in $(seq 1 $NPAR)
do
	root -l $1+\($i,$NPAR\) &
done

wait
echo can merge!
hadd output.root *_job*