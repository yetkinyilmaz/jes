

void plotJEC_problems_djt(int ptmin = 40){

  TH1::SetDefaultSumw2();

  TFile* inf = new TFile("~/data/jec_r2/mcPbqcdakPu4PF_djt.root");
  TNtuple* nt = (TNtuple*)inf->Get("nt");

  //  TProfile* p = new TProfile("p",";centrality(%);#LTp_{T}^{reco}/p_{T}^{gen}#GT",50,0,100);
  TH1D* h0 = new TH1D("h0",";centrality(%);events",50,0,100);
  TH1D* h = new TH1D("h",";centrality(%);events",50,0,100);
  TH1D* hb = new TH1D("hb",";centrality(%);events",50,0,100);


  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  nt->Draw("bin*0.5>>h0","pthatweight*(genpt1 > 100 && genpt2 > 40 && pthat > 30)");
  nt->Draw("bin*0.5>>h","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && pthat > 30 && acos(cos(jtphi1-jtphi2)) > 2.0944)");
  nt->Draw("bin*0.5>>hb","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && pthat > 30 && acos(cos(jtphi1-jtphi2)) < 1.0472)");

  h->Add(hb,-1);

  TLine* line = new TLine(0,1,100,1);
  line->SetLineStyle(2);

  h->Scale(1./h->Integral());
  h0->Scale(1./h0->Integral());

  h->Draw();
  //  h0->Draw("same hist");
  //  line->Draw();

  c1->Print(Form("dijet_centrality_pt%d.png",ptmin));






}

