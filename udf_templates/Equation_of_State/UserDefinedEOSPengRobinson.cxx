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
   std::cout << "contructed template_class_name" << std::endl;
   _STATE_gas    = get_integer("STATE_gas");
   _STATE_liquid = get_integer("STATE_liquid");

   _EN_antoine         = get_integer("EN_antoine");
   _EN_satcurv_water   = get_integer("EN_satcurv_water");
   _EN_satcurv_ammonia = get_integer("EN_satcurv_ammonia");

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

   int nphases = 1; 
   // Only 1 phase, so no looping necessary
   // only push back to set iphase = 0
   _k.push_back(0.39234);
   _VT.push_back(0);
   _Pc.push_back(4.61e+06);
   _Tc.push_back(190.564);
   _state_of_phase.push_back(_STATE_gas);
   _saturation_curve_method.push_back(_EN_antoine);

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
   double T = mgr->get<tmb::Temperature>(iphase);                        //=
   double P = mgr->get<tmb::Pressure>(iphase) + *(_system_pressure);     //=
   double Mw = 18.015;                                                   //= 
                                                                         //=
   //double density  = mgr->get<tmb::Density>(iphase);                   //=
   //double visc     = mgr->get<tmb::Viscosity>(iphase);                 //=
   //double drhodp_t = mgr->get<tmb::DrhoDp_t>(iphase);                  //=
   //double drhodt_p = mgr->get<tmb::DrhoDt_p>(iphase);                  //=
   //double drhodp   = mgr->get<tmb::DrhoDp>(iphase);                    //=
   //double cp       = mgr->get<tmb::Cp>(iphase);                        //=
   //double lambda   = mgr->get<tmb::Lambda>(iphase);  //thermal conductivity//=
   //double ss       = mgr->get<tmb::Soundspeed>(iphase);                //=
   double R        = tmb::Constants::GasConstant();                      //=
   //double Gamma    = 1.4;                                              //=
   // ====================================================================
   std::vector<double> roots;
   double specificVolume, bc, ac, alpha, pinv;
   double acubic, bcubic, ccubic;

   Mw = 0.001 * Mw;

   P = std::max(_small,P);
   _bound(&P, &T, mgr, iphase);
   
   bc = 0.077796*R*_Tc[iphase]/_Pc[iphase];
   ac = 0.45724*(R*_Tc[iphase])*(R*_Tc[iphase])/_Pc[iphase];
   alpha = std::pow(1.0 + _k[iphase]*(1.0 - sqrt(T/_Tc[iphase])),2);
 
   pinv = 1.0/P;
   acubic =  pinv*(P*bc - R*T);
   bcubic = -3.0*bc*bc- pinv*(2.0*bc*R*T -ac*alpha);
   ccubic =  bc*bc*bc + pinv*( bc*bc*R*T -ac*alpha*bc);

   roots.resize(3,0.0);
   UTILITY_FUNCTIONS::cuberoots(acubic,bcubic,ccubic,roots);

   if (roots[1] < _smdiff){
     // Only one solution, roots[0]
     specificVolume = roots[0];
   } 
   else {
      if (_state_of_phase[iphase] == _STATE_liquid) { // liquid
         specificVolume = *min_element(roots.begin(),roots.end());
      } else if (_state_of_phase[iphase] == _STATE_gas) {
         specificVolume = *max_element(roots.begin(),roots.end());
      } else {
         const std::string state_of_phase_error;
         std::string message("State of phase not set properly to use User Defined EOS. Check that phases using User Defined are either a liquid or a gas.");
         message.append(state_of_phase_error.c_str());
         soft_stop_cxx(message.c_str());
      }
   }

   // here the density is calculatd from Peng Robinson
   // the "dens" variable is returned as the result
   double dens = std::max(1.0e-9,Mw/(specificVolume-_VT[iphase]));
   return dens;

}
double template_class_name::DrhoDp_t(tmb::Properties_Manager* mgr, unsigned iphase) {
   double specificVolume, bc, ac, alpha;
   double Mw, Tr;
   double denom, dpdv_t;

   Mw = mgr->get<tmb::MolecularWeight>(iphase); //Mw: Molecular weight     (kg/mol)
   Mw = 0.001 * Mw;

   double T   = mgr->get<tmb::Temperature>(iphase);
   double R   = tmb::Constants::GasConstant();
   double Den = mgr->get<tmb::Density>(iphase);
   double P   = mgr->get<tmb::Pressure>(iphase) + *(_system_pressure);
   P = std::max(_small,P);
   _bound(&P, &T, mgr, iphase);


   specificVolume = Mw/Den + _VT[iphase];
   Tr = T/_Tc[iphase];

   bc = 0.077796*R*_Tc[iphase]/_Pc[iphase];
   ac = 0.457240*(R*_Tc[iphase])*(R*_Tc[iphase])/_Pc[iphase];
   alpha = std::pow(1.0 + _k[iphase]*(1.0 - std::sqrt(Tr)),2);
 
   denom = std::pow(specificVolume,2) + 2.0*bc*specificVolume - bc*bc;
   dpdv_t = -R*T/std::pow(specificVolume - bc,2) + 2.0*alpha*ac 
            *(specificVolume + bc)/(std::pow(denom,2)+_small);

   return -std::pow(Den,2)/(Mw*dpdv_t + _small);

}
double template_class_name::DrhoDt_p(tmb::Properties_Manager* mgr, unsigned iphase) {
   double specificVolume, bc, ac, alpha, sqrtAlpha;
   double Mw, Tr;
   double denom, dpdt_v, dpdv_t, dvdt_p;

   Mw = mgr->get<tmb::MolecularWeight>(iphase); //Mw: Molecular weight     (kg/mol)
   Mw = 0.001 * Mw;

   double T   = mgr->get<tmb::Temperature>(iphase);
   double R   = tmb::Constants::GasConstant();
   double Den = mgr->get<tmb::Density>(iphase);
   double P   = mgr->get<tmb::Pressure>(iphase) + *(_system_pressure);
   P = std::max(_small,P);
   _bound(&P, &T, mgr, iphase);

   specificVolume = Mw/Den + _VT[iphase];
   Tr = T/_Tc[iphase];

   bc = 0.077796*R*_Tc[iphase]/_Pc[iphase];
   ac = 0.457240*(R*_Tc[iphase])*(R*_Tc[iphase])/_Pc[iphase];
   sqrtAlpha = 1.0 + _k[iphase]*(1.0 - std::sqrt(Tr));
   alpha = sqrtAlpha*sqrtAlpha;
   denom = std::pow(specificVolume,2) + 2.0*bc*specificVolume - bc*bc;
 
   dpdt_v = R/(specificVolume - bc) + alpha*ac*(specificVolume + bc)*_k[iphase]
           *sqrtAlpha/(_Tc[iphase]*std::sqrt(Tr)*denom + _small);
   dpdv_t = -R*T/std::pow(specificVolume - bc,2) + 2.0*alpha*ac 
            *(specificVolume + bc)/(std::pow(denom,2)+_small);
   dvdt_p = -dpdt_v/(dpdv_t + _small);

   return -std::pow(Den,2)*dvdt_p/Mw;

}

