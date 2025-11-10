// template_class_name.cxx //

#include "template_class_name.h"
#include "cppinterface.decl.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream>
#include <toolbox.h>
#include "mpi.h"

template_class_name::template_class_name() {
}

template_class_name::~template_class_name() {
}

void template_class_name::operator ()() {
   unsigned glob_rank = static_cast<unsigned>(get_integer("glob_rank"));
   if (glob_rank == 0) {
      // open file (append mode)
      std::ofstream myfile;
      myfile.open("particleposition.dat", std::ofstream::out | std::ofstream::trunc);

      myfile << "#-------------------------------------------------------\n";
      myfile << "# 1: time s t\n",
      myfile << "# 2: time_step - tstep\n",
      myfile << "# 3: particle_substep - ptstep\n",
      myfile << "# 4: ip_proc - gip\n",
      myfile << "# 5: block_no - gib\n",
      myfile << "# 6: particle_xpos m x_p\n",
      myfile << "# 7: particle_ypos m y_p\n",
      myfile << "# 8: particle_zpos m z_p\n",
      myfile << "#-------------------------------------------------------\n";

      // close file
      myfile.close();
   }

}
