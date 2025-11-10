// template_class_name.cxx //

#include "template_class_name.h"
#include "cppinterface.decl.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <fstream>
#include <toolbox.h>
#include "mpi.h"
#include "Particle.h"

template_class_name::template_class_name() {
}

template_class_name::~template_class_name() {
}

void template_class_name::operator ()() {
   unsigned timestep  = static_cast<unsigned>(get_integer("timestep"));
   unsigned substep   = static_cast<unsigned>(get_integer("particle_substep"));
   double   ctime     =                       get_double ("time");
   double   dtime     =                       get_double ("dtime");
   double   dtsubstep =                       get_double ("particle_dtsubstep");
   unsigned glob_rank = static_cast<unsigned>(get_integer("glob_rank"));
   unsigned glob_size = static_cast<unsigned>(get_integer("glob_size"));
   unsigned nParticles = tmb::Particle::getNumParticlesLocal();

   double time;

   if (ctime == 0.0) // beginning of the simulation
      time = 0.0;
   else 
      time = (ctime-dtime) + substep*dtsubstep;
   
   for (unsigned grank=0; grank<glob_size; grank++){
      MPI_Barrier(MPI_COMM_WORLD);
      if ((grank == glob_rank)&&(nParticles > 0)){
         // open file (append mode)
         std::ofstream myfile;
         myfile.open("particleposition.dat", std::ofstream::out | std::ofstream::app);

         for (unsigned iParticle=0; iParticle < nParticles; iParticle++){
            tmb::Particle p(iParticle);
            myfile << std::fixed;
            myfile << std::setprecision(5) << time << " ";
            myfile << std::setw(2)         << timestep << " ";
            myfile << std::setw(2)         << substep  << " ";
            myfile << std::fixed;
            myfile << std::setprecision(5) << iParticle << " "; 
            myfile << std::setprecision(5) << p.getGlobalBlockIndex() << " ";
            myfile << std::setprecision(5) << p.getPosition().transpose() << " ";
            myfile << std::endl;
         }

         // close file
         myfile.close();
      }
   }

}
