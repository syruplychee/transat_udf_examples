/*
 * PorousResistance_Functors.h
 *
 *  Created on: Sep 24, 2013
 *      Author: vonrickenbach
 */

#ifndef POROUSRESISTANCE_FUNCTORS_H_
#define POROUSRESISTANCE_FUNCTORS_H_
#include "PorousResistance_Base.decl.h"
#include <vector>
#include <string>
#include <map>

/**
 * Implementation for a constant porous resistance
 */
class PorousResistance_constant : public PorousResistance_Base {
public:
   PorousResistance_constant();
   virtual ~PorousResistance_constant();


   virtual double operator()(int nb, int ii);
private:
   double permeability;

};


class PorousResistance_linear_fit : public PorousResistance_Base {
public:

   PorousResistance_linear_fit();
   virtual ~PorousResistance_linear_fit();

    virtual double operator()(int nb, int ii);
private:

   // Declare a type for the nb pointers
   typedef std::vector<double*> nb_pointers_t;

   // Enumeration for the variables from fortran
   enum nb_arrays_enum{en_uvelocity,
                       en_vvelocity,
                       en_wvelocity,
                       en_density,
                       en_hembed,
                       en_viscosity};

   // Maps an enumeration to a given nb_pointer
   typedef std::map<nb_arrays_enum,nb_pointers_t> nb_pointers_map;

   // Maps an enumeration to the name used in set_pointer
   typedef std::map<nb_arrays_enum,std::string> nb_pointers_name_map;

   // Map of nb_pointers
   nb_pointers_map _nb_pointers;

   // Map of nb_pointer names
   nb_pointers_name_map _nb_pointers_name;

   // Coefficients for the linear fit
   std::vector<double> _coefficients;
   double _lengthscale;

   // Variables from fortran
   double _small;
   int _nblocks;

   // Creates nb_pointer objects, sets them to the fortran array
   // puts them into nb_pointers_map
   void set_nb_pointers();

   // Read the coefficients and the lengthscale from a file
   void read_coefficients();

   // Wrapper function to get the value of the fortran
   // array
   double getValue(nb_arrays_enum var,int nb,int ii);

};


#endif /* POROUSRESISTANCE_FUNCTORS_H_ */
