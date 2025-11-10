// template_class_name.cxx //

#include "template_class_name.h"
#include "cppinterface.decl.h"
#include <cmath>

// Constructor 
template_class_name::template_class_name() {

   // Definition of line in Y direction at x=1.99 and z=0.0005,
   // interpolation Bilinear. 
   output = new line_output(LO_YDIR,1.99,0.0005,LO_BILINEAR);

   // Definition of variables to plot
   output->add_variable("uvelocity");
   output->add_variable("vvelocity");
   output->add_variable("temperature");
}

// Destructor
template_class_name::~template_class_name() {

   // Delete pointer
   delete output;
}

void template_class_name::operator ()() {

   // Write profiles to file
   output->write_file("template_class_name.dat"); 
}
