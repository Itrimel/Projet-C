#include "Graphics.h"

int collision_part(Particle *p, Event *e_col,int Np, double diameter)  // cette fonction renvoi le temps min entre deux colision de particule
{
	int i,y,count=0; // on va tester pour chaque particule si elle va croiser une autre
	int num_t=0; // temps stocke le temps le plus faible et num_t la position correspondant 
	double tmp; // variable poubelle
	double temps=10E42;
	double DVX, DVY, DX, DY, a, b, c, delta; // variable pour calculer le temps de colision
	double t1, t2;
	if(Np==1)
	{
		e_col[0].time=10e42;
		return 0;
	}
	for(i=0; i < Np; i++)
	{
		for(y=i+1; y<Np; y++)
		{
			DVX=p[i].vx-p[y].vx; // on recupère les valeurs 
			DVY=p[i].vy-p[y].vy;// utile pour calculer les coef du polynome
			DX=p[i].x-p[y].x;
			DY=p[i].y-p[y].y;
			
			a=(DVX*DVX+DVY*DVY);
			b=2*(DVX*DX+DVY*DY);
			c=(DX*DX+DY*DY-diameter*diameter);
			
			delta=b*b-4*a*c;
			if (delta < 0)
			{
				e_col[count].type=particle;
				e_col[count].ia=i;
				e_col[count].ib=y;
				e_col[count].time=10e42;		
			}
			else 
			{
				e_col[count].type=particle;
				e_col[count].ia=i;
				e_col[count].ib=y;
				
				t1=(-b-sqrt(delta))/(2*a);
				//t2=(-b+sqrt(delta))/(2*a);
				
				if(t1>0)
					tmp=t1;
				else 
					tmp=10e42;
				
				e_col[count].time=tmp;
				if (temps > tmp)
				{
					temps=tmp;
					num_t=count;	
				}
			}
			count++;	
		}
	}
	return (num_t);
}

double temperature(Particle *p, int Nb, double m)
{
	int i; // On a T = m<v^2>/Kb on prendra pour la masse la surface de la particule. 
	double V=0;
	for(i=0; i<Nb; i++)
	{
		V=V+(p[i].vx*p[i].vx+p[i].vy*p[i].vy);
	}
	return (m*V/Nb);
}

void creathist(Particle *p, int Np, double* normeV, double* compoVx, double* compoVy, int GO) //le tableau comprend les valeurs qu'il faudra utiliser prendre pour crée l'histograme
{	
	static int count=0; // variable qui va stocker le nombre de fois ou on appelle la fonction pour faire la moyenne;
	char NOMnorme[100];
	char NOMVx[100];
	char NOMVy[100];
	
	int j;
	FILE* norme = NULL;
	FILE* vx = NULL;
	FILE* vy = NULL;
	for (j=0; j<Np; j++)
	{
		normeV[j]=normeV[j]+(p[j].vx*p[j].vx+p[j].vy*p[j].vy);
		compoVx[j]=compoVx[j]+p[j].vx;
		compoVy[j]=compoVy[j]+p[j].vy;
	}
	
	if(GO)
	{
		snprintf(NOMnorme, sizeof(NOMnorme), "HISTO/norme.dat");
		snprintf(NOMVx, sizeof(NOMVx), "HISTO/vx.dat");
		snprintf(NOMVy, sizeof(NOMVy), "HISTO/vy.dat");
		
		norme=fopen(NOMnorme,"w");
		vx=fopen(NOMVx,"w");
		vy=fopen(NOMVy,"w");
		
		if (norme == NULL || vx == NULL || vy == NULL)
		{
			printf("TOCARD * 42 \n");
			exit(42);
		
		}
		for (j=0; j<Np; j++)
		{
			fprintf(norme, "%f\n", normeV[j]/count);
			fprintf(vx, "%f\n", compoVx[j]/count);
			fprintf(vy, "%f\n", compoVy[j]/count);
	}
		fclose(norme);
		fclose(vx);
		fclose(vy);
	}
	count++;
}

double* allocdouble(int Np) // cette fonction crée un tableau de Nb double initialisé à zero
{
	double *pouet = NULL;
	pouet = (double*)calloc(Np, sizeof(double));
	if (pouet == NULL)
	{
		printf("Allocation fail\n");
		exit(-13);
	}
	return(pouet);

}


