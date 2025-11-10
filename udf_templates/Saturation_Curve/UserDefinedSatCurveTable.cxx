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
//   Temp_input = {
//379.473
//,379.473
//,385.031
//,389.889
//,394.219
//,398.136
//,401.72
//,405.029
//,408.107
//,410.988
//,413.698
//,416.26
//,418.69
//,421.003
//,423.212
//,425.326
//,427.355
//,429.306
//,431.185
//,432.999
//,434.752
//,436.45
//,438.095
//,439.692
//,441.244
//,442.753
//,444.222
//,445.654
//,447.051
//,448.415
//,449.747
//,451.049
//,452.322
//,453.569
//,454.79
//,455.987
//,457.161
//,458.312
//,459.442
//,460.551
//,461.641
//,462.712
//,463.766
//,464.801
//,465.821
//,466.824
//,467.811
//,468.784
//,469.743
//,470.687
//,471.618
//,472.536
//
//                 };

// Fill in array corresponding to Psat for every Tsat above
//   Pres_input = {
//};

   
   std::cout << "finished constructed template_class_name" << std::endl;
}

// Class Destructor
template_class_name::~template_class_name() {
   std::cout << "destructed template_class_name" << std::endl;
}

double template_class_name::saturation_pressure(double T, unsigned iphase) {
   
   double Psat = interpolate_pressure(T);
   return Psat;
}

double template_class_name::saturation_temperature(double P, unsigned iphase) {
   
   P += *(_system_pressure);
   double Tsat = interpolate_temperature(P);
   return Tsat;
}

double template_class_name::interpolate_pressure(double T) {
   int temp_index = 0;
   for (int i = 0; i < Temp_input.size(); i++) {
      if (T >= Temp_input[i] && T <= Temp_input[i+1]) {
         temp_index = i;
         break;
      }
   }

   double value = 0;
   double lower_input = Temp_input[temp_index];
   double upper_input = Temp_input[temp_index+1];

   double lower_value = Pres_input[temp_index];
   double upper_value = Pres_input[temp_index+1];

   value = (upper_value - lower_value) / (upper_input - lower_input) * (T - lower_input) + lower_value;

   return value;


}

double template_class_name::interpolate_temperature(double P) {
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

   double lower_value = Temp_input[pres_index];
   double upper_value = Temp_input[pres_index+1];

   value = (upper_value - lower_value) / (upper_input - lower_input) * (P - lower_input) + lower_value;

   return value;


}
