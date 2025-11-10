/*
 * template_class_name.cxx
 *
 */

#include "template_class_name.h"
#include "cppinterface.h"
#include <cmath>

// Class Constructor
template_class_name::template_class_name() {
   std::cout << "constructed template_class_name" << std::endl;
   /**
    * We don't want to access system pressure for every point in the domain
    * because it a constant for the whole domain
    * However, it can change with time.
    */
   set_pointer(&(_system_pressure),"system_pressure");
   /**
    * We want to avoid the situtation of a number divided by zero, that's why
    * we always divide by a variable after adding the parameter small whose
    * value
    * is specified in shared_modules/constants.f90
    */
   _small = tmb::Constants::Small();
   _smdiff = tmb::Constants::Smdiff();
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
   //double E = mgr->get<tmb::Energy>(iphase);                           //=
   //double T = mgr->get<tmb::Temperature>(iphase);                      //=
   //double P = mgr->get<tmb::Pressure>(iphase) + *(_system_pressure);   //=
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
   //double Gamma    = 1.4;                                              //=
   // ====================================================================
   // here the density is set to a constant value
   // the "dens" variable is returned as the result
   double dens = 1.205;
   return dens;


}
double template_class_name::DrhoDp_t(tmb::Properties_Manager* mgr, unsigned iphase) {
   double drhodp_t = 0;
   return drhodp_t;
}
double template_class_name::DrhoDt_p(tmb::Properties_Manager* mgr, unsigned iphase) {
   double drhodt_p = 0;
   return drhodt_p;
}

double template_class_name::DrhoDp(tmb::Properties_Manager* mgr, unsigned iphase) {
   double drhodp_t = mgr->get<tmb::DrhoDp_t>(iphase);
   double drhodt_p = mgr->get<tmb::DrhoDt_p>(iphase);
   double rho      = mgr->get<tmb::Density>(iphase);
   double cp       = mgr->get<tmb::Cp>(iphase);
   
   // DEFAULT FORMULATION: this is the standard definition of DrhoDP
   //                      not needed to be defined or set
   //                      DrhoDP_t & DrhoDT_P should be set (i.e. calculated from a table/curve fit, or formulation)
   //                      DrhDP only need be reformulated if user wants something different and knows what they are doing
   return drhodp_t + drhodt_p/(rho*cp+_small);
}
double template_class_name::Soundspeed(tmb::Properties_Manager* mgr, unsigned iphase) {
   double drhodp_s = mgr->get<tmb::DrhoDp>(iphase);

   // DEFAULT FORMULATION: this is the standard definition of Sound Speed
   //                      Not necessary to adjust this. Drhodp_s is calculated from DrhoDp_t and DrhoDt_p
   //                      DrhoDP_t & DrhoDT_P should be set (i.e. calculated from a table/curve fit, or formulation)
   //                      For incompressible DrhoDp_T = 0 & DrhoDt_p = 0, sound speed = inf
   //                      But, Sound Speed is only for post processing. Can provide custom formulation if desired
   return std::sqrt(1.0/(drhodp_s+_small));
}
