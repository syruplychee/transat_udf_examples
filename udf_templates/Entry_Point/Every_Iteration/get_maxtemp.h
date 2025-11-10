// template_class_name.h // 

#include "EntryPoint_Base.decl.h"
#include <vector>
#include <string>
#include <map>

class template_class_name : public EntryPoint_Base {
private:
   // Private members Time and Mass
   std::vector<double> Time;
   std::vector<double> Mass;
public:
   template_class_name();
   virtual ~template_class_name();
   static EntryPoint_Base* create(){return new template_class_name;};
   /**
    * @brief Function to be called at entry point.
    */
   virtual void operator()();
};
