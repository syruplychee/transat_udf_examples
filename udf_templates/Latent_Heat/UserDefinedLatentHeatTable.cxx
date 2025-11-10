/*
 * template_class_name.cxx
 *
 */

#include "template_class_name.h"
#include "cppinterface.h"
#include <cmath>
#include "utility_functions.decl.h"
#include "cppinterface.decl.h"
#include <cmath>
#include <algorithm>


// Class Constructor
template_class_name::template_class_name() {
   std::cout << "constructed template_class_name" << std::endl;

   /**
    * We want to avoid the situtation of a number divided by zero, that's why
    * we always divide by a variable after adding the parameter small whose
    * value
    * is specified in shared_modules/constants.f90
    */
   _small = tmb::Constants::Small();
   _smdiff = tmb::Constants::Smdiff();

   /**
    * We don't want to access system pressure for every point in the domain
    * because it a constant for the whole domain
    * However, it can change with time.
    */
   set_pointer(&(_system_pressure),"system_pressure");
//   Pres_input = {
//0
//,101325
//,121325
//,141325
//,161325
//,181325
//,201325
//,221325
//,241325
//,261325
//,281325
//,301325
//,321325
//,341325
//,361325
//,381325
//,401325
//,421325
//,441325
//,461325
//,481325
//,501325
//,521325
//,541325
//,561325
//,581325
//,601325
//,621325
//,641325
//,661325
//,681325
//,701325
//,721325
//,741325
//,761325
//,781325
//,801325
//,821325
//,841325
//,861325
//,881325
//,901325
//,921325
//,941325
//,961325
//,981325
//,1001325
//,1021325
//,1041325
//,1061325
//,1081325
//,1101325};

//   output = {
//      // Enter in latent heat, one value for each pressure
//};



   
}

// Class Destructor
template_class_name::~template_class_name() {
   std::cout << "destructed template_class_name" << std::endl;
}

double template_class_name::latent_heat(double P) {
   
   P += *(_system_pressure);
   return interpolate(P);
}

double template_class_name::interpolate(double P) {
   int pres_index = 0;
   for (int i = 0; i < Pres_input.size(); i++) {
      if (P >= Pres_input[i] && P <= Pres_input[i+1]) {
         pres_index = i;
         break;
      }
   }

   double value = 0;
   double lower_input = Pres_input[pres_index];
   double upper_input = Pres_input[pres_index+1];

   double lower_value = output[pres_index];
   double upper_value = output[pres_index+1];

   value = (upper_value - lower_value) / (upper_input - lower_input) * (P - lower_input) + lower_value;

   return value;


}