double template_class_name::DrhoDp(tmb::Properties_Manager* mgr, unsigned iphase) {
   double drhodp_t = mgr->get<tmb::DrhoDp_t>();
   double drhodt_p = mgr->get<tmb::DrhoDt_p>();
   double rho      = mgr->get<tmb::Density>();
   double cp       = mgr->get<tmb::Cp>(iphase);

   // DEFAULT FORMULATION: this is the standard definition of DrhoDP
   //                      not needed to be defined or set
   //                      DrhoDP_t & DrhoDT_P should be set (i.e. calculated from a table and interplated, curve fit, or formulation)
   //                      DrhDP only need be reformulated if user wants something different and knows what they are doing
   return drhodp_t + drhodt_p/(rho*cp+_small);
}

double template_class_name::Soundspeed(tmb::Properties_Manager* mgr, unsigned iphase) {
   // This should be drho/dp|s
   double drhodp_t = mgr->get<tmb::DrhoDp_t>();
   double drhodt_p = mgr->get<tmb::DrhoDt_p>();
   double rho      = mgr->get<tmb::Density>();
   double T        = mgr->get<tmb::Temperature>(iphase);
   double cp       = mgr->get<tmb::Cp>(iphase);
   double P        = 0; // Dummy, Pressure not used in ss, but needed for bound
   _bound(&P, &T, mgr, iphase);

   // DEFAULT FORMULATION: this is the standard definition of Sound Speed
   //                      Not necessary to adjust this. Drhodp_s is calculated from DrhoDp_t and DrhoDt_p
   //                      DrhoDP_t & DrhoDT_P should be set (i.e. calculated from a table/curve fit, or formulation)
   //                      For incompressible DrhoDp_T = 0 & DrhoDt_p = 0, sound speed = inf
   //                      But, Sound Speed is only for post processing. Can provide custom formulation if desired
   //double drhodp_s = mgr->get<tmb::DrhoDp>(iphase);
   //return std::sqrt(1.0/(drhodp_s+_small));
   
   // in this case, replaced the default formulation
   double drhodp_s = std::max(_small,drhodp_t - (T/rho)*std::pow(drhodt_p,2)/(rho*cp + _small));
   return std::sqrt(1.0/drhodp_s);
}

// PRIVATE FUNCTION
//     this is specific bound function necessary only for Peng Robinsony
//     (to avoid this extra code in the main routine) 
//
// Take care of metastable states by bounding P or T
//   If phase == STATE_LIQUID adjust pressure since density depends less on pressure
//      for liquids
//   If phase == STATE_GAS    adjust temperature for gases
void template_class_name::_bound(double *P, double *T, tmb::Properties_Manager* mgr,
                                                  unsigned iphase) {
   // Error checking done in density calculation (this statement is only if sat_curve)
   if (_saturation_curve_method[iphase] == _EN_antoine) { //sat_curve is antoine
      if (_state_of_phase[iphase] == _STATE_liquid) { // Liquid
         double Psat = get_saturation_pressure_antoine(*T,iphase);
         *P = std::max(*P, Psat);
      } else if (_state_of_phase[iphase] == _STATE_gas) { // Gas
         double Tsat = get_saturation_temperature_antoine(*P,iphase);
         *T =  std::max(*T, Tsat);
      }
   }
   else if (_saturation_curve_method[iphase] == _EN_satcurv_water) {
      if (_state_of_phase[iphase] == _STATE_liquid) { // Liquid
         double Psat = get_saturation_pressure_water(*T);
         *P = std::max(*P, Psat);
      } else if (_state_of_phase[iphase] == _STATE_gas) { // Gas
         double Tsat = get_saturation_temperature_water(*P);
         *T =  std::max(*T, Tsat);
      }
   }
   else if (_saturation_curve_method[iphase] == _EN_satcurv_ammonia) {
      if (_state_of_phase[iphase] == _STATE_liquid) { // Liquid
         double Psat = get_saturation_pressure_ammonia(*T);
         *P = std::max(*P, Psat);
      } else if (_state_of_phase[iphase] == _STATE_gas) { // Gas
         double Tsat = get_saturation_temperature_ammonia(*P);
         *T =  std::max(*T, Tsat);
      }
   }
}
