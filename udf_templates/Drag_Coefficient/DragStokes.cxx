/*
 * template_class_name.cxx
 *
 */

#include "template_class_name.h"
#include "cppinterface.h"

// Class Constructor
template_class_name::template_class_name() {
}

// Class Destructor
template_class_name::~template_class_name() {
}

// Location to implement UDF if Drag is only interphase model (no Lift, or Wall Lubrication)
// see more details in header file (template_class_name.h).
void template_class_name::operator ()(double &ff, double &ffD, double vel_slip_mag, double Re, double Eo) {

   // friction factor for stokes is 1,
   // derivative is then 0
   ff  = 1;
   ffD = 0;
}

// Location to implement UDF if Lift or Wall Lubrications models are included 
// see more details in header file (template_class_name.h).
void template_class_name::operator ()(double &ff, double &ffDx, double &ffDy, double &ffDz,
                                 double vel_slip_mag, double u_slip, double v_slip, double w_slip, double Re, double Eo) {

   // friction factor for stokes is 1,
   // derivative is then 0
   ff   = 1;
   ffDx = 0;
   ffDy = 0;
   ffDz = 0;
}
