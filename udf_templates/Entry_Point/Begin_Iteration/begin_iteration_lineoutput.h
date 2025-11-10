// template_class_name.h // 

#include "EntryPoint_Base.decl.h"
#include <vector>
#include <string>
#include <map>
#include "line_output.h"

class template_class_name : public EntryPoint_Base {
private:
   // line_output data type
   line_output *output;
public:
   template_class_name();
   virtual ~template_class_name();
   static EntryPoint_Base* create(){return new template_class_name;};
   /**
    * @brief Function to implement the UDF
    *        the current template prints line output of velocity and temperature
    *        at the beginning of an iteration. This could be compared against
    *        end_iteration_lineoutput
    */
   virtual void operator()();
};
