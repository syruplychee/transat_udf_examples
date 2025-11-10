/*
 * template_class_name.h
 *
 */

#ifndef template_class_name_H_
#define template_class_name_H_
#include "LiftCoeff_Base.decl.h"

class template_class_name : public LiftCoeff_Base {
public:
   template_class_name();
   virtual ~template_class_name();
   static LiftCoeff_Base* create(){return new template_class_name;};
   double _small;

   /**
    * @brief This is the function where Lift UDFs can be implemented.
    *        The current template shows the Tomiyama lift model.
    *
    * @param Cl Lift coefficient, to be defined by the UDF 
    * @param ClDx Derivative of Cl in x direction. Needed because of Newton-Raphson method
    * @param ClDy Derivative of Cl in y direction. Needed because of Newton-Raphson method
    * @param ClDz Derivative of Cl in z direction. Needed because of Newton-Raphson method
    * @param vel_slip_mag slip velocity magnitude
    * @param u slip velocity component in x direction
    * @param v slip velocity component in y direction
    * @param w slip velocity component in z direction
    * @param Re Reynolds number from slip velocity magnitude
    * @param Eo Eotvos number
    */
   virtual void operator()(double &Cl, double &ClDx, double &ClDy, double &ClDz,
                           double vel_slip_mag, double u_slip, double v_slip, double w_slip, double Re, double Eo);
};

#endif /* template_class_name_H_ */
