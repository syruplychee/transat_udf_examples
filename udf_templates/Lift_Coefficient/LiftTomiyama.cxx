/*
 * template_class_name.cxx
 *
 */

#include "template_class_name.h"
#include "cppinterface.h"
#include <cmath>

// Constructor
template_class_name::template_class_name() {
   _small = get_double("small");
}

// Destructor
template_class_name::~template_class_name() {
}

// Location to implement UDF 
// Cl can be redefined here, keeping note that the derivative of Cl in each 
// coordinate direction must also be set. 
// check template_class_name.h for more description of variables
void template_class_name::operator()(double &Cl, double &ClDx, double &ClDy, double &ClDz, 
                                     double vel_slip_mag, double u_slip, double v_slip, double w_slip, double Re, double Eo) {

   // Some constants required in the tomiyama lift model
   double Eo4fun1=0, Eo4fun2=0;

   // Initialize values 
   Cl  = -0.27; 
   ClDx = 0; ClDy = 0; ClDz = 0;

   // Tomiyama model calculates Cl based on Eo 
   if (Eo < 4) {
      Eo4fun1 = 0.288 * std::tanh(0.121*Re); // !Note, introducing Re, dependent on velocity
      Eo4fun2 = 0.00105 * Eo*Eo*Eo - 
                0.0159  * Eo*Eo    -
                0.0204  * Eo       +
                0.474;
      Cl = std::min(Eo4fun1, Eo4fun2);
      Cl = std::max(0.001, Cl);  // bootstrapping for case Re = 0

      if (Eo4fun1 < Eo4fun2) {
         // If function Eo4fun1 is smaller, and is used for Cl calculation
         // then also need derivative wrt velocity
         double denom = vel_slip_mag * pow( std::cosh(0.121*Re), 2) + _small;
         ClDx = 0.034848*(Re/(vel_slip_mag + _small))*u_slip / denom;
         ClDy = 0.034848*(Re/(vel_slip_mag + _small))*v_slip / denom;
         ClDz = 0.034848*(Re/(vel_slip_mag + _small))*w_slip / denom;
      }
   } else if (Eo < 10){

      // Cl is independent of velocity,
      // so the derivative is zero
      Cl      = 0.00105 * Eo*Eo*Eo - 
                0.0159  * Eo*Eo    -
                0.0204  * Eo       +
                0.474;
   }
   // else (Eo > 10) Cl = 0.27, ClD = 0
}
