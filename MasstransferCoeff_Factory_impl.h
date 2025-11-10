/*
 * MasstransferCoeff_Factory.impl.h
 *
 *  Created on: Sep 24, 2013
 *      Author: vonrickenbach
 */

#include "MasstransferCoeff_Factory.decl.h"
#include "MasstransferCoeff_Functors.h"
#include <iostream>

/**
* \brief Get a functor for the porous resistance model
* @param type
* @return MasstransferCoeff_Base*
*/
MasstransferCoeff_Base* MasstransferCoeff_Factory::get_functor(functor_type type){
   MasstransferCoeff_Base* func;
   switch (type){
      case en_no_coeff:
         return NULL;
         break;
      case en_constant:
         func = new MasstransferCoeff_constant();
         break;
      case en_fit:
         func = new MasstransferCoeff_linear_fit();
         break;
      default:
         std::cout << "Warning: invalid Masstransfer Functor"
         << std::endl;
    }
    return func;
}

