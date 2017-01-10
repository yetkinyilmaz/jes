
static TLine* line;
static TF1* fsafe;

static TTree* t;
static TProfile* p;
static bool zoom = 0;

static TCut cent00to10("bin >= 0 && bin < 20");
static TCut cent10to30("bin >= 20 && bin < 60");
static TCut cent30to100("bin >= 60 && bin < 200");

static TCut selection("genpt > 0 && patpt > 0 && gensubid == 0");

static TCut eta0("abs(geneta) >= 0 && abs(geneta) < 1.5");
static TCut eta1("abs(geneta) >= 1.5 && abs(geneta) < 1.6");
static TCut eta2("abs(geneta) >= 1.6 && abs(geneta) < 1.8");
static TCut eta3("abs(geneta) >= 1.8 && abs(geneta) < 2");

static TCut etaBin = eta0;
static TCut centrality = cent30to100;

static TCut weight("cweight");

static int centBins[] = {0,10,20,30,40,50,60,80,100,140,200};


void plot(double c0, double c1, double c2, double c3, double c4 = 15, int centBin = -1){

  if(centBin >= 0){
    centrality = Form("(bin >= %d && bin < %d)",centBins[centBin],centBins[centBin+1]);
  }



  //  t->SetAlias("corpt",Form("patpt/(%f-%f*exp(-%f*par)+%f/(par-%f)/(par-%f))",c0,c1,c2,c3,c4,c4));
  t->SetAlias("cc3","3.023*exp(-0.0113*bin)");
  t->SetAlias("corpt",Form("patpt/(%f-%f*exp(-%f*par)+cc3/(par-%f)/(par-%f))",c0,c1,c2,c4,c4));


  t->Draw("corpt/genpt:genpt>>p",weight*(centrality&&selection&&etaBin),"prof");
  //  t->Draw("genc*patpt/genpt:genpt","weight*(bin >= 60 && bin < 200 && genpt > 20 && patpt > 0 && abs(geneta) < 1.5)","prof same");


  p->SetMaximum(1.3);
  p->SetMinimum(0.8);

  if(zoom){
    p->SetMaximum(1.02);
    p->SetMinimum(0.98);
  }
  p->Draw();
  line->Draw("same");
  fsafe->Draw("same");

}


