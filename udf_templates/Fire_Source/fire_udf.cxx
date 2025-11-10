// template_class_name.cxx //

#include "template_class_name.h"
#include "cppinterface.h"
#include "Region_Base.h"
#include "FireSource.h"
#include <iostream>
#include <sstream>

// Constructor
template_class_name::template_class_name() {
   // Set up a new fire source:
   //    - fire is inside a box of dimensions [0.0, 1.0] x [0.0, 1.0] x [0.0, 1.0]
   //    - the source is elevated by an angle theta = pi/4 = 0.785 (xz-plane)
   //    - Heat from fire  = 1.0 MW/m^3
   //    - Smoke from fire = 0.1 [C]/s (e.g. [C] = g/m^3)
   myFire = new FireSource("myFire", 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.785, 0.0, 1000000, 0.1);

   if (get_integer("glob_rank") == 0) {
      // Root processor outputs some information
      // about the fire source object 'myFire'
      // at the beginning of the simulation
      std::cout << std::endl << "------------------" << std::endl;
      std::cout << std::endl << "-- Fire Source  --" << std::endl;
      std::cout << std::endl << "------------------" << std::endl;
      std::cout << "Name       : " << myFire->getName() << std::endl;
      std::cout << "Region Name: " << myFire->getFireRegion()->getName() << std::endl;
      std::cout << "Volume     : " << myFire->getFireRegion()->getVolume() << std::endl;
      std::cout << "Vol. heat power  : "  << myFire->getVolumetricHeatPower() << std::endl;
      std::cout << "Vol. smoke prod. : "  << myFire->getVolumetricSmokeSource() << std::endl;
      std::cout << std::endl;
   }
}

// Destructor
template_class_name::~template_class_name() {
   // Delete 'FireSource' object 'myFire',
   // allocated in the constructor,
   // after the simulation is finished
   delete myFire;
}

// Operator for function, calculated during the solver
void template_class_name::operator ()() {
   // Apply fire source
   myFire->takeAction();
}
