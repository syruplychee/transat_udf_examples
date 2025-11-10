// template_class_name.cxx // 

#include "template_class_name.h"
#include "cppinterface.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream>

// Constructor
template_class_name::template_class_name() {
}

// Destructor
template_class_name::~template_class_name() {
}

// Operator for function, calculated during the solver 
void template_class_name::operator ()() {
   
   // Paramter number of blocks (nblocks)
   // Parameter number of cells (nijk)
   int nijk, nblocks;

   // rhs for source term
   // z for coordinate at cell center
   double *rhs, *z;

   // arbitrary zmax coordinate
   double zmax = 0.1;
   
   // Get parameters
   nblocks  = get_integer("nblocks" );
   // Loop over blocks
   for (int nb=0; nb < nblocks; nb++){
      // Total number of cells in block
      nijk = get_integer(nb,"nijk");
      set_pointer(nb, &rhs, "rhs");
      set_pointer(nb, &z, "cellcenterz");

      // Loop over cells
      for (int ii=0; ii < nijk; ii++) {

         // set W-source term as 0.1 * (xcoord / xmax) 
         rhs[ii] += 0.1 * (z[ii] / zmax);
      }
   }
}
