/*
 * template_class_name.cxx
 *
 */

#include "template_class_name.h"
#include "cppinterface.h"
#include <cmath>

// Constructor
template_class_name::template_class_name() {
}

// Destructor
template_class_name::~template_class_name() {
}

// Location to implement UDF 
// Auton has a constant value set for the parameter Cl, so nothing is required in the function
// Cl can be redefined here, keeping note that the derivative of Cl in each 
// coordinate direction must also be set. 
// LiftTomiyama.cxx demonstrates how to calculate ClDx, ClDy and Cldz
// check template_class_name.h for more description of variables
void template_class_name::operator ()(double &Cl, double &ClDx, double &ClDy, double &ClDz,
                                 double vel_slip_mag, double u_slip, double v_slip, double w_slip, double Re, double Eo) {
}
