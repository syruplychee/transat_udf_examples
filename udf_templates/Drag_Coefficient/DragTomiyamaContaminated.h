/*
 * template_class_name.h
 *
 */

#ifndef template_class_name_H_
#define template_class_name_H_
#include "DragTerm_Base.decl.h"

/**
 * @brief template_class_name class, derived from DragTerm_Base
 */
class template_class_name : public DragTerm_Base {
public:
   template_class_name();
   virtual ~template_class_name();
   static DragTerm_Base* create(){return new template_class_name;};
   double _small;
  
   /**
    * @brief This is the function that is called for computing 
    *        the slip flux. If Drag is the only interphase slip force (no Lift or 
    *        wall lubrication), then you need to use this function. Otherwise if 
    *        the simulation includes Lift or Wall forces, write your UDF function
    *        within the second operator function. 
    *
    *
    * @param ff friction factor that the drag model will calculate
    * @param ffD derivative of ff. Needed because of Newton-Raphson method
    * @param vel_slip_mag slip velocity magnitude
    * @param Re Reynolds number from slip velocity magnitude
    * @param Eo Eotvos number
    */
   virtual void operator()(double &ff, double &ffD, double vel_slip_mag, double Re, double Eo);

   /**
    * @brief This is the function that is called for computing 
    *        the slip velocity. If the simulation includes Lift 
    *        or Wall forces, write your UDF function here. Otherwise,
    *        if Drag is the only interphase slip force (no Lift or 
    *        wall lubrication), then you need to use the previous operator function.
    *        This function uses slip velocity components, in addition to magnitude.
    *
    * @param ff friction factor that the drag model will calculate
    * @param ffDx derivative of ff in x direction. Use u to find derivative
    * @param ffDy derivative of ff in y direction. Use v to find derivative
    * @param ffDz derivative of ff in z direction. Use w to find derivative
    * @param vel_slip_mag slip velocity magnitude
    * @param u slip velocity component in x direction
    * @param v slip velocity component in y direction
    * @param w slip velocity component in z direction
    * @param Re Reynolds number from slip velocity magnitude
    * @param Eo Eotvos number
    */
   virtual void operator()(double &ff, double &ffDx, double &ffDy, double &ffDz,
                           double vel_slip_mag, double u_slip, double v_slip, double w_slip, double Re, double Eo);
};

#endif /* template_class_name_H_ */
