#ifndef Analysis_h_
#define Analysis_h_

static int leadMin = 120;
static int subMin = 30;


static bool mc = 1;
static bool pp = 1;
static bool peri = 0;
static bool density = 0;

static double ptMins[] = {120,130,140,150,160,180,200,220,260,300,350};

static const int Npt = 21;
static const int Neta = 4;
static const int Ncent = 10;

static double etaMinFine[] = {0.0,1.5,1.6,1.8,2.,9999999};
static double ptBins[] = {0,20,30,40,50,60,70,80,90,100,120,140,160,180,200,240,280,320,380,440,500,600,999999999};
static int centBins[] = {0,10,20,30,40,50,60,80,100,140,200,999999};
static int centMinCoarse[] = {0,20,60,200};

static int Xbins = -1;
static double Xmin = 0;
static double Xmax = 1;


static string dir = "/Users/yetkin/data/jec_r3";
static string file = "/Users/yetkin/data/smear_v2/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_PbPb_akPu4PF_jes.root";


void defineVariables(TTree* t){
  t->SetAlias("pt1","jtpt1");
  t->SetAlias("pt2","jtpt2");
  t->SetAlias("eta1","jteta1");
  t->SetAlias("eta2","jteta2");
  t->SetAlias("phi1","jtphi1");
  t->SetAlias("phi2","jtphi2");
  t->SetAlias("dphi","acos(cos(phi1-phi2))");
  t->SetAlias("gendphi","acos(cos(genphi1-genphi2))");
  t->SetAlias("smdphi","acos(cos(smphi1-smphi2))");

  t->SetAlias("scpt1","mycorr1*pt1");
  t->SetAlias("scpt2","mycorr2*pt2");

  t->SetAlias("jtresp","jtc*jtpt/refpt");
  t->SetAlias("resp","patc*patpt/genpt");

  t->SetAlias("resp1","pt1/refpt1");
  t->SetAlias("resp2","pt2/refpt2");
  t->SetAlias("resp12","pt1/refpt1");
  t->SetAlias("resp21","pt2/refpt2");

  t->SetAlias("smresp","smpt/genpt");
  t->SetAlias("smresp1","smpt1/smrefpt1");
  t->SetAlias("smresp2","smpt2/smrefpt2");
  t->SetAlias("smresp12","smpt1/smrefpt1");
  t->SetAlias("smresp21","smpt2/smrefpt2");

  t->SetAlias("xj","pt2/pt1");
  t->SetAlias("genxj","genpt2/genpt1");
  t->SetAlias("smxj","smpt2/smpt1");
  t->SetAlias("scxj","scpt2/scpt1");
  t->SetAlias("refxj","refpt2/refpt1");
  t->SetAlias("smrefxj","smrefpt2/smrefpt1");

  t->SetAlias("aj","(pt1-pt2)/(pt1+pt2)");
  t->SetAlias("rj",Form("(pt2 > %d)",subMin));

  t->SetAlias("bkg",Form("(pt2 > %d && dphi < 1.0472)",subMin));

  t->SetAlias("eta_dijet","(eta1+eta2)/2.");
  t->SetAlias("deta","(eta1-eta2)/2.");

}

void rescalePt(TTree* t, int jet, double c, double s, double n){
  t->SetAlias("c",Form("0.+%0.2f*bin/bin",c));
  t->SetAlias("s",Form("0.+%0.2f*bin/bin",s));
  t->SetAlias("n",Form("0.+%0.2f*bin/bin",n));
  cout<<"Scaled"<<endl;
  t->SetAlias(Form("mycorr%d",jet),Form("(c+s/sqrt(jtpt%d)+n/jtpt%d)",jet,jet));
  t->SetAlias(Form("pt%d",jet),Form("jtpt%d*mycorr%d",jet,jet));

}

void correctPt(TTree* t, int jet, double c, double s, double n){

  rescalePt(t,jet,c,s,n);
  t->SetAlias(Form("mycorr%d",jet),Form("(1./(c+s/sqrt(jtpt%d)+n/jtpt%d))",jet,jet));
  t->SetAlias(Form("pt%d",jet),Form("jtpt%d*mycorr%d",jet,jet));

}



