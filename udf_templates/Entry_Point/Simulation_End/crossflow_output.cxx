// template_class_name.cxx //

#include "template_class_name.h"
#include "cppinterface.decl.h"
#include <cmath>

template_class_name::template_class_name() {
  output = new line_output(LO_YDIR,1.99,0.0005,LO_BILINEAR);
  output->add_variable("uvelocity");
  output->add_variable("vvelocity");
  output->add_variable("temperature");
}

template_class_name::~template_class_name() {
  delete output;
}

void template_class_name::operator ()() {
   // Write profiles in crossflow direction in template_class_name.dat
   output->write_file("template_class_name.dat"); 
}
