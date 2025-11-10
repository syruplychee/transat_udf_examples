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

double template_class_name::latent_heat(double P) {
   // any expression may be worked out to set latent heat within this function
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
   // here the latent heat is set to a constant value
   // the "LH" variable is returned as the result

   double LH = 2.2e6;
   return LH;
}

