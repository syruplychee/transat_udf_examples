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
   size_t nblocks = tmb::Constants::Nblocks();
   unsigned nphases = tmb::Constants::Nphases();

   _hershel_bulkley_tau.resize(nphases);
   _hershel_bulkley_k.resize(nphases);
   _hershel_bulkley_n.resize(nphases);

   for (unsigned iphase = 0; iphase < nphases; ++iphase) {
      _hershel_bulkley_tau[iphase] = get_double("hershel_bulkley_tau", iphase);
      _hershel_bulkley_k[iphase] = get_double("hershel_bulkley_k", iphase);
      _hershel_bulkley_n[iphase] = get_double("hershel_bulkley_n", iphase);
   }
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

double template_class_name::Viscosity(tmb::Properties_Manager* mgr, unsigned iphase) {
   // any expression may be worked out to set Viscosity within this function
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

   // here the Viscosity is calculated from the Shear rate
   //  using Hershel Bulkley Non Newtonian model
   //
   // the "visc" variable is returned as the result

   double visc = _get(mgr->get<tmb::Shear_Rate>(iphase), iphase);
   return visc;
}


double template_class_name::_get(double shear_rate,
                                               unsigned iphase) {
   return _hershel_bulkley_tau[iphase] / (shear_rate + _small) +
          _hershel_bulkley_k[iphase] *
              std::pow(shear_rate + _small, _hershel_bulkley_n[iphase] - 1);
}
