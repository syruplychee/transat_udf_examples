// template_class_name.h //
#include "EntryPoint_Base.decl.h"
#include <vector>
#include <string>
#include <map>

class FireSource;

class template_class_name : public EntryPoint_Base {
private:
  FireSource* myHeatSource;
public:
   template_class_name();
   virtual ~template_class_name();
   static EntryPoint_Base* create(){return new template_class_name;};
   virtual void operator()();
};
