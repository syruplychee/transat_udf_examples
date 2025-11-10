// template_class_name.h // 
#include "PhaseComponentEntryPoint_Base.decl.h"
#include <vector>
#include <string>
#include <map>

class template_class_name : public PhaseComponentEntryPoint_Base {
public:
   template_class_name();
   virtual ~template_class_name();
   static PhaseComponentEntryPoint_Base* create(){return new template_class_name;};
   virtual void operator()(int iphase, int icomponent);
};
