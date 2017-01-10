

TH1* get(string var="xj", const char* name = "h", TTree* t = 0){

  t->SetAlias("xj","pt2/pt1");
  t->SetAlias("aj","(pt1-pt2)/(pt1+pt2)");
  t->SetAlias("rj","(pt2 > 30)");
  t->SetAlias("eta_dijet","(eta1+eta2)/2.");
  t->SetAlias("deta","(eta1-eta2)/2.");

  //  t->SetAlias("d","acos(cos((dphi)))");

  TCut selection("pt1> 100 && pt2 > 30 && acos(cos(dphi)) > 2.0944");

  int xBins = 20;
  double xMin = 0;
  double xMax = 1;

  if(var == "acos(cos(dphi))"){
    xMax = TMath::Pi();
    selection = "pt1> 100 && pt2 > 30";
  }

  if(var=="eta_dijet" || var=="deta"){
    xBins = 16;
    xMin = -2;
    xMax = 2;
  }

  TH1* h;

  if(var=="rj:pt1"){
    xMin = 0;
    xMax = 300;
    xBins = 30;

    h = new TProfile("h",Form(";p_{T,1};Dijet Ratio"),xBins,xMin,xMax);
    t->Draw(Form("%s>>%s",var.data(),h->GetName()),"","prof");
  }else{
    h = new TH1D("h",Form(";%s;Event Fraction",var.data()),xBins,xMin,xMax);
    t->Draw(Form("%s>>%s",var.data(),h->GetName()),selection);
    h->Scale(1./h->Integral());
  }

  return h;

}


void plotDijets(){

  TH1::SetDefaultSumw2();
  int cone = 3;

  TChain* t6 = new TChain(Form("dijets%02d/nt",cone));
  t6->Add("dijets_pthat80_cfg_*.root");
  t6->SetAlias("dphi","acos(cos(phi1-phi2))");

  //  t6->Add("pythia8/dijets_pthat80_pythia8_*.root");

  //  TChain* t8 = new TChain("t");
  //  t8->Add(Form("fullsim_dijets_pythia8_pp_ak%dPF.root",cone));
  //  t8->SetAlias("d","acos(cos(dphi))");

  TChain* t8 = new TChain(Form("dijets%02d/nt",cone));
  t8->Add("pythia8/dijets_pthat80_pythia8_*.root");
  t8->SetAlias("dphi","acos(cos(phi1-phi2))");

  string var = "acos(cos(dphi))";

  var = "rj:pt1";
  var = "eta_dijet";
  var = "deta";

  TH1* h6 = get(var, "h6", t6);
  TH1* h8 = get(var, "h8", t8);

  cout<<h6->GetEntries()<<endl;
  cout<<h8->GetEntries()<<endl;

  cout<<h6->GetMean()<<endl;
  cout<<h8->GetMean()<<endl;

  TCanvas* c1 = new TCanvas();

  if(var=="acos(cos(dphi))") h6->SetTitle(";#Delta#phi_{1,2};Event Fraction");
  h6->Draw("hist");
  h8->Draw("same");


}




