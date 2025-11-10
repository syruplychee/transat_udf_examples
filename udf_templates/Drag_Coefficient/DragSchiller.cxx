/*
 * template_class_name.cxx
 *
 */

#include "template_class_name.h"
#include "cppinterface.decl.h"
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

   // Define some constants needed in the expression for schiller
   double Re_const = Re / (vel_slip_mag + _small);
   double constA   = pow(Re_const, 0.687);
   double damp     = 0.5 + 0.5 * std::tanh(Re /100 - 10);
   double fun0     = 1 + 0.15*pow(Re, 0.687);
   double fun1     = 0.0183*Re;
   double fun2     = Re_const/(200*pow((std::cosh(Re/100 - 10)),2)+_small);
   double fun3     = 0.10305 * constA * pow(std::abs(vel_slip_mag),-0.313);
   double fun4     = 0.0183*Re_const;

   // Calculate ff based on constants
   ff  = (1-damp)*fun0 + damp*fun1;

   // Calculate derivative of friction factor (ffD), for Newton-Raphson method
   ffD = (vel_slip_mag/(std::abs(vel_slip_mag)+_small)) * (fun2 * (fun1-fun0) + damp * (fun4-fun3) + fun3);
}

// Location to implement UDF if Lift or Wall Lubrications models are included 
// see more details in header file (template_class_name.h).
void template_class_name::operator()(double &ff, double &ffDx, double &ffDy, double &ffDz, 
                                     double vel_slip_mag, double u_slip, double v_slip, double w_slip, double Re, double Eo) {

   // Define some constants needed in the expression for schiller
   double Re_const = Re / (vel_slip_mag+_small);
   double constA   = pow(Re_const, 0.687);
   double damp     = 0.5 + 0.5 * std::tanh(Re /100 - 10);
   double fun0     = 1 + 0.15*pow(Re, 0.687);
   double fun1     = 0.0183*Re;
   double fun2     = Re_const/(200*pow(std::cosh(Re/100 - 10),2)+_small);
   double fun3     = 0.10305 * constA * pow(std::abs(vel_slip_mag),-0.313);
   double fun4     = 0.0183*Re_const;

   // Calculate ff based on constants
   ff   = (1-damp)*fun0 + damp*fun1;

   // Calculate derivative of friction factor (ffD), for Newton-Raphson method
   // (in each direction)
   ffDx = (u_slip/(std::abs(vel_slip_mag)+_small)) * (fun2 * (fun1-fun0) + damp * (fun4-fun3) + fun3);
   ffDy = (v_slip/(std::abs(vel_slip_mag)+_small)) * (fun2 * (fun1-fun0) + damp * (fun4-fun3) + fun3);
   ffDz = (w_slip/(std::abs(vel_slip_mag)+_small)) * (fun2 * (fun1-fun0) + damp * (fun4-fun3) + fun3);
}
