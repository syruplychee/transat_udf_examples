// template_class_name.cxx //

#include "template_class_name.h"

template_class_name::template_class_name() {
}

template_class_name::~template_class_name() {
}

void template_class_name::operator ()() {
    int timestep = get_integer("timestep");

    //***********      Set Part           **********//
    // set inflow uvelocity and temperature
    if (timestep == 1) {
        bcmag.set_bc_value("Udinflow", "uvel", 0.33);
        bcmag.set_bc_value("Udinflow", "temp", 300);
        bcmag.set_bc_value("Udoutflow", "pressure", 1023);
    } 
    if (timestep == 10) {
        bcmag.set_bc_value("Udinflow", "uvel", 1);
        bcmag.set_bc_value("Udinflow", "temp", 600);
        // pressure not change 
    }

    //***********      Get Part           **********//
    double avgtemp = bcmag.get_bc_avg_value("Udoutflow", "temp");     // get outflow averaged temperature
    std::cout << "Outflow averaged temperature is " << avgtemp << "K" << std::endl; 
}
