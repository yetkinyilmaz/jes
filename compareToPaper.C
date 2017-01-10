#include "Analysis.h"


static int leadMin = 120;
static int subMin = 30;


static bool mc = 1;
static bool pp = 1;
static bool peri = 0;
static bool density = 0;

static double ptMins[] = {120,130,140,150,160,180,200,220,260,300,350};



//static TColor blue;


TH1* getPaperPlot(string var = "xj");


void comparePlot(
		 string var = "acos(cos(dphi))",		    
		 string algo = "akPu3PF"
		 ){
  
  blue = TColor::GetColor(152,235,230);

  TH1::SetDefaultSumw2();

  if(pp) algo = "ak3PF";

  TChain* t = new TChain("nt");
  if(mc){
    t->Add(Form("/data_CMS/cms/lisniak/bjet2015/mc%sqcd%s_djt.root",pp?"pp":"Pb",algo.data()));
  }else{   
    t->Add(Form("/data_CMS/cms/lisniak/bjet2015/dt%s%s_djt.root",pp?"ppjpf":"Pbj60",algo.data()));
  }

  defineVariables(t);

  TH1* h0 = getPaperPlot(var);
  TH1* h = get(var, "h", t);

  cout<<h->GetEntries()<<endl;

  cout<<"2011 Mean : "<<h0->GetMean()<<endl;
  cout<<"2016 Mean : "<<h->GetMean()<<endl;

  TCanvas* c1 = new TCanvas("c1","c1",600,600);

  if(var=="acos(cos(dphi))"){

    c1->SetLogy();
    h0->SetTitle(";#Delta#phi_{1,2};Event Fraction");

    h0->Scale(1./h0->Integral());
    h0->SetMaximum(1.5);
  }

  if(var == "rj"){
    h0->SetMaximum(1.5);
    h0->SetMinimum(0.3);
    h0->SetTitle(";p_{T,1};N_{dijet}(subtracted)/N_leading jet");

  }

  if(var == "subrj"){
    h0->SetMaximum(0.2);
    h0->SetMinimum(0.);
    h0->SetTitle(";p_{T,1};N(#Delta#phi_{1,2}<#frac{1}{3}#pi)/N(#Delta#phi_{1,2}>#frac{2}{3}#pi)");

  }

  if(var == "xj"){
    h0->SetMaximum(0.28);
    h0->SetMinimum(0.);
    h0->SetTitle(";p_{T,2}/p_{T,1};Event Fraction");

  }

  if(var == "aj"){
    h0->SetMaximum(0.32);
    h0->SetMinimum(0.);
    h0->SetTitle(";A_{J};Event Fraction");

  }


  if(mc){
    //    h0->SetMarkerColor(blue);
    //    h->SetMarkerColor(blue);
  }


  h0->Draw("hist");
  h->Draw("same");



  TLegend *t3=new TLegend(0.18,0.62,0.62,0.94);
  if(mc){
    t3->AddEntry(h,"Pythia6(Z2,#sqrt{s}) + Hydjet(#sqrt{s})","");
  }else{
    t3->AddEntry(h,"Data","");
  }


  if(pp && mc){
    t3->AddEntry(h,Form("2015 - 5.02 TeV, pp"),"p");
    t3->AddEntry(h,Form("Mean : %0.3f #pm %0.3f (stat)",h->GetMean(),h->GetMeanError()),"");
    t3->AddEntry(h0,"2011 - 2.76 TeV, PbPb 70-100%","l");
    t3->AddEntry(h0,Form("Mean : %0.3f #pm %0.3f (stat)",h0->GetMean(),h0->GetMeanError()),"");
  }else{
    t3->AddEntry(h,Form("2015 - 5.02 TeV Mean : %0.3f #pm %0.3f (stat)",h->GetMean(),h->GetMeanError()),"p");
    t3->AddEntry(h0,Form("2011 - 2.76 TeV Mean : %0.3f #pm %0.3f (stat)",h0->GetMean(),h0->GetMeanError()),"l");
  }
  t3->AddEntry(h,algo.data(),"");

  if(pp){
    if(!mc) t3->AddEntry(h,"pp","");
  }else if(peri){
    t3->AddEntry(h,"70-100%","");
  }else if(density){
    t3->AddEntry(h,"2.76(0-20%) vs 5.02(10-30%)","");
    t3->AddEntry(h,"(similar density)","");
  }else{
    t3->AddEntry(h,"0-20%","");
  }

  t3->SetFillColor(0);
  t3->SetBorderSize(0);
  t3->SetFillStyle(0);
  t3->SetTextFont(43);
  t3->SetTextSize(18);

  t3->Draw();

  string sample(Form("%s_%s",pp?"pp":"Pb",peri?"peripheral":"central"));
  if(pp) sample = "pp";
  if(density) sample = "density";

  c1->Print(Form("figure_2011vs2015_%s_%s_%s_%s.png",var.data(),algo.data(),sample.data(),mc?"mc":"data"));


}


