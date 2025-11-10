// template_class_name.cxx //

#include<stdio.h>
#include<math.h>
#include"cppinterface.h"
#include<string>
#include "mpi.h"
#include "template_class_name.h"

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
   double max_temperature;        // maximum temperature (local)
   double global_max_temperature; // maximum temperature (global)

   // MPI tools
   int glob_rank, numprocs;// Get the individual process ID.
   glob_rank = MPI::COMM_WORLD.Get_rank(); //get the number of blocks handled by the current processor.
   nblocks = get_integer("nblocks");
   
   // Find the maximum temperature in the blocks
   // handled by the current processor
   max_temperature = 0.0;

   // Loop on the blocks handled by the current processor
   for (nb=1; nb<= nblocks; nb++){

      // get the number of cells on the current block
      nijk = get_integer(nb,"nijk");

      // link to the temperature array of TransAT.
      // This has to be done inside the block loop.
      set_pointer(nb,&T,"temperature");

      // Loop on the cells of the block
      for (ii=0; ii < nijk; ii++) {
         max_temperature = fmax(max_temperature,T[ii]);
      }
   }

   // MPI communication to obtain the global maximum
   MPI::COMM_WORLD.Allreduce(&max_temperature,&global_max_temperature,1,MPI::DOUBLE,MPI::MAX);
      
   // Root processor writes the result
   if (glob_rank == 0) {
      printf("Maximum temperature is %f\n", global_max_temperature);
   }
   
}
