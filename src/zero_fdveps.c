/*------------------------------------------------------------------------
 *   zero wavefield
 *  
 *  
 *   last update 17/02/07, T. Bohlen
 *
 *  ----------------------------------------------------------------------*/

#include "fd.h"

void zero_fdveps(int ny1, int ny2, int nx1, int nx2, float ** vy, float ** syy, float ** sxy, float ** vym1, float ** vyp1,
                 float ** psi_sxy_x, float ** psi_vyx, float ** psi_syy_y, float ** psi_vyy){



	register int i, j, k;
	extern int FW, NX, NY;

	
		for (j=ny1;j<=ny2;j++){
			for (i=nx1;i<=nx2;i++){
			
				vy[j][i]=0.0;
				syy[j][i]=0.0;
                sxy[j][i]=0.0;
                vym1[j][i]=0.0;
				vyp1[j][i]=0.0;
				
			}
		}
		
		for (j=1;j<=NY;j++){
		         for (i=1;i<=2*FW;i++){
		 
		                psi_sxy_x[j][i] = 0.0;
		                psi_vyx[j][i] = 0.0;   
		 
		         }
		}
		
		for (j=1;j<=2*FW;j++){
		         for (i=1;i<=NX;i++){
		                
		                
		                psi_syy_y[j][i] = 0.0;
		                /*psi_sxy_y[j][i] = 0.0;*/
		                psi_vyy[j][i] = 0.0;
		                
		         }
		}            
	
}
