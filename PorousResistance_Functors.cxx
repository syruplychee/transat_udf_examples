/*
 * PorousResistanceFunctors.cxx
 *
 *  Created on: Sep 24, 2013
 *      Author: vonrickenbach
 */

#include "PorousResistance_Functors.h"
#include "PorousResistance_Factory_impl.h"
#include "cppinterface.decl.h"
#include <iostream>
#include <math.h>
#include <sstream>
#include <fstream>

/**
 *  \brief Dummy implementation just returns a constant
 *  value for the permeability
 */
PorousResistance_constant::PorousResistance_constant() {
   permeability = 1.0;
}

PorousResistance_constant::~PorousResistance_constant() {
}

double PorousResistance_constant::operator ()(int nb, int ii) {
   return permeability;
}

/**
 *  \brief Nondimensional pressure drop
    \delta p d**2/\mu*u_superficial
     Reads the lengthscale and the coeffiencts for a linear fit
 */
PorousResistance_linear_fit::PorousResistance_linear_fit() {

   // Define the map of enumerations to string used in
   // cpp interface
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

PorousResistance_linear_fit::~PorousResistance_linear_fit() {
}

/**
 * Uses the local variables to calculate the Re number.
 * Gets the corresponding non-dimensional pressure drop.
 * Makes the pressure drop dimensional and returns it.
 * @param nb
 * @param ii
 * @return
 */
double PorousResistance_linear_fit::operator ()(int nb, int ii) {
   //interpolate delta_p_nd
   double loc_vel, loc_vel_s;
   double loc_hembed, loc_rho, loc_visc;
   double Re, delta_p_nd, delta_p;


   // Compute local values
   loc_vel = sqrt(pow(getValue(en_uvelocity,nb,ii),2)+
                  pow(getValue(en_vvelocity,nb,ii),2)+
                  pow(getValue(en_wvelocity,nb,ii),2));

   loc_hembed = getValue(en_hembed,nb,ii);
   loc_vel_s  = loc_vel * loc_hembed;
   loc_rho    = getValue(en_density,nb,ii);
   loc_visc   = getValue(en_viscosity,nb,ii);
   Re         = loc_vel_s * _lengthscale * loc_rho / (loc_visc + _small);

   // Get the nondimensional pressure drop from the
   // fit: delta_p_nd = coefficients[0] + coefficient[1] *Re
   delta_p_nd = 0.0;
   std::vector<double>::iterator it;
   int exponent;
   for (it= _coefficients.begin(), exponent=0;
        it != _coefficients.end();
        ++it,++exponent){
       delta_p_nd += (*it)*pow(Re,exponent);
   }
   // convert to dp/dL
   delta_p = delta_p_nd* loc_visc* loc_vel_s / pow(_lengthscale,2);
   return delta_p;
}

void PorousResistance_linear_fit::set_nb_pointers() {
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

void PorousResistance_linear_fit::read_coefficients() {
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

   infile.open("res_coefficients.dat");

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

double PorousResistance_linear_fit::getValue(nb_arrays_enum var, int nb,
   int ii) {
   return _nb_pointers[var].at(nb)[ii];
}
