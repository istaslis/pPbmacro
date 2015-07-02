root -l <<< ".L Subset_MCForest_parallel.C+"

root -l  Subset_MCForest_parallel.C+\(0\) & 
root -l  Subset_MCForest_parallel.C+\(1\) & 
root -l  Subset_MCForest_parallel.C+\(2\) & 
root -l  Subset_MCForest_parallel.C+\(3\) & 

wait
echo Subset forest is done!

root -l <<<".L Append_MC_Pthatweight.C+"
root -l Append_MC_Pthatweight.C+\(0\) &
root -l Append_MC_Pthatweight.C+\(1\) &
root -l Append_MC_Pthatweight.C+\(2\) &
root -l Append_MC_Pthatweight.C+\(3\) &

wait

hadd -f QCDPPb_HiForest.root tempout30.root tempout50.root tempout80.root tempout120.root

rm tempout30.root tempout50.root tempout80.root tempout120.root

echo done!