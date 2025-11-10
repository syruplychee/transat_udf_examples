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
// shows wall coefficient calculation based on Frank model.
// see template_class_name.h for further description
void template_class_name::operator ()(double &Cw, double yw, double D, double Eo) {

   // Cw definition (from Tomiyama)
   if (Eo < 1.0) {
      Cw = 0.470481;
   } else if (Eo < 5.0) {
      Cw = std::exp(-0.933*Eo + 0.179);
   } else if (Eo < 33.0) {
      Cw = 0.00927623*Eo - 0.0351155;
   } else {
      Cw = 0.271;
   }

   // Factor, based on Frank model
   double FR_factor = 0;
   if (yw < 100) {
      FR_factor = std::max(0.0, (1/6.8) * (1 - yw / (10*D)) / (yw * pow(yw/(10*D), 0.7) + _small));
   }
   Cw = Cw * 0.5 * D * FR_factor;

}
