/*
 * template_class_name.cxx
 *
 */

#include "template_class_name.h"
#include "cppinterface.h"
#include <cmath>

// Class Constructor
template_class_name::template_class_name() {
   _small   = get_double("small");
}

// Class Destructor
template_class_name::~template_class_name() {
}

// Location to implement UDF if Drag is only interphase model (no Lift, or Wall Lubrication)
// see more details in header file (template_class_name.h).
void template_class_name::operator()(double &ff, double &ffD, double vel_slip_mag, double Re, double Eo) {

   // Define some constants needed in the expression for tomiyama contaminated
   double Re_const = Re / (vel_slip_mag + _small);
   double constA = pow(Re_const, 0.687);
   double constB = Re_const * Eo / (9 * (Eo + 4));
   double ff1 = 1 + 0.15 * pow(Re, 0.687);
   double ff2 = constB * std::abs(vel_slip_mag);

   // Calculate ff based on constants
   ff  = std::max(ff1,ff2);

   // Calculate derivative of friction factor (ffD), for Newton-Raphson method
   if (ff1 > ff2) {
      ffD = vel_slip_mag * 0.10305 * constA * pow(std::abs(vel_slip_mag), -1.313);
   } else {
      ffD = constB / (std::abs(vel_slip_mag) + _small);
   }
      
}


// Location to implement UDF if Lift or Wall Lubrications models are included 
// see more details in header file (template_class_name.h).
void template_class_name::operator()(double &ff, double &ffDx, double &ffDy, double &ffDz, 
                                     double vel_slip_mag, double u_slip, double v_slip, double w_slip, double Re, double Eo) {

   // Define some constants needed in the expression for tomiyama contaminated
   double Re_const = Re / (vel_slip_mag + _small);
   double constA = pow(Re_const, 0.687);
   double constB = Re_const * Eo / (9 * (Eo + 4));
   double term = 0;
   double ff1 = 1 + 0.15 * pow(Re, 0.687);
   double ff2 = constB * vel_slip_mag;

   // Calculate ff based on constants
   ff  = std::max(ff1,ff2);

   // Calculate derivative of friction factor (ffD), for Newton-Raphson method
   // (in each direction)
   if (ff1 > ff2) {
      term = 0.10305 * constA * pow(vel_slip_mag, -1.313);
   } else {
      term = constB / (vel_slip_mag + _small);
   }
   ffDx = u_slip * term; // (already included 1/vel_slip_mag in term) 
   ffDy = v_slip * term; // (already included 1/vel_slip_mag in term)
   ffDz = w_slip * term; // (already included 1/vel_slip_mag in term)
}
