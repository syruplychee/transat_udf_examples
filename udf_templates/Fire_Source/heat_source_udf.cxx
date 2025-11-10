// template_class_name.cxx //

#include "template_class_name.h"
#include "cppinterface.h"
#include "Region_Base.h"
#include "FireSource.h"
#include <iostream>
#include <sstream>

// Constructor
template_class_name::template_class_name() {
   // Using the 'FireSource' class to setup
   // a heat source (i.e.  fire without smoke):
   //    - heat source is inside a box of dimensions [0.0, 1.0] x [0.0, 1.0] x [0.0, 1.0]
   //    - the source is elevated by an angle theta = pi/4 = 0.785 (xz-plane)
   //    - Heat added  = 100 W/m^3
   //
   //    NOTE: The 'FireSource' class can only be used as a source, not a sink
   myHeatSource = new FireSource("myHeatSource", 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.785, 0.0, 100);

   if (get_integer("glob_rank") == 0) {
      // Root processor outputs some information
      // about the fire source object 'myHeatSource'
      // at the beginning of the simulation
      std::cout << std::endl << "------------------" << std::endl;
      std::cout << std::endl << "-- Heat Source  --" << std::endl;
      std::cout << std::endl << "------------------" << std::endl;
      std::cout << "Name       : " << myHeatSource->getName() << std::endl;
      std::cout << "Region Name: " << myHeatSource->getFireRegion()->getName() << std::endl;
      std::cout << "Volume     : " << myHeatSource->getFireRegion()->getVolume() << std::endl;
      std::cout << "Vol. heat power  : "  << myHeatSource->getVolumetricHeatPower() << std::endl;
      std::cout << std::endl;
   }
}

// Destructor
template_class_name::~template_class_name() {
   // Delete 'FireSource' object 'myHeatSource',
   // allocated in the constructor,
   // after the simulation is finished
   delete myHeatSource;
}

// Operator for function, calculated during the solver
void template_class_name::operator ()() {
   // Apply heat source
   myHeatSource->takeAction();
}
