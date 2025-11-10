/*
 * template_class_name.cxx
 *
 */

#include "template_class_name.h"
#include "cppinterface.h"
#include <cmath>
#include "utility_functions.decl.h"
#include "cppinterface.decl.h"
#include <cmath>
#include <algorithm>


// Class Constructor
template_class_name::template_class_name() {
   std::cout << "constructed template_class_name" << std::endl;

   /**
    * We want to avoid the situtation of a number divided by zero, that's why
    * we always divide by a variable after adding the parameter small whose
    * value
    * is specified in shared_modules/constants.f90
    */
   _small = tmb::Constants::Small();
   _smdiff = tmb::Constants::Smdiff();

   /**
    * We don't want to access system pressure for every point in the domain
    * because it a constant for the whole domain
    * However, it can change with time.
    */
   set_pointer(&(_system_pressure),"system_pressure");

}

// Class Destructor
template_class_name::~template_class_name() {
   std::cout << "destructed template_class_name" << std::endl;
}

double template_class_name::Density(tmb::Properties_Manager* mgr, unsigned iphase) {
   // any expression may be worked out to set density within this function
   //
   // access other variables using the Properties Manager:
   //  (uncomment those needed to use the variables)
   // ====================================================================
   double E = mgr->get<tmb::Energy>(iphase);                             //=
   //double T = mgr->get<tmb::Temperature>(iphase);                      //=
   double P = mgr->get<tmb::Pressure>(iphase) + *(_system_pressure);     //=
   //double Mw = 18.015;                                                 //= 
                                                                         //=
   //double density  = mgr->get<tmb::Density>(iphase);                   //=
   //double visc     = mgr->get<tmb::Viscosity>(iphase);                 //=
   //double drhodp_t = mgr->get<tmb::DrhoDp_t>(iphase);                  //=
   //double drhodt_p = mgr->get<tmb::DrhoDt_p>(iphase);                  //=
   //double drhodp   = mgr->get<tmb::DrhoDp>(iphase);                    //=
   //double cp       = mgr->get<tmb::Cp>(iphase);                        //=
   //double lambda   = mgr->get<tmb::Lambda>(iphase);  //thermal conductivity//=
   //double ss       = mgr->get<tmb::Soundspeed>(iphase);                //=
   //double R        = tmb::Constants::GasConstant();                    //=
   double Gamma    = 1.4;                                                //=
   // ====================================================================

   //            P
   // Den = -------------
   //       E*(Gamma-1.0)
   P = std::max(_small,P);
   // here the density is calculatd from Ideal Gas
   // the "dens" variable is returned as the result
   double dens = P / (E * (Gamma - 1.0) + _small);
   return dens;

}
double template_class_name::DrhoDp_t(tmb::Properties_Manager* mgr, unsigned iphase) {
   //  drho      1.0
   // ------  = -----
   //   dp      (R*T)
   //     t
   double T = mgr->get<tmb::Temperature>(iphase);
   double R = tmb::Constants::GasConstant() * 1000.0 /
              mgr->get<tmb::MolecularWeight>(iphase);

   return 1.0 / (R * T + _small);

}
double template_class_name::DrhoDt_p(tmb::Properties_Manager* mgr, unsigned iphase) {
   //  drho         -P
   // ------  = -----------
   //   dT       (R*T**2)
   //     p
   double P = mgr->get<tmb::Pressure>(iphase) + *(_system_pressure);
   double T = mgr->get<tmb::Temperature>(iphase);
   double R = tmb::Constants::GasConstant() * 1000.0 /
              mgr->get<tmb::MolecularWeight>(iphase);
   P = std::max(_small,P);

   return -P / (R * std::pow(T, 2) + _small);
}
double template_class_name::DrhoDp(tmb::Properties_Manager* mgr, unsigned iphase) {
   //  drho       1.0
   // ----- = -----------
   //   dp     (R*Gamma*T)
   //     s
   double T = mgr->get<tmb::Temperature>(iphase);
   double Gamma = 1.4;
   double R = tmb::Constants::GasConstant() * 1000.0 /
              mgr->get<tmb::MolecularWeight>(iphase);

   // DEFAULT FORMULATION: this is the standard definition of DrhoDP
   //                      not needed to be defined or set
   //                      DrhoDP_t & DrhoDT_P should be set (i.e. calculated from a table/curve fit, or formulation)
   //                      DrhDP only need be reformulated if user wants something different and knows what they are doing
   //return drhodp_t + drhodt_p/(rho*cp+_small);

   // in this case, replaced the default formulation with Ideal Gas 
   return 1.0 / (R * Gamma * T + _small);
}
double template_class_name::Soundspeed(tmb::Properties_Manager* mgr, unsigned iphase) {

   // a = sqrt(Gamma*R*T)
   double T = mgr->get<tmb::Temperature>(iphase);
   double Gamma = 1.4;
   double R = tmb::Constants::GasConstant() * 1000.0 /
              mgr->get<tmb::MolecularWeight>(iphase);


   // DEFAULT FORMULATION: this is the standard definition of Sound Speed
   //                      Not necessary to adjust this. Drhodp_s is calculated from DrhoDp_t and DrhoDt_p
   //                      DrhoDP_t & DrhoDT_P should be set (i.e. calculated from a table/curve fit, or formulation)
   //                      For incompressible DrhoDp_T = 0 & DrhoDt_p = 0, sound speed = inf
   //                      But, Sound Speed is only for post processing. Can provide custom formulation if desired
   //double drhodp_s = mgr->get<tmb::DrhoDp>(iphase);
   //return std::sqrt(1.0/(drhodp_s+_small));

   // in this case, replaced the default formulation with Ideal Gas 
   return std::sqrt(Gamma * R * T);
}
