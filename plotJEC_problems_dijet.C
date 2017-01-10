

void plotJEC_problems_dijet(int ptmin = 40){

  TH1::SetDefaultSumw2();

  TFile* inf = new TFile("~/data/jec_r3/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_PbPb_akPu4PF_jesres.root");
  TTree* t = (TTree*)inf->Get("t");

  //  TProfile* p = new TProfile("p",";centrality(%);#LTp_{T}^{reco}/p_{T}^{gen}#GT",50,0,100);
  TH1D* h0 = new TH1D("h0",";centrality(%);events",50,0,100);
  TH1D* h = new TH1D("h",";centrality(%);events",50,0,100);
  TH1D* hb = new TH1D("hb",";centrality(%);events",50,0,100);


  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  t->Draw("bin*0.5>>h","weight*(pt1 > 100 && pt2 > 40 && pthat > 30 && acos(cos(phi1-phi2))> 2.0944)");
  t->Draw("bin*0.5>>hb","weight*(pt1 > 100 && pt2 > 40 && pthat > 30 && acos(cos(phi1-phi2))< 1.0472)");

  h->Add(hb,-1);

  TLine* line = new TLine(0,1,100,1);
  line->SetLineStyle(2);

  h->Scale(1./h->Integral());
  h0->Scale(1./h0->Integral());

  h->Draw();
  //  h0->Draw("same hist");
  //  line->Draw();

  c1->Print(Form("dijet_centrality_pt%d_jec_r3.png",ptmin));


  TProfile* p = new TProfile("p",";centrality(%);#LTp_{T}^{reco}/p_{T}^{gen}#GT",50,0,100);


  TCanvas* c2 = new TCanvas("c2","c2",600,600);
  t->Draw("patc*patpt/genpt:bin*0.5>>p",Form("weight*(genpt > %d && genpt < %d && abs(geneta) < 1.5 && pthat > 30 && patpt > 30)",ptmin,ptmin+10),"prof");

  p->SetMaximum(1.2);
  p->SetMinimum(0.8);

  line = new TLine(0,1,100,1);
  line->SetLineStyle(2);

  p->Draw();
  line->Draw();

  c2->Print(Form("jes_centrality_pt%d_jec_r3.png",ptmin));




}

