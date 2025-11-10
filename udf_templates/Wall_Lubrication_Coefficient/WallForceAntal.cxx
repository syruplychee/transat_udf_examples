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

// Location to implement the UDF for wall lubrication model. The template
// shows wall coefficient calculation based on Antal model.
// see template_class_name.h for further description
void template_class_name::operator ()(double &Cw, double yw, double D, double Eo) {
   Cw = std::max(0.0, -0.005 + 0.025*D / (yw + _small));
}
