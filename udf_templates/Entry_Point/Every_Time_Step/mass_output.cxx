// template_class_name.cxx //

#include "template_class_name.h"
#include "cppinterface.decl.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream>
#include <toolbox.h>
#include "mpi.h"

template_class_name::template_class_name() {
}

template_class_name::~template_class_name() {
}

void template_class_name::operator ()() {
   update_time_mass();
   write_time_mass();
}

void template_class_name::update_time_mass() {

   int nijk, nblocks;
   int  *kblk;
   double *rho, *vol, *Hembed;
   double t, rho_water, alpha_water, local_water_mass(0.), global_water_mass(0.);
   
   // Get parameters
   nblocks  = get_integer("nblocks" );
   t        = get_double ("time"    );
   // Loop over blocks
   for (int nb=0; nb < nblocks; nb++){
      // Total number of cells in block
      nijk = get_integer(nb,"nijk");
      // Access to TransAT data arrays
      set_pointer(nb, &kblk,      "kblk"       );
      set_pointer(nb, &vol,       "cellvolume" );
      set_pointer(nb, &Hembed,    "hembed"     );

      // Loop over cells
      for (int ii=0; ii < nijk; ii++) {
         properties_initialize(nb, ii);
         // Get water density
         rho_water = properties_get("density",0);
         // Get water volume fraction
         alpha_water = properties_get("alpha",0);
         // Increment local mass
         // kblk[ii]*Hembed[ii]*vol[ii]: fluid cell volume (equal to 0 outside computational domain)
         local_water_mass += rho_water*alpha_water*kblk[ii]*Hembed[ii]*vol[ii];
      }
   }
   // Sum partial masses from all processors
   MPI_Allreduce(&local_water_mass, &global_water_mass, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
   // Append data to vectors storing time and water mass
   Time.push_back(t);
   Mass.push_back(global_water_mass);


}

void template_class_name::write_time_mass() {
   int glob_rank;
   std::vector<std::string> varnames;
   // Get processor rank
   MPI_Comm_rank(MPI_COMM_WORLD, &glob_rank);
   // Write using root processor
   if(glob_rank == 0){
      // Set variable names for header
      varnames.push_back("Time s");
      varnames.push_back("Mass kg");
      // Group data
      std::vector<std::vector<double> > variables;
      variables.push_back(Time);
      variables.push_back(Mass);
      // Write data to file
      tmb::toolbox::writeDataVectorsToFile(std::string("test_output.dat"), varnames, variables);  
   }


}
