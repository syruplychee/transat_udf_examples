// template_class_name.cxx //

#include<stdio.h>
#include<math.h>
#include"cppinterface.h"
#include<string>
#include "template_class_name.h"

template_class_name::template_class_name() {
}

template_class_name::~template_class_name() {
}

void template_class_name::operator ()() {

   int i, j, k, ii;
   int i_index;
   int nblocks, ni, nj, nij;
   double *p,*u,*v,*yc,*xc,*y,*phi,*cp;
   double ycoord;
   FILE *testunit;

   double xlines;
   std::string filenames;

   xlines    = 0.00018;
   filenames = "test_output.dat";
  
   nblocks = get_integer("nblocks");

   for (int nb=0; nb < nblocks; nb++){
     ni = get_integer(nb,"ni");
     nj = get_integer(nb,"nj");
     nij = get_integer(nb,"nij");

     set_pointer(nb,&p,"pressure");
     set_pointer(nb,&u,"uvelocity");
     set_pointer(nb,&v,"vvelocity");
     set_pointer(nb,&yc,"cellcornery");
     set_pointer(nb,&xc,"cellcornerx");
     set_pointer(nb,&y,"cellcentery");
     set_pointer(nb,&phi,"levelset");

     j = 3;
     k = 3;
     i_index = -1;
     for (i = 3; i <= ni-3; i++){
        ii = (i-1) + (j-1)*ni + (k-1)*nij;
        if (( xc[ii] < xlines )&&( xc[ii+1] >= xlines)) i_index = i;
     }

     if (i_index > 0) {
        testunit = fopen(filenames.c_str(),"w");
        fprintf(stdout,"Writing test output file: %s\n",filenames.c_str());
        fprintf(testunit,"# Flow profiles\n");
        fprintf(testunit,"# 1: y-coordinate\n");
        fprintf(testunit,"# 2: pressure\n");
        fprintf(testunit,"# 3: U-velocity\n");
        fprintf(testunit,"# 4: V-velocity\n");
        fprintf(testunit,"# 5: Level Set\n");
        i = i_index;
        k = 3;
        for (j = 2; j <= nj-2; j++){
           ii = (i-1) + (j-1)*ni + (k-1)*nij;
           ycoord = y[ii];
           if (j ==    2) ycoord = yc[ii+ni];
           if (j == nj-2) ycoord = yc[ii   ];
           fprintf(testunit,"%18.12e %18.12e %18.12e %18.12e %18.12e\n",ycoord,p[ii],u[ii],v[ii],phi[ii]);
        }
        fclose(testunit);
     }
   }
}
