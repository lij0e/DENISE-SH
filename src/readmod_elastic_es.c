/*------------------------------------------------------------------------
 *   Read elastic model properties (vp,vs,density) from files  
 *
 *  Copyright (c)  T. Bohlen
 *  last update 29.06.2003
 *  ----------------------------------------------------------------------*/


/* This file contains function readmod, which has the purpose
   to read data from model-files for viscoelastic simulation */

#include "fd.h"

void readmod_elastic_es(float  **  rho, float **  pi, float **  u, float ** matmod, int is){

	extern int NX, NY, NXG, NYG,  POS[3], MYID, INVMAT1;
	extern float DH;
	extern char  MFILE[STRING_SIZE];	
	extern FILE *FP;

		
	/* local variables */
	float rhov, muv, piv, vp, vs;
	int i, j, ii, jj;
	FILE *fp_vs, *fp_vp, *fp_rho;
	char filename[STRING_SIZE];


        int h=47; /* depth of the seafloor (gridpoints) */



	   fprintf(FP,"\n...reading model information from model-files...\n");
           
	   /* read density and seismic velocities */
	   /* ----------------------------------- */
	   if(INVMAT1==1){ 
	   fprintf(FP,"\t Vp:\n\t %s.vp\n\n",MFILE);
	   sprintf(filename,"%s.vp",MFILE);
	   fp_vp=fopen(filename,"r");
	   if (fp_vp==NULL) err(" Could not open model file for Vp ! ");


	   fprintf(FP,"\t Vs:\n\t %s.vs\n\n",MFILE);
	   sprintf(filename,"%s.vs",MFILE);
	   fp_vs=fopen(filename,"r");
	   if (fp_vs==NULL) err(" Could not open model file for Vs ! ");

	   fprintf(FP,"\t Density:\n\t %s.rho\n\n",MFILE);
	   sprintf(filename,"%s.rho",MFILE);
	   fp_rho=fopen(filename,"r");
	   if (fp_rho==NULL) err(" Could not open model file for densities ! ");
           }
	   
	   /* read density and Lame parameters */
	   /* ----------------------------------- */
	   if(INVMAT1==3){ 
	   fprintf(FP,"\t Lame parameter lambda:\n\t %s.lam\n\n",MFILE);
	   sprintf(filename,"%s.lam",MFILE);
	   fp_vp=fopen(filename,"r");
	   if (fp_vp==NULL) err(" Could not open model file for Lame parameter lambda ! ");


	   fprintf(FP,"\t Lame parameter mu:\n\t %s.vs\n\n",MFILE);
	   sprintf(filename,"%s.mu",MFILE);
	   fp_vs=fopen(filename,"r");
	   if (fp_vs==NULL) err(" Could not open model file for Lame parameter mu ! ");

	   fprintf(FP,"\t Density:\n\t %s.rho\n\n",MFILE);
	   sprintf(filename,"%s.rho",MFILE);
	   fp_rho=fopen(filename,"r");
	   if (fp_rho==NULL) err(" Could not open model file for densities ! ");
           }


	/* loop over global grid */
		for (i=1;i<=NXG;i++){
			for (j=1;j<=NYG;j++){
			fread(&vp, sizeof(float), 1, fp_vp);
			fread(&vs, sizeof(float), 1, fp_vs);
			fread(&rhov, sizeof(float), 1, fp_rho);
			
			if(j>=h){
			   vp = (matmod[is][1] - matmod[is][4] * (h*DH)) + matmod[is][4] * (j*DH);
			   vs = (matmod[is][2] - matmod[is][5] * (h*DH)) + matmod[is][5] * (j*DH);
			   rhov = (matmod[is][3] - matmod[is][6] * (h*DH)) + matmod[is][6] * (j*DH);
			}
				
			/* only the PE which belongs to the current global gridpoint 
			is saving model parameters in his local arrays */
				if ((POS[1]==((i-1)/NX)) && 
				    (POS[2]==((j-1)/NY))){
					ii=i-POS[1]*NX;
					jj=j-POS[2]*NY;
                                
				u[jj][ii]=vs;
                                rho[jj][ii]=rhov;
                                pi[jj][ii]=vp;
				
				}
			}
		}
	




	fclose(fp_vp);
	fclose(fp_vs);
	fclose(fp_rho);
	
	
	/* each PE writes his model to disk */
	   
	   
	sprintf(filename,"%s.fdveps.vp",MFILE);

	writemod(filename,pi,3);

	MPI_Barrier(MPI_COMM_WORLD);

	if (MYID==0) mergemod(filename,3);
	
	
	sprintf(filename,"%s.fdveps.vs",MFILE);

	writemod(filename,u,3);

	MPI_Barrier(MPI_COMM_WORLD);

	if (MYID==0) mergemod(filename,3);

        sprintf(filename,"%s.fdveps.rho",MFILE);

	writemod(filename,rho,3);

	MPI_Barrier(MPI_COMM_WORLD);

	if (MYID==0) mergemod(filename,3);
}




