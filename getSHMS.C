#include "SHMSMagnets.h"

////////////////////
// global variables:
////////////////////
const double tol = 1E-5;
int debug = 0;

//////////////////////////////////////
// input the desired momentum in [GeV]
//////////////////////////////////////
int main(int argc, char *argv[]){

  double P = atof(argv[1]);
  P = abs(P);

  double eta[5] = {10000, 10000, 10000, 10000, 10000};
  int ii[5] = {0, 0, 0, 0, 0};
  double I_iter[5] = {calcLinearHB(P), calcLinearQ1(P), calcLinearQ2(P), calcLinearQ3(P), calcLinearDip(P)};
  double I_linear[5] = {calcLinearHB(P), calcLinearQ1(P), calcLinearQ2(P), calcLinearQ3(P), calcLinearDip(P)};
  double I_max[5] = {3600, 2400, 3660, 2480, 3200};

  char const *magnets[5] = {"HB","Q1","Q2","Q3","Dipole"};

  static double (*cpratio[5])(double a, double b) = {calcPratioHB,calcPratioQ1,calcPratioQ2,calcPratioQ3,calcPratioDip};
  static double (*bratio[5])(double a) = {calcBetaRatioHB,calcBetaRatioQ1,calcBetaRatioQ2,calcBetaRatioQ3,calcBetaRatioDip};
  static double (*lratio[5])(double a) = {calcLeffRatioHB,calcLeffRatioQ1,calcLeffRatioQ2,calcLeffRatioQ3,calcLeffRatioDip};

  static double (*error[5])(double a) = {calcErrorHB,calcErrorQ1,calcErrorQ2,calcErrorQ3,calcErrorDip};
  cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
  cout<<"If increasing momentum, go to Minimum On Loop (MOL) current first, then to Iset!"<<endl;

  for (int kk=0; kk<5; kk++){
  
    while (fabs(eta[kk])>tol){
      eta[kk] = cpratio[kk](P, I_iter[kk]);
      if (eta[kk] > 0) {I_iter[kk] = I_iter[kk]*(1-fabs(eta[kk])/2.0);}
      else {I_iter[kk] = I_iter[kk]*(1+fabs(eta[kk])/2.0);}
      
      ii[kk]++;
      if (ii[kk] > 100) {break;}
    }

    cout<<"Magnet:\t"<<magnets[kk]<<"\tVersion:\t"<<getVersion(kk)<<endl;

    double I_ramp = I_iter[kk]+300;
    if (I_ramp > I_max[kk]){I_ramp = I_max[kk];}
    if (kk==1 && I_iter[kk]<1200){I_ramp = 1500;}

    if (debug == 1){
      cout<<"\tRequested energy:\t"<<P<<" [GeV]"<<endl;
      cout<<"\tRecommended Iset: "<<I_iter[kk]<<" [A], from linear approx: "<<I_linear[kk]<<" [A]."<<endl;
      cout<<"\tbeta ratio: "<<bratio[kk](I_iter[kk])<<" leff ratio: "<<lratio[kk](I_iter[kk])<<endl;
      cout<<"\tResulting difference from nominal: "<<eta[kk]<<endl;
      cout<<"\tUncertainty: +/-"<<error[kk](I_iter[kk])<<"[%]"<<endl;
    }
    else{
      if (P<0.01){
	cout<<"\tIset: "<<0.0<<" [A]. MOL current: "<<0.0<<" [A]."<<endl;
	cout<<" "<<endl;
      }
      else{
	cout<<"\tIset: "<<I_iter[kk]<<" [A]. MOL current: "<<I_ramp<<" [A]."<<endl;
	cout<<" "<<endl;
      }
    }
  }
  cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;

  return 0;

}
