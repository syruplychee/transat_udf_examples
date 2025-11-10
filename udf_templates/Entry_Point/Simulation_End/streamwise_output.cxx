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
   // Block parameters
   int ni, nj, nij;
   // Local variables
   int ii, iiWallCell, iBoundaryCell, jBoundaryCell, kBoundaryCell, ncellsblock;
   double Tnum, Tden, Tb, Twall, x, lambda, heatflux, Nu;
   // Variables
   double *u, *T, *cellvolume;
   int  *kblk;
   // Parameters
   int nblocks = get_integer("nblocks");
   char direction_top[] = "ymax";
   double R = 0.1;

   // Pointers to vectors storing data on each processor
   std::vector <double>* x_local       = new std::vector <double> ();
   std::vector <double>* Nu_local      = new std::vector <double> ();
   std::vector <double>* Tbulk_local   = new std::vector <double> ();

   // Pointers to vectors storing gathered data
   std::vector <double>* x_global;
   std::vector <double>* Nu_global;
   std::vector <double>* Tbulk_global;
   
   int glob_rank;
   MPI_Comm_rank(MPI_COMM_WORLD, &glob_rank);
   
   // Loop over blocks
   for (int iblock=0; iblock<nblocks; iblock++){
      // Get number of top wall cells in the block
      ncellsblock = getWallBoundaryNFaces(iblock, direction_top);
      // Access block data of variables
      set_pointer(iblock,&u,"uvelocity");
      set_pointer(iblock,&T,"temperature");
      set_pointer(iblock,&cellvolume,"cellvolume");
      set_pointer(iblock,&kblk,"kblk");

      ni  = get_integer(iblock,"ni");
      nj  = get_integer(iblock,"nj");
      nij = get_integer(iblock,"nij");

      // Looping cells in the streamwise direction
      for (int icell = 0; icell < ncellsblock; icell++){
          // Get wall cell 1D-index in block
          iiWallCell = getWallBoundaryCellIndex(iblock, icell, direction_top);
          // Get 3D-indexing of wall cell
          convert1DindexTo3D(iblock,iiWallCell,iBoundaryCell,jBoundaryCell,kBoundaryCell);
          // Get x-coordinate
          x = getWallBoundaryValue(iblock, icell, "x", direction_top);
          x_local->push_back(x);
          // Get wall temperature
          Twall = getWallBoundaryValue(iblock, icell, "temperature", direction_top);
          // Compute bulk temperature
          // Initialise numerator and denominator for bulk temperature computation
          Tnum = 0;
          Tden = 0;   
          // Integrate along the crossflow direction over the internal cells to get the bulk temperature
          /*************************************************************************************************
          Note: the integration used in this example is only correct  when cross-section is contained in a
                single block. Tnum and Tden must be summed over the blocks containing the cross-section when
                it spans over multiple blocks.
          *************************************************************************************************/
          for (int j = 2; j < nj-3; j++){
             ii     = iBoundaryCell + j*ni + kBoundaryCell*nij;
             Tnum  += kblk[ii]*T[ii]*u[ii]*cellvolume[ii];
             Tden  += kblk[ii]*u[ii]*cellvolume[ii];
          }
          Tb = Tnum/Tden;
          Tbulk_local->push_back(Tb);
          // Get conductivity using TransAT's properties module
          properties_initialize(iblock,ii);
          lambda = properties_get("lambda");
          // Compute Nusselt number
          heatflux = getWallBoundaryValue(iblock, icell, "q", direction_top);
          Nu = 2*R*heatflux/(lambda*(Twall-Tb));
          Nu_local->push_back(Nu);
       }
   }

   // Gather data from all processors on the root processor
   x_global      = tmb::toolbox::parallel_collectVector(x_local);
   Nu_global     = tmb::toolbox::parallel_collectVector(Nu_local);
   Tbulk_global  = tmb::toolbox::parallel_collectVector(Tbulk_local);

   if(glob_rank==0){
      // Sort data by x-coordinate
      std::vector<size_t> * indexSorted;
      indexSorted = tmb::toolbox::indexMapFromSort(x_global);
      std::vector<double>* x_globalSorted;
      std::vector<double>* Nu_globalSorted;
      std::vector<double>* Tbulk_globalSorted;

      x_globalSorted      = tmb::toolbox::getVectorSortedByIndexMap(indexSorted, x_global);
      Nu_globalSorted     = tmb::toolbox::getVectorSortedByIndexMap(indexSorted, Nu_global);
      Tbulk_globalSorted  = tmb::toolbox::getVectorSortedByIndexMap(indexSorted, Tbulk_global);

      // Group data
      std::vector<std::vector<double> > variables;
      variables.push_back(*x_globalSorted);
      variables.push_back(*Tbulk_globalSorted);
      variables.push_back(*Nu_globalSorted);

      // Set column names
      std::vector<std::string> varnames;
      varnames.push_back("x-coordinate");
      varnames.push_back("bulk_temperature");
      varnames.push_back("Nu");

      // Write data to file
      tmb::toolbox::writeDataVectorsToFile(std::string("template_class_name.dat"), varnames, variables);

      // Delete data-sorting pointers
      delete indexSorted, x_globalSorted, Nu_globalSorted, Tbulk_globalSorted;
   }

   // Delete data-storing pointers
   delete x_local, Nu_local, Tbulk_local, x_global, Nu_global, Tbulk_global;
}