void playScale(){

  int cent = 0;
  int cone = 4;

  // PAS file
  //  TFile* inf = new TFile(Form("~/data/jec_r3/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_PbPb_akPu%dPF_jesres.root",cone));

  // CS file
  TFile* inf = new TFile(Form("/Users/yetkin/data/cs_pythia8_eta15_v0/akCs4_pythia8_v1.root"));

  t = (TTree*)inf->Get("t");
  t->SetAlias("par","patpt");


  p = new TProfile("p",";p_{T}^{gen};#LTp_{T}^{reco}/p_{T}^{gen}#GT",50,0,500);
  line = new TLine(0,1,500,1);
  line->SetLineStyle(2);
  fsafe = new TF1("fsafe","40/(x+5)",10,40);
  fsafe->SetLineStyle(3);

  TCanvas* can1 = new TCanvas("can1","can1",600,600);

  TLegend *leg0=new TLegend(0.44,0.55,0.92,0.92);
  leg0->SetFillColor(0);
  leg0->SetBorderSize(0);
  leg0->SetFillStyle(0);
  leg0->SetTextFont(43);
  leg0->SetTextSize(25);
  //  if(cent == 0) leg0->AddEntry(fsafe,"0-10%","");
  //  if(cent == 1) leg0->AddEntry(fsafe,"10-30%","");
  //  if(cent == 2) leg0->AddEntry(fsafe,"30-100%","");

  //  leg0->AddEntry(p0,"Before","p");
  //  leg0->AddEntry(p1,"After","p");
  leg0->AddEntry(fsafe,"Safety zone","l");
  leg0->AddEntry(fsafe,"(y*(x+5) < 40)","");


  for(int ieta = 0; ieta < 4; ++ieta){
    for(int icent = 0; icent < 3; ++icent){

      if(ieta == 0) etaBin = eta0;
      if(ieta == 1) etaBin = eta1;
      if(ieta == 2) etaBin = eta2;
      if(ieta == 3) etaBin = eta3;

      if(icent == 0) centrality = cent00to10;
      if(icent == 1) centrality = cent10to30;
      if(icent == 2) centrality = cent30to100;

      plot(1.,0,0,0,0);

      TLegend *l=new TLegend(0.24,0.55,0.92,0.92);
      l->SetFillColor(0);
      l->SetBorderSize(0);
      l->SetFillStyle(0);
      l->SetTextFont(43);
      l->SetTextSize(25);
      l->AddEntry(fsafe,Form("%s",(const char*)etaBin),"");
      l->AddEntry(fsafe,Form("%s",(const char*)centrality),"");
      l->Draw();
      can1->Print(Form("performance_cs_eta%d_cent%d.png",ieta,icent));
    }
  }



  //  eta < 1.5
  //  plot(1.,0.08,0.01,0,15); perfect for gen 70-100%, good enough for reco
  //  plot(1.,0.08,0.01,230,15); +1% in 0-5%
  //  plot(1.,0.08,0.01,200,15); +1% in 0-10%
  //  plot(1.,0.08,0.01,55,15); superflat -1% in 10-30%
  //  plot(1.,0.08,0.01,5,15); superflat -1% in 30-100%

  //  Used 20160616
  //  plot(1.01,0.08,0.01,150,15); // superflat in 0-10%
  //  plot(0.99,0.08,0.01,55,15); // superflat in 10-30%
  //  plot(0.995,0.08,0.01,5,15); // superflat in 30-100%

  // eta > 1.8
  //  plot(1.,0.04,0.01,-8,15); good enough for 70-100%
  // plot(1.1,0.04,0.01,550,15); pm 5% for 0-5%

  //  plot(1.1,0.02,0.01,430,15);


  //Fine cent binning

  // |eta| < 1.5
  //  plot(1.,0.08,0.01,210,15,0); // maybe better
  //  plot(1.,0.08,0.01,230,15,0);
  //  plot(1.,0.08,0.01,130,15,1);
  //  plot(0.997,0.08,0.01,80,15,2);
  //  plot(0.995,0.08,0.01,60,15,3);
  //  plot(0.99,0.08,0.01,50,15,4);
  //  plot(0.99,0.08,0.01,25,15,5);
  //  plot(0.99,0.08,0.01,15,15,6);
  //  plot(0.99,0.08,0.01,10,15,7);
  //  plot(0.995,0.08,0.01,5,15,8);
  //  plot(1.0,0.08,0.01,0,15,9);


  // 1.5 < |eta| < 1.6
  //  plot(1.,0.06,0.01,0,15,9); // +- 2%
  //  plot(0.99,0.06,0.01,5,15,8); // += 2%
  //  plot(1.,0.06,0.01,5,15,7);


  // 1.6 < |eta| < 1.8

  // 1.8 < |eta| < 2



  // ------   R = 0.3 -------

  //  eta < 1.5

  //  plot(1.,0.08,0.008,5,20,9); // ok
  //  plot(1.,0.08,0.008,5,20,8); // ok

  //  plot(1.,0.12,0.008,5,20,7);
  //  plot(1.,0.14,0.008,5,20,6);

  //  plot(1.,0.14,0.008,15,20,5);
  //  plot(1.,0.14,0.008,15,20,4);
  //  plot(1.,0.14,0.008,15,20,3);
  //  plot(1.,0.14,0.008,15,20,2); // ok
  //  plot(1.,0.14,0.008,15,20,1); // ok
  //  plot(1.,0.12,0.008,20,20,0); // ok


  //  leg0->Draw();
  //  can1->Print("central_corr.png");

  //  plot(1.,0.,0.,0,0,9);
  //  can1->Print("peripheral_nocorr.png");

  //  plot(1.,0.,0.,0,0,0);
  //  can1->Print("central_nocorr.png");


  return;

  //-----

  // Plotting

  double c0 = 1., c1 = 0, c2 =0, c3 = 0, c4 = 0;
  if(cent == 0){
    centrality = cent00to10;
    c0 = 1.01;
    c1 = 0.08;
    c2 = 0.01;
    c3 = 150;
    c4 = 15;
  }

  if(cent == 1){
    centrality = cent10to30;
    c0 = 0.99;
    c1 = 0.08;
    c2 = 0.01;
    c3 = 55;
    c4 = 15;

  }
  if(cent == 2){
    centrality = cent30to100;
    c0 = 0.995;
    c1 = 0.08;
    c2 = 0.01;
    c3 = 5;
    c4 = 15;
  }

  plot(1.,0.,0.,0.,0.);
  TProfile* p0 = (TProfile*)p->Clone("p0");


  plot(c0,c1,c2,c3,c4);
  TProfile* p1 = (TProfile*)p->Clone("p1");

  p0->SetMarkerStyle(25);
  p0->Draw();
  p1->Draw("same");
  line->Draw("same");

  fsafe->Draw("same");

  can1->Print(Form("figure_residual_cent%d.png",cent));


}

