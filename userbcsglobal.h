#ifndef _USERBCSGLOBAL_H
#define _USERBCSGLOBAL_H

#include "userbcsglobal_prototype.decl.h"
#include "line_output.h"

class UserBCsGlobal:public UserBCsGlobal_Prototype
{  
 private:
   //-----------------------------------------
   // add your classes here
   // to have access during the simulation
   //-----------------------------------------

 public:
   // allocation
   UserBCsGlobal();

   // deallocation
   ~UserBCsGlobal();

void User_BCs_Density(double*, double*, double, int, int, int);

void User_BCs_Uvelocity(double*, double*, double, int, int, int);

void User_BCs_Vvelocity(double*, double*, double, int, int, int);

void User_BCs_Wvelocity(double*, double*, double, int, int, int);

void User_BCs_Temperature(double*, double*, double, int, int, int);

void User_BCs_Concentration(double*, double*, double, int, int, int);

void User_BCs_TKE(double*, double*, double, int, int, int);

void User_BCs_Epsilon(double*, double*, double, int, int, int);

void User_BCs_Eddy_Viscosity_Ratio(double*, double*, double, int, int, int);

void User_BCs_Pressure(double*, double*, double, int, int, int);

void User_BCs_Exit_Pressure(double*, double*, double, int, int, int);

};

#endif 
