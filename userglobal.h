#ifndef _USERGLOBAL_H
#define _USERGLOBAL_H

#include "userglobal_prototype.decl.h"
#include "line_output.h"

class UserGlobal:public UserGlobal_Prototype
{  
 private:
   //-----------------------------------------
   // add your classes here
   // to have access during the simulation
   //-----------------------------------------

 public:
   //--------------//
   // entry points //
   //--------------//
   //
   void User_processing_everyiteration();

   void User_iteration_begin();

   void User_iteration_end();

   void User_processing_everystep();

   void User_processing_everyparticlesubstep();

   void User_processing_begin();

   void User_processing_end();

   void User_processing_outputfreq();

   void User_processing_beforesolver();

   void User_processing_init();

   void User_add_source_term(char*);

   void User_add_phase_source_term(char*, int);

   void User_add_phase_component_source_term(char*, int, int);

   // allocation
   UserGlobal();

   // deallocation
   ~UserGlobal();

};

#endif 
