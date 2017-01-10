#include "Analysis.h"


TF1* plotJES_Function(
		      double c = 1.1,
		      double s = -4.,
		      double n = 20.
		      ){
  
  TF1* f = new TF1("f","[0]+[1]/sqrt(x)+[2]/x",0,300);
  f->SetParameters(c,s,n);

  TH2D* h = new TH2D("h","",300,0,300,100,0.8,1.2);

  TCanvas* c1 = new TCanvas("c1","c1",600,600);

  h->Draw();
  f->Draw("same");

  drawLine(300);
 
  c1->Print(Form("figure_variation_c%d_s%d_n%d.png",int(c*100),abs(int(s)),abs(int(n)))); 
  return f;

}



