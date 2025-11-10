// template_class_name.h // 
#include "PhaseEntryPoint_Base.decl.h"
#include <vector>
#include <string>
#include <map>

class template_class_name : public PhaseEntryPoint_Base {
public:
   template_class_name();
   virtual ~template_class_name();
   static PhaseEntryPoint_Base* create(){return new template_class_name;};
   virtual void operator()(int iphase);
};
