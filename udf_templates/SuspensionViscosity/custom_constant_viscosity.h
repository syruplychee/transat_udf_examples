/*
 * template_class_name.h
 *
 *  Created on: Sep 24, 2013
 *      Author: vonrickenbach
 */

#include "SuspensionViscosity_Base.decl.h"
#include <vector>
#include <string>
#include <map>

class template_class_name : public SuspensionViscosity_Base {
public:
   template_class_name();
   virtual ~template_class_name();
   static SuspensionViscosity_Base* create(){return new template_class_name;};
   virtual void operator()(double &var_out);
};
