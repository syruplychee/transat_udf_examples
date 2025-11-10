/*
 * MasstransferCoeffFunctors.cxx
 *
 *  Created on: Sep 24, 2013
 *      Author: vonrickenbach
 */

#include "MasstransferCoeff_Functors.h"
#include "MasstransferCoeff_Factory_impl.h"
#include "cppinterface.decl.h"
#include <iostream>
#include <math.h>
#include <sstream>
#include <fstream>

/**
 *  \brief Dummy implementation just returns a constant
 *  value for the permeability
 */
MasstransferCoeff_constant::MasstransferCoeff_constant() {
   permeability = 1.0;

}

MasstransferCoeff_constant::~MasstransferCoeff_constant() {
}

double MasstransferCoeff_constant::operator ()(int nb, int ii) {
   return permeability;
}

/**
 *  \brief Nondimensional pressure drop
    \delta p d**2/\mu*u_superficial
     Reads the lengthscale and the coeffiencts for a linear fit
 */
MasstransferCoeff_linear_fit::MasstransferCoeff_linear_fit() {

   // Define the map of enumerations to string used in
   // cpp interface
   std::cout <<  "Create fit functor for mt_coefficient" << std::endl;
   _nb_pointers_name[en_uvelocity]= "uvelocity";
   _nb_pointers_name[en_vvelocity] = "vvelocity";
   _nb_pointers_name[en_wvelocity] = "wvelocity";
   _nb_pointers_name[en_hembed]    = "hembed";
   _nb_pointers_name[en_density]   = "rhofluid";
   _nb_pointers_name[en_viscosity] = "isvis";
   // Create and set the nb pointers
   set_nb_pointers();
   // Read coefficients from file
   read_coefficients();
}

MasstransferCoeff_linear_fit::~MasstransferCoeff_linear_fit() {
}

/**
 * Uses the local variables to calculate the Re number.
 * Gets the corresponding non-dimensional sherwood number.
 * The mt_coefficient will be multiplied later with the diffusion
 * coefficient
 * Makes the pressure drop dimensional and returns it.
 * @return
 */
double  MasstransferCoeff_linear_fit::operator()(int nb,int ii) {
   double loc_vel, loc_vel_s;
   double loc_hembed, loc_rho, loc_visc;
   double Re, delta_p_nd, delta_p;
   double mt_coefficient;

   // Masstransfer coefficient is coeff1 + Re^(coeff1)
   // Compute local values
   loc_vel = sqrt(pow(getValue(en_uvelocity,nb,ii),2)+
                  pow(getValue(en_vvelocity,nb,ii),2)+
                  pow(getValue(en_wvelocity,nb,ii),2));

   loc_hembed = getValue(en_hembed,nb,ii);
   loc_vel_s  = loc_vel * loc_hembed;
   loc_rho    = getValue(en_density,nb,ii);
   loc_visc   = getValue(en_viscosity,nb,ii);
   Re         = loc_vel_s * _lengthscale * loc_rho / (loc_visc + _small);

   mt_coefficient = (_coefficients.at(0) * pow(Re,_coefficients.at(1)))
                    / _lengthscale;

   return mt_coefficient;
}

void MasstransferCoeff_linear_fit::set_nb_pointers() {
   _nblocks = get_integer("nblocks");
   _small   = get_double("small");

   nb_pointers_t nb_pointer_temp;
    nb_pointers_name_map::iterator it_name;

    // Loop over all the enumerations in the constructor
    // and create the corresponding nb_pointer object and
    // set it to the fortran pointer
   for (it_name = _nb_pointers_name.begin();
        it_name!= _nb_pointers_name.end();
        ++it_name){
      nb_pointer_temp = nb_pointers_t();
      nb_pointer_temp.resize(_nblocks);
      for(int nb=0; nb< _nblocks;++nb)
         set_pointer(nb,&(nb_pointer_temp.at(nb)),it_name->second.c_str());

      // Create the map of enumerations -> nb_pointer
      _nb_pointers[it_name->first] = nb_pointer_temp;
   }
}

void MasstransferCoeff_linear_fit::read_coefficients() {
   std::ifstream infile;
   std::string line;

   typedef std::map<std::string,double*> input_map_t;
   input_map_t input_map;
   // Maps the name in the input file to the member
   // variable
   _coefficients.resize(2);
   input_map["coeff1"]      = &_coefficients.at(0);
   input_map["coeff2"]      = &_coefficients.at(1);
   input_map["lengthscale"] = &_lengthscale;

   infile.open("mt_coefficients.dat");

   // Reads the file and assings the correct
   // members
   int nlines = 0;
    // Loop over the file
   while (std::getline(infile, line))
   {
      nlines++;
       std::istringstream iss(line);
       double coeff;
       std::string name;
       // Read the line into the string stream
       if (!(iss >> name >> coeff)) { break; } // error

       // Find the correct member and assign the value
       input_map_t::iterator it = input_map.find(name);
       if(it != input_map.end())
          *(it->second)= coeff;
       else
          std::cout << "ERROR: Key not found" << std::endl;

   }
   if (!nlines)
      std::cout << "File is empty or does not exist"<< std::endl;
   infile.close();
}

double MasstransferCoeff_linear_fit::getValue(nb_arrays_enum var, int nb,int ii) {
   return _nb_pointers[var].at(nb)[ii];
}
