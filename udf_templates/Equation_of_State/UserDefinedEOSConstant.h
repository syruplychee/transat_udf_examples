/*
 * template_class_name.h
 *
 */

#ifndef template_class_name_H_
#define template_class_name_H_
#include "UserDefinedPhaseProperties_Base.decl.h"
#include "TmbManager.h"
#include "TmbProperties.decl.h"

/**
 * @brief template_class_name class, derived from the Base class
 */
class template_class_name : public UserDefinedPhaseProperties_Base {
public:
   template_class_name();
   virtual ~template_class_name();
   static UserDefinedPhaseProperties_Base* create(){return new template_class_name;};
   
   /**
    * @brief This is the function that is called for calculating density
    *        It is normally used as UDF. Here is a place holder and template
    *        the example uses Peng Robinson
    *
    *
    * @param mgr gives access to all the Properties Manager variables (Density, viscosity ...)
    * @param iphase is access to the phase 
    */
   virtual double Density(tmb::Properties_Manager*,unsigned);
   virtual double Soundspeed(tmb::Properties_Manager* mgr,unsigned iphase);
   virtual double DrhoDp(tmb::Properties_Manager* mgr,unsigned iphase);
   virtual double DrhoDp_t(tmb::Properties_Manager* mgr,unsigned iphase);
   virtual double DrhoDt_p(tmb::Properties_Manager* mgr,unsigned iphase);
private:
      /**
       * @brief System pressure is needed for many properties but it is a
       * constant in the domain and only changes in time
       * We maintain a pointer to this Fortran global variable
       */
      double* _system_pressure;
      /**
       * @brief All divisions are done after adding _small to the divisor
       * All comparisons are done using _smdiff.
       */
      double _small;
      double _smdiff;
};

#endif /* template_class_name_H_ */
