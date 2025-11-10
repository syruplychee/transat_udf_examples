// template_class_name.h //
#include "EntryPoint_Base.decl.h"

class Fan;

class template_class_name : public EntryPoint_Base {
private:
   Fan *myFan;
public:
   template_class_name();
   virtual ~template_class_name();
   static EntryPoint_Base* create(){return new template_class_name;};
   virtual void operator()();
};
