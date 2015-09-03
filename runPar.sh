echo Compiling macro...

s1='gSystem->CompileMacro("'
s2='","kfc"); gSystem->Exit(0);'

echo $s1$1$s2 | root -b -l

NPAR=10
echo Running $NPAR processes
for i in $(seq 1 $NPAR)
do
	root -l $1+\($((i-1)),$NPAR\) &
done

wait
echo can merge!

suffix="_job0"
d=`ls *$suffix`
OUTNAME=${d%$suffix}


hadd -f $OUTNAME *_job*
#hadd -f6 -O $OUTNAME *_job*
rm *_job*