TH1* getPaperPlot(string var){

  int xBins = 10;
  double xMin = 0;
  double xMax = 1;
  if(var == "aj"){
    xBins = 15;
    xMax = 0.9;
  }

  double xjy[] = {0,0,0.0692,0.1444,0.1550,0.1543,0.1426,0.1350,0.1118,0.0877};
  double xje[] = {0,0,0.0025,0.0029,0.0029,0.0028,0.0027,0.0026,0.0024,0.0021};

  double xjymc[] = {0,0,0.0121,0.0630,0.1092,0.1388,0.1671,0.1960,0.1648,0.1490};
  double xjemc[] = {0,0,0.0031,0.0051,0.0061,0.0067,0.0069,0.0069,0.0054,0.0047};


  double ajyperi[] = {0.212,0.150,0.178,0.157,0.068,0.093,0.057,0.0410,0.0228,0.0159,0.0023,0.0023,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  double ajeperi[] = {0.022,0.019,0.020,0.019,0.012,0.015,0.011,0.0097,0.0072,0.0060,0.0039,0.0023,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};

  double ajymcperi[] = {0.2050,0.2033,0.1626,0.1229,0.0984,0.0779,0.0520,0.0446,0.0207,0.0098,0.00249,0.00041,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  double ajemcperi[] = {0.0047,0.0074,0.0058,0.0051,0.0053,0.0049,0.0037,0.0061,0.0027,0.0021,0.00068,0.00019,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};

  double ajypp[] = {0.1692,0.1827,0.1560,0.1296,0.1191,0.0792,0.05790,0.05180,0.02840,0.01560,0.0071,0.00135,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  double ajepp[] = {0.0076,0.0079,0.0073,0.0066,0.0063,0.0052,0.0044,0.0042,0.0031,0.0024,0.0016,0.00068,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};

  double *ajymcpp = ajymcperi;
  double *ajemcpp = ajemcperi;



  double dphiy[] = {0.00403,0.00344,0.00403,0.00443,0.00411,0.00362,0.00309,0.00385,0.00344,0.00362,0.00420,
		    0.00318,0.00295,0.00322,0.00380,0.00425,0.00443,0.00447,0.00564,0.00608,0.00783,0.00953,
		    0.00953,0.01275,0.01919,0.0272,0.0417,0.0698,0.1138,0.2195,0.3988};

  double dphie[] = {0.00042,0.00039,0.00042,0.00044,0.00043,0.00040,0.00037,0.00041,0.00039,0.00040,0.00043,
		    0.00038,0.00041,0.00044,0.00044,0.00045,0.00050,0.00052,0.00059,0.00065,0.00076,0.00093,
		    0.0011,0.0014,0.0018,0.0023,0.0031,0.0042};

  double dphiymc[] = {0.00107,0.00134,0.00165,0.0026,0.00114,0.00123,0.00103,0.00183,0.00158,0.00061,0.0027,
		      0.0022,0.00049,0.00159,0.00233,0.0029,0.00226,0.00226,0.00297,0.0045,0.0049,0.0083,
		      0.0106,0.0208,0.0292,0.0492,0.0778,0.1202,0.2207,0.4200};
  double dphiemc[] = {0.00050,0.00055,0.00081,0.0011,0.00049,0.00078,0.00048,0.00090,0.00069,0.00032,0.0010,
		      0.0010,0.00016,0.00055,0.00095,0.0012,0.00078,0.00082,0.00095,0.0011,0.0011,0.0015,
		      0.0017,0.0025,0.0028,0.0036,0.0046,0.0056,0.0072,0.0096};



  double rjy[] = {0.7992,0.8464,0.8657,0.9037,0.9153,0.9553,0.9653,0.9839,0.9952,0.9901};
  double rje[] = {0.0031,0.0035,0.0039,0.0043,0.0037,0.0037,0.0049,0.0045,0.0048,0.0099};
 
  double rjymc[] = {0.9094,0.9170,0.9485,0.9633,0.9702,0.963,0.964,0.9926,0.99843,0.99871};
  double rjemc[] = {0.0058,0.0063,0.0051,0.0040,0.0052,0.013,0.021,0.0031,0.00050,0.00070};

  double subrjy[] = {0.0413,0.0297,0.0294,0.0185,0.0181,0.0107,0.0074,0.0016,0.,0.};
  double subrje[] = {0.0018,0.0020,0.0025,0.0024,0.0023,0.0025,0.0030,0.0035,0.0038,0.011};

  double subrjymc[] = {0.0159,0.0179,0.0096,0.0076,0.0043,0.00179,0.00073,0.00056,0.000087,0.};
  double subrjemc[] = {0.0033,0.0050,0.0032,0.0048,0.0025,0.00053,0.00027,0.00022,0.00030,0.00036};


  double *y;
  double *e;

  if(var=="acos(cos(dphi))"){
    xBins = 30;
    xMax = TMath::Pi();
  }

  TH1D* h;

  if(var == "rj" || var == "subrj"){
    h = new TH1D("h0",";;",xBins, ptMins);
  }else{
    h = new TH1D("h0",";;",xBins, xMin, xMax);
  }



  for(int i = 0; i < xBins; ++i){

    if(var=="aj"){
      if(pp){
	if(mc){
	  y = ajymcpp;
	  e = ajemcpp;
	}else{
	  y = ajypp;
          e = ajepp;
	}
      }else if(peri){
	if(mc){
          y = ajymcperi;
          e = ajemcperi;
        }else{
          y = ajyperi;
          e = ajeperi;
        }
      }else{
	cout<<"MISSING DATA POINTS"<<endl;
	return 0;
      }      

    }




    if(var=="xj"){
      if(mc){
	y = xjymc;
	e = xjemc;
      }else{
	y = xjy;
	e = xje;
      }
    }

    if(var=="rj"){
      if(mc){
        y = rjymc;
        e = rjemc;
      }else{
        y = rjy;
        e = rje;
      }
    }

    if(var=="subrj"){
      if(mc){
        y = subrjymc;
        e = subrjemc;
      }else{
        y = subrjy;
        e = subrje;
      }
    }
    
    if(var=="acos(cos(dphi))"){
      if(mc){
        y = dphiymc;
        e = dphiemc;
      }else{
        y = dphiy;
        e = dphie;
      }
    }


    h->SetBinContent(i+1,y[i]);
    h->SetBinError(i+1,e[i]);

  }


  //  h->GetXaxis()->CenterTitle();
  //  h->GetYaxis()->CenterTitle();


  return h;

}




void compareToPaper(){

  string vars[] = {"xj","rj","subrj","acos(cos(dphi))"};
  string algos[] = {"akPu4PF","akPu3PF","akVs3PF","akPu3Calo","akVs3Calo"};

  pp = 1;
  peri = 0;

  mc = 0;
  comparePlot("aj","akPu3PF");


  mc = 1;
  comparePlot("aj","akPu3PF");

  return;
  pp = 0;
  peri = 1;

  mc = 0;
  comparePlot("aj","akPu3PF");
  mc = 1;
  comparePlot("aj","akPu3PF");

  pp = 0;
  peri = 0;

  mc = 0;
  comparePlot("xj","akPu3PF");
  mc = 1;
  comparePlot("xj","akPu3PF");

  density = 1;
  mc = 0;
  comparePlot("xj","akPu3PF");
  mc = 1;
  comparePlot("xj","akPu3PF");




  if(0){
  for(int i = 0; i < 4; ++i){
    for(int j = 0; j < 3; ++j){
      mc = 1;
      comparePlot(vars[i],algos[j]);
      mc = 0;
      comparePlot(vars[i],algos[j]);
    }
  }
  }
}


