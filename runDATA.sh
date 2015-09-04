root -l Subset_Data_JetTrigger.C+
./runPar.sh Append_Data_TriggerWeight.C
./runPar.sh ApplyCut_MC_Acc.C \"DATA\"
./runPar.sh Merge_MuonJet.C \"DATA\"
./runPar.sh Flattify.C \"DATA\"
