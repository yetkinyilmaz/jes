

void correctBin(int bin = 0, bool mc = 1){

  TH1::SetDefaultSumw2();

  TFile* inf = new TFile("/data_CMS/cms/lisniak/bjet2015/mcPbqcdakPu4PF_djt.root");

  TNtuple* nt = (TNtuple*)inf->Get("nt"); 

  TProfile* p = new TProfile("p","",200,0,200);
  TF1* f = new TF1("f","0.5*(TMath::Erf((x-[0])/[1])+1.)",20,120);
  f->SetParameter(0,70);
  f->SetParameter(1,10);
  f->Draw();

  TCut weight("weight");

  TCut lead("jtpt1 > 100");

  TCut centrality("bin < 200");
  if(bin == 1) centrality = "bin >= 0 && bin < 20";
  if(bin == 2) centrality = "bin >= 20 && bin < 60";
  if(bin == 3) centrality = "bin >= 60 && bin < 200";

  TCanvas* c1 = new TCanvas(Form("c%d",bin),"c",600,600);
  nt->Draw("(subid2 == 0 && refpt2 > 20):jtptSignal2>>p",weight*(lead&&centrality));

  p->Fit(f,"MRQ");


  cout<<"Correction for centrality "<<(const char*)centrality<<endl;
  cout<<Form("TF1* f = new TF1(\"f\",\"1./(0.5*(TMath::Erf((x-[0])/[1])+1.))\",20,500);")<<endl;
  cout<<Form("f->SetParameter(0,%f);",f->GetParameter(0))<<endl;
  cout<<Form("f->SetParameter(1,%f);",f->GetParameter(1))<<endl;

  c1->Print(Form("figure_correction_subleading_%s_bin%d.png",mc?"mc":"data",bin));

}



void correctSubleading(){

  correctBin(1);
  correctBin(2);
  correctBin(3);


}


