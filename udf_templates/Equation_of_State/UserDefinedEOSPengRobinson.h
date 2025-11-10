/*
 * template_class_name.h
 *
 */

#ifndef template_class_name_H_
#define template_class_name_H_
#include "UserDefinedPhaseProperties_Base.decl.h"
#include "TmbManager.h"

/**
 * @brief template_class_name class, derived from the Base Class
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
   virtual double Density(tmb::Properties_Manager* mgr,unsigned iphase);
   virtual double Soundspeed(tmb::Properties_Manager* mgr,unsigned iphase);
   virtual double DrhoDp(tmb::Properties_Manager* mgr,unsigned iphase);
   virtual double DrhoDp_t(tmb::Properties_Manager* mgr,unsigned iphase);
   virtual double DrhoDt_p(tmb::Properties_Manager* mgr,unsigned iphase);
private:
      int    _STATE_gas;
      int    _STATE_liquid;
      int    _EN_antoine;
      int    _EN_satcurv_water;
      int    _EN_satcurv_ammonia;
      /**
       * @brief All divisions are done after adding _small to the divisor
       * All comparisons are done using _smdiff.
       */
      double _small;
      double _smdiff;

      /**
       * @brief User Defined parameters are needed for many properties but are
       * constants in the domain and only changes in time
       * We maintain a pointer to this Fortran global variable
       */
      std::vector<double> _k;
      std::vector<double> _VT;
      std::vector<double> _Pc;
      std::vector<double> _Tc;

      /**
       * @brief System pressure is needed for many properties but it is a
       * constant in the domain and only changes in time
       * We maintain a pointer to this Fortran global variable
       */
      double* _system_pressure;

      /**
       * @brief state of phase. User Defined depends on state of phase
       *        using get_integer is computationally expensive, for faster
       *        call of User Defined need to get state of phase in constructor
       *        as a pointer and use as a member
       */
      std::vector<int>_state_of_phase;

      /**
       * @brief Saturation curve method, if desired to bound by Saturation values
       *        currently curve fit is only available from antoine equation.
       *        using get_integer is computationally expensive, for faster
       *        call of User Defined need to get curve fit method in constructor
       *        as a pointer and use as a member
       */
      std::vector<int> _saturation_curve_method;

      /**
       * @brief This bounds the Pressure or the temperature depending
       *        on the state of phase
       *
       * @param P input pressure, in-out
       * @param T input temperature, in-out
       */
      void _bound(double*, double*, tmb::Properties_Manager*, unsigned);
};

#endif /* template_class_name_H_ */
