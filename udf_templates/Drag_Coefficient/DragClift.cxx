/*
 * DragTermClift.cxx
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
void template_class_name::operator ()(double &ff, double &ffD, double vel_slip_mag, double Re, double Eo) {

   // Define some constants needed in the expression for clift
   double Re_const = Re / (vel_slip_mag + _small);
   double a = 0.15;
   double b = 0.0175;
   double c = 0.687;
   double d = 42500;
   double denom = 1 + d / pow(Re, 1.16);

   // Calculate ff based on constants
   ff  = 1 + a * pow(Re,c) + b * Re / denom; 

   // Calculate derivative of friction factor (ffD), for Newton-Raphson method
   double term1 = a*c*pow(Re_const, c)*pow(vel_slip_mag, c-1);
   double term2 = b*((Re_const/denom) + 1.16*d*pow(Re_const, -2.16)*pow(vel_slip_mag, -1.16) / (denom * denom));
   ffD = term1 + term2;
}

// Location to implement UDF if Lift or Wall Lubrications models are included 
// see more details in header file (template_class_name.h).
void template_class_name::operator ()(double &ff, double &ffDx, double &ffDy, double &ffDz,
                                 double vel_slip_mag, double u_slip, double v_slip, double w_slip, double Re, double Eo) {

   // Define some constants needed in the expression for clift
   double Re_const = Re / (vel_slip_mag + _small);
   double a = 0.15;
   double b = 0.0175;
   double c = 0.687;
   double d = 42500;
   double denom = d + pow(Re, 1.16);

   // Calculate ff based on constants
   ff  = 1 + a * pow(Re,c) + b * Re / (1+d/pow(Re,1.16)); 
   
   // Calculate derivative of friction factor (ffD), for Newton-Raphson method
   // (in each direction)
   double term1 = a*c*pow(Re_const, c)*pow(vel_slip_mag, c-1);
   double term2 = b*Re_const*(denom+1.16*d)*(denom-d)/(denom*denom);

   ffDx = (u_slip/(vel_slip_mag+_small))*(term1 + term2);
   ffDy = (v_slip/(vel_slip_mag+_small))*(term1 + term2);
   ffDz = (w_slip/(vel_slip_mag+_small))*(term1 + term2);
}
