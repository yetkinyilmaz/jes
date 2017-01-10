
#include "plotJES_Absolute.C"
#include "plotJES_Subleading.C"
#include "plotJES_Function.C"

void plotJES_Full(){


  double c = 1.03;
  double s = 0.;
  double n = 0.;
  plotJES_Subleading(c,s,n,"xj",0);
  plotJES_Subleading(c,s,n,"xj",1);
  plotJES_Subleading(c,s,n,"refxj",1);
  plotJES_Subleading(c,s,n,"genxj",1);
  plotJES_Subleading(c,s,n,"refpt1",1);
  plotJES_Subleading(c,s,n,"genpt1",1);
  plotJES_Subleading(c,s,n,"refpt2",1);
  plotJES_Subleading(c,s,n,"genpt2",1);

  plotJES_Function(c,s,n);

  c = 1.06;
  s = -4.;
  n = 20;

  plotJES_Function(c,s,n);

  return;
  plotJES_Absolute(c,"xj",0);
  plotJES_Absolute(c,"xj",1);
  plotJES_Absolute(c,"refxj",1);
  plotJES_Absolute(c,"genxj",1);

  plotJES_Absolute(c,"refpt1",1);
  plotJES_Absolute(c,"refpt2",1);
  plotJES_Absolute(c,"genpt1",1);
  plotJES_Absolute(c,"genpt2",1);

  c = 0.94;
  plotJES_Absolute(c,"xj",0);
  plotJES_Absolute(c,"xj",1);
  plotJES_Absolute(c,"refxj",1);
  plotJES_Absolute(c,"genxj",1);

  plotJES_Absolute(c,"refpt1",1);
  plotJES_Absolute(c,"refpt2",1);
  plotJES_Absolute(c,"genpt1",1);
  plotJES_Absolute(c,"genpt2",1);



}

