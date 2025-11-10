// template_class_name.h // 

#include <iostream>
#include <string>
#include <vector>
#include "cppinterface.decl.h"
#include "BoundaryManager.h"
#include "EntryPoint_Base.decl.h"

class template_class_name : public EntryPoint_Base {
private:
   BoundaryManager bcmag;
public:
   template_class_name();
   virtual ~template_class_name();
   static EntryPoint_Base* create(){return new template_class_name;};
   virtual void operator()();
};