TH1* get(string var="xj", const char* name = "h", TTree* t = 0, TCut subtract = "", TCut cut = ""){

  TH1::SetDefaultSumw2();

  //  defineVariables(t);
  if(var == "acos(cos(dphi))")  subtract = "";

  TCut lead(Form("(pt1> %d)",leadMin));
  TCut sublead(Form("(pt1> %d) && (pt2 > %d)",leadMin,subMin));

  TCut dijet(Form("(pt1> %d && pt2 > %d && acos(cos(dphi)) > 2.0944)",leadMin,subMin));

  TCut ptBin(Form("(pt1> 120 && pt1< 150)"));

  if(pp || peri) ptBin = Form("(pt1> 120)");

  TCut centrality("(bin < 40)");
  if(density) centrality = "bin >= 20 && bin < 60";
  if(peri) centrality = "bin >= 140";
  if(pp) centrality = "bin > -9999";

  TCut subtraction(Form("(pt1> %d && pt2 > %d && acos(cos(dphi)) < 1.0472)",leadMin,subMin));

  TCut selection = dijet;
  if(cut != "") selection = cut;

  //  TCut weight("weight*(pthat > 50 && hltCaloJet80)");
  TCut weight("weight");
  if(mc) weight = "weight";
  weight = "weight*(hltCaloJet80)";
  if(mc) weight = "weight*(pthat > 50)";

  int xBins = 12;
  double xMin = 0;
  double xMax = 1.2;

  if(
     var == "refpt2" || var == "refpt1" 
     || var == "genpt2" || var == "genpt1" 
     || var == "pt2" || var == "pt1"
     || var == "jtpt2" || var == "jtpt1"
     || var == "smpt2" || var == "smpt1"
     || var == "pthat"
     ){
    xBins = 60;
    xMax = 300;
  }


  if(var == "aj"){
    xBins = 15;
    xMin = 0;
    xMax = 0.9;
  }

  if(var == "resp" || var == "smresp"){
    xBins = 30;
    xMin = 0;
    xMax = 3;
  }


  if(var == "acos(cos(dphi))"){
    xBins = 30;
    xMax = TMath::Pi();
    selection = lead*sublead;
  }

  if(var=="eta_dijet" || var=="deta"){
    xBins = 16;
    xMin = -2;
    xMax = 2;
  }

  if(Xbins > 0){
    xBins = Xbins;
    xMin = Xmin;
    xMax = Xmax;
  }


  TH1* h, *hs, *hl;
  if(var=="bkg"){

    double cBins[] = {0,10,30,100};
    h = new TProfile(name,Form(";centrality;N[#Delta#phi_{1,2}<#frac{2}{3}#pi]/N_{leading jet}"),3,cBins);
    t->Draw(Form("bkg:(bin/2)>>%s",h->GetName()),weight*lead,"prof");

  }else if(var=="rj:pt1"){
    xMin = 0;
    xMax = 300;
    xBins = 30;

    h = new TProfile(name,Form(";p_{T,1};Dijet Ratio"),xBins,xMin,xMax);
    t->Draw(Form("%s>>%s",var.data(),h->GetName()),weight*selection,"prof");

  }else if(var=="rj" || var=="subrj"){

    xMin = 120;
    xMax = 300;
    xBins = 10;

    hl = new TH1D("hl",Form(";%s;Event Fraction",var.data()),Npt,ptBins);
    t->Draw(Form("pt1>>%s",hl->GetName()),weight*centrality*lead);

    h = new TH1D("h",Form(";%s;Event Fraction",var.data()),Npt,ptBins);
    t->Draw(Form("pt1>>%s",h->GetName()),weight*centrality*dijet);

    if(subtract != ""){
      hs = new TH1D("hs",Form(";%s;Event Fraction",var.data()),Npt,ptBins);
      t->Draw(Form("pt1>>%s",hs->GetName()),weight*centrality*subtraction);
      h->Add(hs,-1);
    }

    h->Divide(h,hl,1,1,"B");
    hs->Divide(hs,hl,1,1,"B");


  }else if(var == "genpt" || var == "jtpt" 
	   || var == "refpt" || var == "pthat"
	   || var == "pt1" || var == "genpt1"
	   ){

    if(Xbins > 0){
      h = new TH1D("h",Form(";%s;Event Fraction",var.data()),Xbins,Xmin,Xmax);
    }else{
      h = new TH1D("h",Form(";%s;Event Fraction",var.data()),Npt,ptBins);
    }

    t->Draw(Form("%s>>%s",var.data(),h->GetName()),weight*selection,"");

  }else{

    h = new TH1D("h",Form(";%s;Event Fraction",var.data()),xBins,xMin,xMax);
    t->Draw(Form("%s>>%s",var.data(),h->GetName()),weight*(selection));

    if(subtract != ""){
      hs = new TH1D("hs",Form(";%s;Event Fraction",var.data()),xBins,xMin,xMax);
      t->Draw(Form("%s>>%s",var.data(),hs->GetName()),weight*subtract);
      h->Add(hs,-1);
    }

    h->Scale(1./h->Integral());
  }

  if(var=="subrj") return hs;

  h->SetName(name);
  return h;

}

TLine* drawLine(double xMax = 1, double y = 1){

  TLine* line = new TLine(0,y,xMax,y);
  line->SetLineStyle(2);
  line->Draw();


  return line;
}

double addQuad(double a, double b, double c = 0, double d = 0, double e = 0, double f = 0, double g = 0){
  return sqrt(a*a+b*b+c*c+d*d+e*e+f*f+g*g);
}


TLegend* makeLegend(double x1 = 0.35, double y1 = 0.65, double x2 = 0.9, double y2 = 0.9){

  TLegend *leg = new TLegend(x1,y1,x2,y2);

  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(43);
  leg->SetTextSize(18);

  return leg;
}

TH1** load(int ipt, int ieta, int icent, int cone = 3){
  TFile* inf = new TFile(file.data());

  TH1* h = (TH1*)inf->Get(Form("h_%d_%d_%d",ipt,ieta,icent));
  TH1* h0 = (TH1*)inf->Get(Form("h0_%d_%d_%d",ipt,ieta,icent));
  TH1* h20 = (TH1*)inf->Get(Form("h20_%d_%d_%d",ipt,ieta,icent));
  TH1* h20s = (TH1*)inf->Get(Form("h20s_%d_%d_%d",ipt,ieta,icent));
  TH1* h30 = (TH1*)inf->Get(Form("h30_%d_%d_%d",ipt,ieta,icent));
  TH1* hh[] = {h,h0,h20,h20s,h30};
  return hh;
}


#endif



