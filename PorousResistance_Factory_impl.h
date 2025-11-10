/*
 * PorousResistance_Factory.impl.h
 *
 *  Created on: Sep 24, 2013
 *      Author: vonrickenbach
 */

#include "PorousResistance_Factory.decl.h"
#include "PorousResistance_Functors.h"
#include <iostream>


/**
* \brief Get a functor for the porous resistance model
* @param type
* @return PorousResistance_Base*
*/
PorousResistance_Base* PorousResistance_Factory::get_functor(functor_type type){
   PorousResistance_Base* func = NULL;
   switch (type){
      case en_no_coeff:
         func = NULL;
         break;
      case en_constant:
         func = new PorousResistance_constant();
         break;
      case en_fit:
         func = new PorousResistance_linear_fit();
         break;
      default:
         std::cout << "Warning: invalid Masstransfer Functor"
                   << std::endl;

    }
    return func;
}

