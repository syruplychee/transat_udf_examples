/*
 * template_class_name.cxx
 *
 */

#include "template_class_name.h"
#include "cppinterface.h"
#include <cmath>

// Class Constructor
template_class_name::template_class_name() {
   _small = get_double("small");
}

// Class Destructor
template_class_name::~template_class_name() {
}

// Location to implement UDF if Drag is only interphase model (no Lift, or Wall Lubrication)
// see more details in header file (template_class_name.h).
void template_class_name::operator()(double &ff, double &ffD, double vel_slip_mag, double Re, double Eo) {

   // Define some constants needed in the expression for tomiyama slightly contaminated
   double Re_const = Re / (vel_slip_mag + _small);
   double constA = pow(Re_const, 0.687);
   double constB = Re_const * Eo / (9 * (Eo + 4));
   double fun0   = 1 + 0.15 * pow(Re, 0.687);
   double v_crit = pow((2/(0.15*constA)),(1./0.687));
   double ff1_blend = 0.5 + 0.5 * std::tanh(5*(vel_slip_mag-v_crit)/v_crit);
   double ff1 = ff1_blend*3 + (1-ff1_blend)*fun0;
   double ff2 = constB * std::abs(vel_slip_mag);
   double fun1 = 0, fun2 = 0;

   // Calculate ff based on constants
   ff  = std::max(ff1,ff2);

   // Calculate derivative of friction factor (ffD), for Newton-Raphson method
   if (ff1 > ff2) {
      fun1 = 0.10305 * constA * pow(std::abs(vel_slip_mag), -0.313);
      fun2 = 2.5*(1-v_crit)/v_crit / (pow(std::cosh(5*(vel_slip_mag-v_crit)/v_crit), 2) + _small);
      ffD  = vel_slip_mag / (std::abs(vel_slip_mag) + _small) * (fun2*3 + fun1 - fun1*ff1_blend - fun0*fun2);
   } else {
      ffD = constB * vel_slip_mag / (std::abs(vel_slip_mag)+_small);
   }
      
}

// Location to implement UDF if Lift or Wall Lubrications models are included 
// see more details in header file (template_class_name.h).
void template_class_name::operator()(double &ff, double &ffDx, double &ffDy, double &ffDz, double vel_slip_mag, double u_slip, double v_slip, double w_slip, double Re, double Eo) {
   
   // Define some constants needed in the expression for tomiyama slightly contaminated
   double Re_const = Re / (vel_slip_mag + _small);
   double constA = pow(Re_const, 0.687);
   double constB = Re_const * Eo / (9 * (Eo + 4));
   double fun0   = 1 + 0.15 * pow(Re, 0.687);
   double v_crit = pow((2/(0.15*constA)),(1/0.687));
   double ff1_blend = 0.5 + 0.5 * std::tanh(5*(vel_slip_mag-v_crit)/v_crit);
   double ff1 = ff1_blend*3 + (1-ff1_blend)*fun0;
   double ff2 = constB * vel_slip_mag;
   double fun1 = 0, fun2 = 0, term = 0;
 
   // Calculate ff based on constants
   ff  = std::max(ff1,ff2);

   // Calculate derivative of friction factor (ffD), for Newton-Raphson method
   // (in each direction)
   if (ff1 > ff2) {
      fun1 = 0.10305 * constA * pow(vel_slip_mag, -0.313);
      fun2 = 2.5*(1-v_crit)/v_crit / (pow(std::cosh(5*(vel_slip_mag-v_crit)/v_crit), 2) + _small);
      term  = (fun2*3 + fun1 - fun1*ff1_blend - fun0*fun2);
   } else {
      term = constB; 
   }
   ffDx = (u_slip / (vel_slip_mag + _small)) * term;
   ffDy = (v_slip / (vel_slip_mag + _small)) * term;
   ffDz = (w_slip / (vel_slip_mag + _small)) * term;
}
