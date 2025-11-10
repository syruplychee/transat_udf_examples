/*
 * template_class_name.h
 *
 */

#ifndef template_class_name_H_
#define template_class_name_H_
#include "UserDefinedPhaseProperties_Base.decl.h"
#include "TmbManager.h"

/**
 * @brief template_class_name class, derived from DragTerm_Base
 */
class template_class_name : public UserDefinedPhaseProperties_Base {
public:
   template_class_name();
   virtual ~template_class_name();
   static UserDefinedPhaseProperties_Base* create(){return new template_class_name;};
   
   /**
    * @brief This is the function that is called for calculating viscosity
    *        It is normally used as UDF. Here is a place holder and template
    *        the example uses Hershel Bulkley (Non-Newtonian)
    *
    *
    * @param mgr gives access to all the Properties Manager variables (viscosity)
    * @param iphase is access to the phase 
    */
   virtual double Viscosity(tmb::Properties_Manager* mgr,unsigned iphase);
private:
      /**
       * @brief System pressure is needed for many properties but it is a
       * constant in the domain and only changes in time
       * We maintain a pointer to this Fortran global variable
       */
      double* _system_pressure;
      /**
       * @brief prevents viscosity from becoming infinite at low shear-rates
       */
      double _small;
      double _smdiff;

      /**
       * @brief specifies yield stress as user-defined in INPUT
       */
      std::vector<double> _hershel_bulkley_tau;
      /**
       * @brief specifies the constant of proportionality between shear stress
       *        and shearrate raised to a certain power in the yielded region as
       *        user-defined in INPUT
       */
      std::vector<double> _hershel_bulkley_k;
      /**
       * @brief specifies the power by which the shear rate is raised so that it
       *        has a linear relation with shear_stress as user-defined in INPUT
       */
      std::vector<double> _hershel_bulkley_n;
      /**
       * \brief get computes the value of Hershel_Bulkley viscosity at a point
       *        from the value of Hershel_Bulkley scalar computed at that
       *        point and other user-defined parameters in INPUT like
       *        hershel-bulkley-tau, hershel-bulkley-k and hershel-bulkley-n
       *
       * \param shear_rate is the magnitude of shear_rate at each cell
       * \return the value of viscosity at each cell
       **/
      double _get(double shear_rate, unsigned);

};

#endif /* template_class_name_H_ */
