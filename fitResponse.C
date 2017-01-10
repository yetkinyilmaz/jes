#include "analyzeResponseEta.C"

static double centCenters[] = {4.5, 14.5, 24.5, 34.5, 44.5, 54.5, 79.5, 119.5, 169.5};

void fitResponse(){

  shiftToReco = 1;

  int plotPar = 3;

  TGraphErrors* g[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  TF1* f[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  TF1* fex[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  int N = 9;

  int etaBin = 0;

  double fit0[] = {0.006,0.006,0.006,0.006};

  //  double fit1_0[] = {114,145,160,170};
  //  double fit1_1[] = {0.0026,-0.0016,-0.0025,-0.0006};
  //  double fit1_2[] = {0.0001,0.00007,0.00006,0.00001};

  double fit1_0[] = {120,130,160,170};
  double fit1_2[] = {0.0001,0.0001,5.8E-05,8.6E-06};


  //  double fit2_0[] = {-1.6,-2,-2,-1.6};
  //  double fit2_1[] = {-0.006,-0.003,-0.003,-0.006};

  //  double fit2_0[] = {-2,-2,-2,-2};
  //  double fit2_1[] = {-0.003,-0.003,-0.003,-0.003};

  //  double fit2_0[] = {-1.6,-1.6,-1.6,-1.6};
  //  double fit2_1[] = {-0.006,-0.006,-0.006,-0.006};


  //  double fit2_0[] = {4.2,6.1,5.8,6.4};
  //  double fit2_1[] = {-0.01,-0.052,-0.046,-0.048};

  double fit2_0[] = {7.57,11.53,13.42,15.00};
  double fit2_1[] = {-0.01,-0.19,-0.22,-0.23};

  TGraphErrors* gFit = new TGraphErrors(N);


  string formula = "1.-[0]*exp([1]*(x-[2]))";
  //  formula = "1.-10.*[0]*exp(-[0]*x)";
  //  formula = "1.-10.*[0]*exp(-0.01*x*x) + [1]*pow((x),[2])";

  //  formula = "1.-[0]*exp(-[1]*x)+[2]*exp(-[3]*x)";
  //  formula = "1.-[0]*exp(-[1]*x)+[2]/(x-[3])";

  formula = "[0]-[1]*exp(-[2]*x)+[3]/(x-[4])/(x-[4])";

  for(int i = N-1; i >= 0; --i){

    g[i] = analyzeEtaBin(etaBin,i);
    g[i]->SetName(Form("g_%d",i));
    g[i]->SetMarkerStyle(24);
    g[i]->SetMarkerColor(i+1);
    g[i]->SetLineColor(i+1);

    double xMin,xMax,yMin,yMax;
    g[i]->GetPoint(2,xMin,yMin);
    g[i]->GetPoint(7,xMax,yMax);

    f[i] = new TF1(Form("f_%d",i),formula.data(),xMin-5,xMax+5);
    fex[i] = new TF1(Form("fex_%d",i),formula.data(),0,500);
    fex[i]->SetLineStyle(2);


    f[i]->FixParameter(0,1.);
    f[i]->FixParameter(1,0.15);
    f[i]->FixParameter(2,0.01);
    //    f[i]->FixParameter(3,0);
    f[i]->FixParameter(4,15);

    f[i]->SetParLimits(3,0,400);
    f[i]->SetLineColor(i+1);
    fex[i]->SetLineColor(i+1);

    g[i]->Fit(f[i],"R");

    fex[i]->SetParameters(f[i]->GetParameters());
    fex[i]->Draw("same");

    gFit->SetPoint(i,centCenters[i],f[i]->GetParameter(plotPar));
    gFit->SetPointError(i,0,f[i]->GetParError(plotPar));

  }



  TH2D* pad = new TH2D("pad","",400,0,400,200,0.5,1.5);
  pad->Draw();
  for(int i = N-1; i >= 0; --i){
    g[i]->Draw("p same");
    fex[i]->Draw("same");
  }

  TCanvas* c2 = new TCanvas("c2","c2",600,600);
  gFit->Draw("Ap");

  TF1* fPar;

  if(plotPar == 1){
    fPar = new TF1("fPar","[0]*(1+[1]*x*x)",0,200);
    fPar->FixParameter(1,0.0001);
  }else{
    fPar = new TF1("fPar","[0]*exp([1]*x)",0,50);
  }

  fPar->SetParameters(5.63,-0.0455);

  gFit->Fit("fPar","R");
  gFit->SetName("gFit");

  fcent = new TF1("fcent","expo",0,200);
  fcent->SetParameters(5.63,-0.0455);
  fcent->SetLineColor(3);
  fcent->SetLineStyle(2);
  fcent->Draw("same");

  gFit->Write();
  c2->Print("jec_vs_cent.png");

}
