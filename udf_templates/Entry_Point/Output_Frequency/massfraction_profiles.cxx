// template_class_name.cxx //

#include "template_class_name.h"
#include "cppinterface.decl.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream>
#include <toolbox.h>
#include "mpi.h"

// Constructor
template_class_name::template_class_name() {
}

// Destructor
template_class_name::~template_class_name() {
}

// This UDF prints the lateral void fraction 
// across the y-coordinate at an x location of 
// 0.0384m. 
//
// This is written for Output_Frequency, and saves 
// the file name with the time-step, 
// however this UDF can be used at other types.
void template_class_name::operator ()() {

   // Cell indexes
   int i, j, k, ii; //(x, y, z, general)

   // Number of blocks
   // Number of cells in x (ni), xy-plane (nij), total (nijk)
   int nblocks, ni, nj, nij, nijk;
   
   // Number of Phases
   int nphases;

   // for file io
   FILE *testunit;

   // homogeneous model 
   bool ahm;

   // variables
   double *p,*u,*x,*y,*yc;
   double ycoord, x_store;
   double *rho;
   std::vector<double*> alpharho;
   double *alpha;
   int itimestep;
   
   ahm = get_logical("activate_homogeneous_model");
   nphases  = get_integer("nphases");

   alpharho.resize(nphases);

   // If homogeneous model not activated:
   // do nothing, alpharho arrays are not allocated.
   // This would lead to a crash.
   if (ahm) {

      nblocks = get_integer("nblocks");
      set_pointer(&rho,"rho");

      for (int nb=0; nb < nblocks; nb++){

         // Provide file name with suffix of time-step
         char numstr[21]; // enough to hold all numbers up to 64-bits
         itimestep = get_integer("timestep");
         sprintf(numstr, "%d", itimestep);
         std::string basename = "riser_test_output";
         std::string filename = basename + numstr + ".dat";
         testunit = fopen(filename.c_str(),"w");


       
         ni = get_integer(nb,"ni");
         nj = get_integer(nb,"nj");
         nij = get_integer(nb,"nij");
         nijk = get_integer(nb,"nijk");
       
         set_pointer(nb,&x, "cellcenterx");
         set_pointer(nb,&y, "cellcentery");
         set_pointer(nb,&yc,"cellcornery");

     
         // find cells at x=0.0384m
         j=3;
         k=3;

         i=3;
         ii = (i-1) + (j-1)*ni + (k-1)*nij;

         x_store = 0;
         while (x[ii] < 0.0384) {
          
            i++;  // increment in x direction
            ii = (i-1) + (j-1)*ni + (k-1)*nij; // find new ii
       
         }


         for (int iphase=0;iphase<nphases;iphase++)
            set_pointer(nb,&alpharho[iphase],  "alpharho",  iphase);
         
         

         alpha        = new double[nphases];


         // Plot void fraction profile at x=0.0384m (using i found from above)
         // iterating across y-direction (j) starting from beginning of block
         for (j = 2; j <= nj-2; j++){

            // Calculate new cell index
            ii = (i-1) + (j-1)*ni + (k-1)*nij;

            // find cell-center
            ycoord = y[ii];

            // If j is at block edge, use corner coordinate
            if (j ==    2) ycoord = yc[ii+ni];
            if (j == nj-2) ycoord = yc[ii];

            // Find void fraction from alpharho
            for(int iphase=0;iphase<nphases;iphase++) {
              alpha[iphase] = alpharho[iphase][ii] / rho[iphase];
            }

            // Print y-coord and void fraction to file
            fprintf(testunit,"%18.12e ",ycoord);
            fprintf(testunit,"%18.12e\n",alpha[0]); // gas volume fraction
           
           
         }
         // close file
         fclose(testunit);
      }
   }
}
