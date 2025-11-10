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

// Location to implement the UDF for wall lubrication model. The template
// shows wall coefficient calculation based on Hosokawa model.
// see template_class_name.h for further description
void template_class_name::operator()(double &Cw, double yw, double D, double Eo) {
   Cw = 0.0217 * Eo * D * D / (4 * yw * yw + _small);
}
