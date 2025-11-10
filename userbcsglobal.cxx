#include "userbcsglobal.h"
#include "userbcs_interface.decl.h"

UserBCsGlobal::UserBCsGlobal(){
}

UserBCsGlobal::~UserBCsGlobal(){
}

void UserBCsGlobal::User_BCs_Density(double*, double*, double, int, int, int){};
void UserBCsGlobal::User_BCs_Uvelocity(double*, double*, double, int, int, int){};
void UserBCsGlobal::User_BCs_Vvelocity(double*, double*, double, int, int, int){};
void UserBCsGlobal::User_BCs_Wvelocity(double*, double*, double, int, int, int){};
void UserBCsGlobal::User_BCs_Temperature(double*, double*, double, int, int, int){};
void UserBCsGlobal::User_BCs_Concentration(double*, double*, double, int, int, int){};
void UserBCsGlobal::User_BCs_TKE(double*, double*, double, int, int, int){};
void UserBCsGlobal::User_BCs_Epsilon(double*, double*, double, int, int, int){};
void UserBCsGlobal::User_BCs_Eddy_Viscosity_Ratio(double*, double*, double, int, int, int){};
void UserBCsGlobal::User_BCs_Pressure(double*, double*, double, int, int, int){};
void UserBCsGlobal::User_BCs_Exit_Pressure(double*, double*, double, int, int, int){};
