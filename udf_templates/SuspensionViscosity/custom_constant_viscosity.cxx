/*
 * SuspensionViscosityFunctors.cxx
 *
 *  Created on: Sep 24, 2013
 *      Author: vonrickenbach
 */

#include "template_class_name.h"
#include "cppinterface.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream>

// Initialize the class
template_class_name::template_class_name() {
}

// Destruct the class
template_class_name::~template_class_name() {
}

// Operator for function, var_out as variable to be written to suspensionviscosity output
void template_class_name::operator ()(double &visc_out) {

   // use properties module to get pressure
   // this UDF writes the absolute pressure
   double gauge_pressure = properties_get("pressure");
   visc_out = 0.0001;
}
