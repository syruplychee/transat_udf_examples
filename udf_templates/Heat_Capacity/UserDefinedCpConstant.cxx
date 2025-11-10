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

double template_class_name::Cp(tmb::Properties_Manager* mgr, unsigned iphase) {
   // any expression may be worked out to set Cp within this function
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

   // here the Cp is set to a constant value
   // the "cp" variable is returned as the result
   double cp = 1005;
   return cp;
}
