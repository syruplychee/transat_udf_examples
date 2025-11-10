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
void template_class_name::operator ()(int iphase) {
   
   // Paramter number of blocks (nblocks)
   // Parameter number of cells (nijk)
   int nijk, nblocks;

   // rhs for source term
   double *rhs;

   // Get parameters
   nblocks  = get_integer("nblocks" );
   // Loop over blocks
   for (int nb=0; nb < nblocks; nb++){
      // Total number of cells in block
      nijk = get_integer(nb,"nijk");
      set_pointer(nb, &rhs, "rhs");

      // Loop over cells
      for (int ii=0; ii < nijk; ii++) {

         // set AlphaRho-source term for first phase
         if (iphase == 1) {
         
            rhs[ii] += 23;
         }
         // set AlphaRho-source term for second phase
         if (iphase == 2) {
         
            rhs[ii] += 23;
         }
      }
   }
}
