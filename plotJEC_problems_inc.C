

void plotJEC_problems_inc(int ptmin = 40){

  TH1::SetDefaultSumw2();

  TFile* inf = new TFile("~/data/jec_r2/mcPbqcdakPu4PF_inc.root");
  TNtuple* nt = (TNtuple*)inf->Get("nt");

  TProfile* p = new TProfile("p",";centrality(%);#LTp_{T}^{reco}/p_{T}^{gen}#GT",50,0,100);


  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  nt->Draw("jtpt/refpt:bin*0.5>>p",Form("weight*(refpt > %d && refpt < %d && abs(jteta) < 1.5 && pthat > 30 && jtpt > 20)",ptmin,ptmin+10),"prof");

  p->SetMaximum(1.2);
  p->SetMinimum(0.8);

  TLine* line = new TLine(0,1,100,1);
  line->SetLineStyle(2);

  p->Draw();
  line->Draw();

  c1->Print(Form("jes_centrality_pt%d.png",ptmin));






}

