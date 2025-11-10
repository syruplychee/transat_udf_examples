// template_class_name.cxx //

#include "template_class_name.h"
#include"cppinterface.h"

template_class_name::template_class_name() {
}

template_class_name::~template_class_name() {
}

void template_class_name::operator ()() {

   int ii;          //cell index
   int nb;          //block index
   int nijk;        //number of cells in one block
   int nblocks;     //number of blocks handled by one processor
   double *T;       // temperature array

   nblocks = get_integer("nblocks");


   // Loop on the blocks handled by the current processor
   for (nb=1; nb<= nblocks; nb++){

      // get the number of cells on the current block
      nijk = get_integer(nb,"nijk");

      // link to the temperature array of TransAT.
      // This has to be done inside the block loop.
      set_pointer(nb,&T,"temperature");

      // Loop on the cells of the block
      for (ii=0; ii < nijk; ii++) {
         T[ii] = 293;
      }
   }
}
