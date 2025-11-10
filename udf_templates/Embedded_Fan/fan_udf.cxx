// template_class_name.cxx //

#include "template_class_name.h"
#include "cppinterface.h"
#include "Fan.h"
#include <iostream>

// Constructor
template_class_name::template_class_name() {

   // Set up a new fan:
   //    - fan has the following dimensions: R = 0.3 m; L = 1.0 m
   //    - the fan centre is located at coordinates (X/Y/Z): (1.5/0.5/0.0)
   //    - the fan axis is slightly tilted in the Z-direction
   //    - Fan target volumetric flow rate  = 5 m^3/s
   myFan =  new Fan("myFan", 0.3, 1.0, {1.5, 0.5, 0.0}, {1.0, 0.0, 0.2}, 5.0);

   if (get_integer("glob_rank") == 0) {
      // Root processor outputs some information
      // about the fan object 'myFan'
      // at the beginning of the simulation
      std::cout << std::endl << "------------------" << std::endl;
      std::cout << std::endl << "-- Fan Info     --" << std::endl;
      std::cout << std::endl << "------------------" << std::endl;
      std::cout << "Name       : " << myFan->getName() << std::endl;
      std::cout << "Region Name: " << myFan->getFanRegion()->getName() << std::endl;
      std::cout << "Target volumetric flow rate : "  << myFan->getVolumetricFlowRate() << std::endl;
      std::cout << "Target bulk velocity magn.  : "  << myFan->getVelBulkTargetMagnitude() << std::endl;
      std::cout << std::endl;
   }
}

// Destructor
template_class_name::~template_class_name() {
   // Delete 'Fan' object 'myFan',
   // allocated in the constructor,
   // after the simulation is finished
   delete myFan;
}

// Operator for function, calculated during the solver
void template_class_name::operator ()() {
   // Apply fan
   myFan->takeAction();
}
