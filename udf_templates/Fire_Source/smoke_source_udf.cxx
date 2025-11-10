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
   // a concentration source:
   //    - heat source is inside a box of dimensions [0.0, 1.0] x [0.0, 1.0] x [0.0, 1.0]
   //    - the source is elevated by an angle theta = pi/4 = 0.785 (xz-plane)
   //    - Heat added  = 0
   //    - Smoke (concentration) = 10 [C]/s (assuming [C] is a volumetric density, i.e. [C] = .../m^3)
   //
   //    NOTE: The 'FireSource' class can only be used as a source, not a sink
   myConcentrationSource = new FireSource("myConcentrationSource", 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.785, 0.0, 0.0, 10.0);

   if (get_integer("glob_rank") == 0) {
      // Root processor outputs some information
      // about the fire source object 'myConcentrationSource'
      // at the beginning of the simulation
      std::cout << std::endl << "---------------------------" << std::endl;
      std::cout << std::endl << "-- Concentration Source  --" << std::endl;
      std::cout << std::endl << "---------------------------" << std::endl;
      std::cout << "Name       : " << myConcentrationSource->getName() << std::endl;
      std::cout << "Region Name: " << myConcentrationSource->getFireRegion()->getName() << std::endl;
      std::cout << "Volume     : " << myConcentrationSource->getFireRegion()->getVolume() << std::endl;
      std::cout << "Vol. conc. source  : "  << myConcentrationSource->getVolumetricSmokeSource() << std::endl;
      std::cout << std::endl;
   }
}

// Destructor
template_class_name::~template_class_name() {
   // Delete 'FireSource' object 'myConcentrationSource',
   // allocated in the constructor,
   // after the simulation is finished
   delete myConcentrationSource;
}

// Operator for function, calculated during the solver
void template_class_name::operator ()() {
   // Apply concentration source
   myConcentrationSource->takeAction();
}
