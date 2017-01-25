/*
Ce fichier de fonction contient la gestion de collision entre particule, une fonction pour obtenir la température, une fonction pour retourner des données pour la création d'un histogramme avec Matlab
Martel Caroff Janvier 2017
*/

#include "Graphics.h"

int collision_part(Particle *p, Event *e_col,int np, double diameter)  // cette fonction renvoi le numéro de l'événement correspondant au temps min entre deux colision de particule et remplie le tableau d'évenement associer
{
	int i,y,count=0; // on va tester pour chaque particule si elle va croiser une autre
	int num_t=0; // temps stocke le temps le plus faible et num_t la position correspondant 
	double tmp; // variable poubelle
	double temps=10e42;
	double dvx, dvy, dx, dy, a, b, c, delta; // variable pour calculer le temps de colision
	double t1, t2;

	if(np==1)
	{
		e_col[0].time=10e42;
		return 0;
	}
	for(i=0; i < np; i++)
	{
		for(y=i+1; y<np; y++)
		{
			dvx=p[i].vx-p[y].vx; // on recupère les valeurs 
			dvy=p[i].vy-p[y].vy;// utile pour calculer les coef du polynome
			dx=p[i].x-p[y].x;
			dy=p[i].y-p[y].y;
			
			a=(dvx*dvx+dvy*dvy);
			b=2*(dvx*dx+dvy*dy);
			c=(dx*dx+dy*dy-diameter*diameter);
			
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

double temperature(Particle *p, int nb, double m)
{
	int i; // on a t = m<v^2>/kb on prendra pour la masse la surface de la particule. 
	double v=0;
	for(i=0; i<nb; i++)
	{
		v=v+(p[i].vx*p[i].vx+p[i].vy*p[i].vy);
	}
	return (m*v/nb);
}

void creathist(Particle *p, int np, double* normev, double* compoVx, double* compoVy, int go) //les tableaux comprend les valeurs qu'il faudra utiliser prendre pour crée l'histograme, les tableau sont créer dans le main.
{	
	static int count=0; // variable qui va stocker le nombre de fois ou on appelle la fonction pour faire la moyenne;
	char NOMnorme[100];
	char NOMVx[100];
	char NOMVy[100];
	
	int j;
	FILE* norme = NULL;
	FILE* vx = NULL;
	FILE* vy = NULL;
	for (j=0; j<np; j++)
	{
		normev[j]=normev[j]+(p[j].vx*p[j].vx+p[j].vy*p[j].vy);
		compoVx[j]=compoVx[j]+p[j].vx;
		compoVy[j]=compoVy[j]+p[j].vy;
	}
	
	if(go)
	{
		snprintf(NOMnorme, sizeof(NOMnorme), "HISTO/norme.dat");
		snprintf(NOMVx, sizeof(NOMVx), "HISTO/vx.dat");
		snprintf(NOMVy, sizeof(NOMVy), "HISTO/vy.dat");
		
		norme=fopen(NOMnorme,"w");
		vx=fopen(NOMVx,"w");
		vy=fopen(NOMVy,"w");
		
		if (norme == NULL || vx == NULL || vy == NULL)
		{
			printf("Erreur création de fichier histogramme \n");
			exit(42);
		
		}
		for (j=0; j<np; j++)
		{
			fprintf(norme, "%f\n", normev[j]/count);
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
		printf("Allocation memoire tableau de double raté\n");
		exit(-13);
	}
	return(pouet);

}

