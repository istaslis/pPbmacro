void Fill(TNtuple *nt, int N1, int N2)
{
	for (int i=0;i<N1;i++)
		nt->Fill(gRandom->Gaus(2,0.5), gRandom->Gaus(2,0.5), 1);

	for (int i=0;i<N2;i++)
		nt->Fill(gRandom->Gaus(3,0.5), gRandom->Gaus(3,0.5), 1);

}

void mvatemplate()
{
	const int Ntrain1 = 1000;
	const int Ntrain2 = 1000;
	const int Ntest1 = 1000;
	const int Ntest2 = 1000;



	TNtuple *train = new TNtuple("train","train","x:y:l");
	TNtuple *test = new TNtuple("test","test","x:y:l");

	int bins = 20;
	float xmin = 0, xmax = 5;

	TH1F *htrain1 = new TH1F("htrain1","htrain1",bins, xmin, xmax


}