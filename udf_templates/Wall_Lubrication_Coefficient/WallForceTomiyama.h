/*
 * template_class_name.h
 *
 */

#ifndef template_class_name_H_
#define template_class_name_H_
#include "WallForceCoeff_Base.decl.h"

class template_class_name : public WallForceCoeff_Base {
public:
   template_class_name();
   virtual ~template_class_name();
   static WallForceCoeff_Base* create(){return new template_class_name;};
   double _small;

   /**
    * @brief Function for determining the wall coefficient for Tomiyama wall
    *        lubrication model
    *
    * @param Cw wall coefficient
    * @param yw distance from wall
    * @param D dispersed phase (bubble/drop/particle) diameter
    * @param Eo Eotvos number
    */
   virtual void operator()(double &Cw, double yw, double D, double Eo);
};

#endif /* template_class_name_H_ */
