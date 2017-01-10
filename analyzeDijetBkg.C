



static int leadMin = 120;
static int subMin = 30;


static bool mc = 1;

static double ptMins[] = {120,130,140,150,160,180,200,220,260,300,350};

static int bkgColor = kMagenta+2;


void analyze(   
	     string var = "pt2",
             string refvar = "refpt2",
	     bool s2760 = 1,
	     int leadCut = 100,
	     int pthat = 80,
	     string algo = "akPu4PF",
	     string sample = "pythia6"
			      ){

  leadMin = leadCut;

  TCut lead(Form("(pt1> %d)",leadMin));
  TCut sublead(Form("(pt2 > %d)",subMin));

  TCut dijet(Form("(pt1> %d && pt2 > %d && acos(cos(dphi)) > 2.0944)",leadMin,subMin));

  TCut ptBin(Form("(pt1> 120 && pt1< 150)"));
  TCut central("(bin < 40)");
  TCut subtraction(Form("(pt1> %d && pt2 > %d && acos(cos(dphi)) < 1.0472)",leadMin,subMin));
  TCut leadB("abs(refparton_flavorForB1) == 5");
  TCut subB("abs(refparton_flavorForB1) == 5");

  TCut subSignal("sub2 == 0 && refpt2 > 0");
  TCut subSignal20("sub2 == 0 && refpt2 > 20");
  TCut subSignal30("sub2 == 0 && refpt2 > 30");
  TCut subSignal40("sub2 == 0 && refpt2 > 40");
  TCut subSignal50("sub2 == 0 && refpt2 > 50");

  TCut subGenMatch("refpt2 > 20");
  TCut recoCut1("pt1 >= 20");
  TCut recoCut2("pt2 >= 20");


  TCut selection = dijet;

  TCut w("w");

  TChain* t = 0;

  if(s2760 || pthat == 80 || sample == "pyquenColl"){
    t = new TChain("t");
    t->Add(Form("/data_CMS/cms/yilmaz/dijet/test7/fullsim_%dGeV_dijets_%s_pthat%d_PbPb_%s.root",s2760?2760:5020,sample.data(),pthat,algo.data()));

    t->SetAlias("sub1","subid1");
    t->SetAlias("sub2","subid2");
    //    t->SetAlias("w","1.+0.");
    t->SetAlias("w","hf");

  }else{

    t = new TChain("nt");

    if(mc){
      t->Add(Form("/data_CMS/cms/lisniak/bjet2015/mcPbqcd%s_djt.root",algo.data()));
    }else{
      t->Add(Form("/data_CMS/cms/lisniak/bjet2015/dtPbj40%s_djt.root",algo.data()));
    }
    
    t->SetAlias("pt1","jtpt1");
    t->SetAlias("pt2","jtpt2");
    t->SetAlias("eta1","jteta1");
    t->SetAlias("eta2","jteta2");
    t->SetAlias("phi1","jtphi1");
    t->SetAlias("phi2","jtphi2");

    t->SetAlias("refpt1","refpt1");
    t->SetAlias("refpt2","refpt2");
    t->SetAlias("refeta1","refeta1");
    t->SetAlias("refeta2","refeta2");
    t->SetAlias("refphi1","refphi1");
    t->SetAlias("refphi2","refphi2");

    t->SetAlias("sub1","subid1");
    t->SetAlias("sub2","subid2");
    t->SetAlias("w",Form("weight*(pthat > %d)",leadMin<80?0:50));

  }

    t->SetAlias("dphi","acos(cos(phi1-phi2))");
    t->SetAlias("xj","pt2/pt1");
    t->SetAlias("aj","(pt1-pt2)/(pt1+pt2)");
    t->SetAlias("rj",Form("(pt2 > %d)",subMin));
    t->SetAlias("eta_dijet","(eta1+eta2)/2.");
    t->SetAlias("deta","(eta1-eta2)/2.");

    t->SetAlias("var",var.data());
    t->SetAlias("refvar",refvar.data());

  TH1D* h[10];
  TH2D* hh[10];
  TH2D* hhf[10];

  int xBins = 60;
  int xMin = 0;
  int xMax = 300;
  int n2D = 50;

  if(var == "xj") xMax = 1;
  if(var == "dphi") xMax = TMath::Pi();


  TLine* one = new TLine(xMin, 1, xMax, 1);
  one->SetLineStyle(2);

  string xname = "subleading jet reco p_{T}";
  if(var == "pt1") xname = "leading jet reco p_{T}";
  if(var == "xj") xname = "p_{T,2}/p_{T,1}";

  for(int i = 0; i < 10; ++i){
    h[i] = new TH1D(Form("h%d",i),Form(";%s;event fraction",xname.data()),xBins,xMin,xMax);

    hh[i] = new TH2D(Form("hh%d",i),Form(";subleading jet gen p_{T}; %s jet reco p_{T}",xname.data()),n2D,0,200,n2D,0,200);
    hhf[i] = new TH2D(Form("hhf%d",i),"",n2D,0,200,n2D,0,200);

  }


  TH2D* hhc = new TH2D("hhc",Form(";subleading jet gen p_{T}; %s jet reco p_{T}",xname.data()),200,0,200,200,0,200);
  TH2D* hhfc = new TH2D("hhfc","",200,0,200,200,0,200);

  TCanvas* c0 = new TCanvas("c0","",600,600);
  c0->SetLogz();
  t->Draw("var:refvar>>hhc",w*(lead && subGenMatch),"colz");
  t->Draw("var:(20.-10.)>>hhfc",w*(lead && !subGenMatch),"colz");
  hhc->Add(hhfc);

  hhc->Draw("colz");

  string plotname = "";
  plotname = "correlation";
  c0->Print(Form("figure_bkg_%s_%s_%s_%s_ptLead%d_%s_%s.png",plotname.data(),var.data(),refvar.data(),sample.data(),leadMin,s2760?"2760GeV":"5020GeV",algo.data()));


  TCanvas* c1 = new TCanvas("c1","",600,600);
  //  c1->SetLogz();

  t->Draw("var:refvar>>hh0",w*(central && lead && subGenMatch && subSignal20),"colz");
  t->Draw("var:refvar>>hh1",w*(central && lead && subGenMatch && (!subSignal20)),"colz");
  t->Draw("var:10>>hh2",w*(central && lead && !subGenMatch && subSignal20),"colz");
  t->Draw("var:10>>hh3",w*(central && lead && !subGenMatch && (!subSignal20)),"colz");
  hh[0]->Add(hh[2]);
  hh[1]->Add(hh[3]);

  hh[0]->SetMarkerSize(0.1);
  hh[1]->SetMarkerSize(0.1);
  hh[1]->SetLineColor(2);
  hh[1]->SetMarkerColor(2);
  hh[0]->SetLineColor(3);
  hh[0]->SetMarkerColor(3);

  //  hh[0]->Draw("");
  //  hh[1]->Draw("same");
  hh[0]->Draw("box z");
  hh[1]->Draw("box same");


  plotname = "separation";
  c1->Print(Form("figure_bkg_%s_%s_%s_%s_ptLead%d_%s_%s.png",plotname.data(),var.data(),refvar.data(),sample.data(),leadMin,s2760?"2760GeV":"5020GeV",algo.data()));


  //  return;


  TCanvas* c2 = new TCanvas("c2","",600,600);

  t->Draw("var>>h0",w*(central && lead && "pt2 >= 20"));
  t->Draw("var>>h1",w*(central && lead && "pt2 >= 20" && (!subSignal20)));
  t->Draw("0>>h2",w*(central && lead && "pt2 < 20"));
  t->Draw("0>>h3",w*(central && lead && "pt2 < 20" && (!subSignal20)));


  // "Fake"
  t->Draw("var>>h4",w*(central && lead && "pt2 >= 20" && (!subSignal20)));
  t->Draw("0>>h5",w*(central && lead && "pt2 < 20" && (!subSignal20)));

  // "Efficiency"
  t->Draw("var>>h6",w*(central && lead && "pt2 >= 20" && (subSignal20)));
  t->Draw("0>>h7",w*(central && lead && "pt2 < 20" && (subSignal20)));



  for(int i = 7; i >= 0; --i){
    h[i]->Scale(1./h[0]->Integral());
  }

  //  h[0]->Add(h[2]);

  //  h[1]->Add(h[2]);
  //  h[1]->Add(h[3]);

  h[0]->Add(h[2]);
  h[0]->Add(h[3]);

  h[1]->SetFillColor(bkgColor);
  h[1]->SetFillStyle(3020);

  h[0]->Draw("hist");
  h[1]->Draw("hist same");


  TLegend *leg=new TLegend(0.5,0.78,0.82,0.94);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(43);
  leg->SetTextSize(18);

  leg->AddEntry(h[0],"all subleading jets","l");
  leg->AddEntry(h[1],"not the true subleading jet","f");
  leg->AddEntry(h[0],Form("p_{T,1} > %d",leadMin),"");
  leg->Draw();


  plotname = "linear";
  c2->Print(Form("figure_bkg_%s_%s_%s_%s_ptLead%d_%s_%s.png",plotname.data(),var.data(),refvar.data(),sample.data(),leadMin,s2760?"2760GeV":"5020GeV",algo.data()));


  TCanvas* c3 = new TCanvas("c3","",600,600);
  c3->SetLogy();
  h[0]->SetMaximum(10);
  h[0]->SetMinimum(0.000001);
  if(var == "dphi"){
    h[0]->SetMaximum(10);
    h[0]->SetMinimum(0.0001);
  }
  h[0]->Draw("hist");
  h[1]->Draw("hist same");

  leg->Draw();

  plotname = "log";
  c3->Print(Form("figure_bkg_%s_%s_%s_%s_ptLead%d_%s_%s.png",plotname.data(),var.data(),refvar.data(),sample.data(),leadMin,s2760?"2760GeV":"5020GeV",algo.data()));




  TCanvas* c4 = new TCanvas("c4","",600,600);
  c4->SetLogy();

  TH1D* hs = (TH1D*)h[0]->Clone("hs");
  hs->Add(h[1],-1);
  hs->Draw("hist");

  plotname = "signalonly";
  c4->Print(Form("figure_bkg_%s_%s_%s_%s_ptLead%d_%s_%s.png",plotname.data(),var.data(),refvar.data(),sample.data(),leadMin,s2760?"2760GeV":"5020GeV",algo.data()));



  TCanvas* c5 = new TCanvas("c5","",600,600);

  TH1D* hr = (TH1D*)h[6]->Clone("hr");
  hr->Divide(hr,h[0],1,1,"B");

  hr->Draw("");

  one->Draw("same");

  plotname = "efficiency";
  c5->Print(Form("figure_bkg_%s_%s_%s_%s_ptLead%d_%s_%s.png",plotname.data(),var.data(),refvar.data(),sample.data(),leadMin,s2760?"2760GeV":"5020GeV",algo.data()));


  TCanvas* c6 = new TCanvas("c6","",600,600);

  TH1D* hf = (TH1D*)h[4]->Clone("hf");
  hf->Divide(hf,h[0],1,1,"B");

  hf->Draw("");
  one->Draw("same");

  plotname = "fake";
  c6->Print(Form("figure_bkg_%s_%s_%s_%s_ptLead%d_%s_%s.png",plotname.data(),var.data(),refvar.data(),sample.data(),leadMin,s2760?"2760GeV":"5020GeV",algo.data()));




  //  t->Draw("acos((cos(jtphi1-jtphi0)))","jtpt0 > 120 && jtpt1 > 50","same");


  //  t->Draw("(subid1!=0):jtpt1","jtpt1 > 50","prof");



}



void analyzeDijetBkg(){

  TH1::SetDefaultSumw2();

  string vars[] = {"pt2","pt1","xj","dphi"};
  string refvar = "refpt2";
  string algos[] = {"akPu4PF","akVs4PF","akPu3PF"};


  //  analyze(var,refvar,s2760,leadCut,pthat,algo,sample);

  analyze("xj");
  analyze("dphi");
  analyze("pt2");

  return;

  for(int j = 0; j < 3; ++j){
    for(int i = 0; i < 4; ++i){
      analyze(vars[i],refvar,0,120,80,algos[j],"pythia6");
      analyze(vars[i],refvar,0,120,100,algos[j],"pyquenColl");

      if(0){
	analyze(vars[i],refvar,0,80,50,algos[j]);
	
	analyze(vars[i],refvar,1,80,50,algos[j]);
	
	analyze(vars[i],refvar,0,120,80,algos[j]);
	analyze(vars[i],refvar,1,120,80,algos[j]);
	
	analyze(vars[i],refvar,0,50,30,algos[j]);
	analyze(vars[i],refvar,1,50,30,algos[j]);
      }
    }

    return; // don't run other algos
  }



}


