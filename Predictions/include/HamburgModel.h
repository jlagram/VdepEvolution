#include "TMath.h"
#include <iostream>

float ZeroC = 273.15;// in K

// Hamburg Model

const double k_B = 8.617343183775136189e-05; // eV/K
const double q_el = 1.60217653e-19; // C
const double eps0 = 8.854187817e-14; // As/Vcm
const double eps = 11.9;

const double ga = 1.81E-2;  // cm-1 //TDR p96 : 1.54e-2  //dierl : 1.81e-2
const double gY = 5.16E-2;  // cm-1 //        : 4.6e-2   //     :5.16e-2

const double _dI_cut = 0.00001; // 0.000000001

const double a10 =  1.23e-17; // A/cm


double I_a1(double T, double t, int days)
{
    double k0 = 1.2e13; // 1/s
    double E = 1.11; // eV
    double invt =  k0 * TMath::Exp(-E/(k_B*T)) * 60 * 60 * 24 * days * t;
    return TMath::Exp(- invt);
}

double I_a2(double T)
{
    return -8.9e-17 + 4.6e-14/T; // A/cm
}

double I_a3(double t, int days)
{
    double b = 3.07e-18; //A/cm
    return -b * TMath::Log(t*days*24*60); // with t0=1min
}

double LeakCorrection(double T1, double T2)
{
    double E = 1.21; // formerly 1.12 eV
    return (T1/T2)*(T1/T2)*TMath::Exp(-E/(2*k_B)*(1/T1 - 1/T2));
}

double N_stable(double Neff0, double Feq, Bool_t verbose=false){
    // Nc:
    //double Nc0_c = 7.5E-2; // cm-1
    double rc = 0.9;  //  Moll: rc=0.69
    double Neff0_c = 10.9E-2; // cm-1
    double c = Neff0_c / fabs(Neff0);
    double Nc0 = rc * Neff0_c / c;
    double gc = 1.6E-2; // cm-1  Moll:1.49 // TDR p96 : 1.77e-2 //dierl 1.6e-2
    double Nc = Nc0 * (1.-TMath::Exp(-c*Feq)) + gc*Feq;
    if(verbose) printf("Stable: %e (%f, %e)\n", Nc, Neff0,Feq);
    return  Nc;
}

// a is time in days
double N_short(double t, double T, double Ni, int a, Bool_t verbose=false){
    //    Na:
    double tau_a = 1./(2.4E13 * TMath::Exp(-1.09/(k_B*T)));  // s-1 //TDR 2.3e13, 1.08 //dierl 2.4e13 1.09
    double Na = Ni * TMath::Exp(-t*(24.*60.*60.*a)/tau_a);
    if(verbose && (Na<0)) printf("Short: %e (%f, %f, %e, %e)\n",Na, t, T,Ni,tau_a);
    return Na;
}

double N_reverse2(double t, double T, int a, Bool_t verbose=false){
    // NY:
    double tau_Y = 1./(1.5E15*TMath::Exp(-1.33/(k_B*T)));  // s-1
    double NY =   1./(1.+t*(24.*60.*60.*a)/tau_Y);
    //  std::cout << "NY in function N_reverse2 " << NY << std::endl;
    if(verbose) printf("Reverse: %e (%f, %f)\n",NY, t, T);
    return NY;
}

// in 20C scaling dIleak should be constant, only temp is amplified
int Ileak_Cavity(double dIleak, double &temp, Float_t dtdp, double abs_Ileak){
    
    Bool_t debug=false;
    if(dIleak<_dI_cut) return -1;
    if(debug)  std::cout << " cavity called with " << dIleak << " " << temp << std::endl;
    double orig_temp = temp;
    double abs_Ileak_atT = LeakCorrection(orig_temp,293.16)*(abs_Ileak+dIleak);
    double dI = LeakCorrection(orig_temp,293.16)*dIleak;
    double new_temp;
    while(dI>_dI_cut){
        if (debug) std::cout << " CORRECTING TEMPERATURE by " << dI*0.300*dtdp << std::endl;
        new_temp = temp + dI*0.300*dtdp;
        dI = LeakCorrection(new_temp,temp)*(abs_Ileak_atT)-abs_Ileak_atT;
        temp=new_temp;
        abs_Ileak_atT = LeakCorrection(temp,293.16)*(abs_Ileak+dIleak);
        if(  abs_Ileak_atT >12 ) {//12
            std::cout << " Current limit reached!!! "<< abs_Ileak_atT <<"mA"<< std::endl;
            return 0;//0 //JLA
        }
    }
    if(debug)  std::cout << "cavity finished with " << dIleak << " " << temp << std::endl;
    return 1;
}
