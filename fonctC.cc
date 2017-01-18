#include "Graphics.h"

int collision_part(Particle *p, Event *e_col,int Np, double diameter)  // cette fonction renvoi le temps min entre deux colision de particule
{
	int i,y,count=0; // on va tester pour chaque particule si elle va croiser une autre
	int num_t=0; // temps stocke le temps le plus faible et num_t la position correspondant 
	double tmp; // variable poubelle
	double temps=10E17;
	double DVX, DVY, DX, DY, a, b, c, delta; // variable pour calculer le temps de colision
	double t1, t2;

	for(i=0; i < Np; i++)
	{
		for(y=i+1; y<Np; y++)
		{

			DVX=p[i].vx-p[y].vx; // on recupère les valeurs 
			DVY=p[i].vy-p[y].vy;// utile pour calculer les coef du polynome
			DX=p[i].x-p[y].x;
			DY=p[i].y-p[y].y;
			
			a=(DVX*DVX+DVY*DVY);
			b=(DVX*DX+DVY*DY);
			c=(DX*DX+DY*DX-diameter*diameter);
			
			delta=b*b-4*a*c;
			printf("delta = %f\t", delta);	
			if (delta < 0)
			{
				e_col[count].type=particle;
				e_col[count].ia=i;
				e_col[count].ib=y;
				e_col[count].time=-1;		
			}
			else 
			{
				e_col[count].type=particle;
				e_col[count].ia=i;
				e_col[count].ib=y;
				
				t1=(-(b)-sqrt(delta))/(2*a);
				t2=(-(b)+sqrt(delta))/(2*a);
				printf("t1 = %f, t2=%f \n", t1, t2);	
				if (t1<0 && t2<0)
					tmp=-1;
				else if(t1>=0 && t2<=0 && b<0) // b est négative quand les boulle s'approche l'une de l'autre, correspond au produit scalaire de DV(x,y)  et DP(x,y)
					tmp=t1;
				else if(t1<0 && t2>=0 && b<0)
					tmp=t2;
				else if (b<0)
					(t1<t2)? (tmp=t1) : (tmp=t2);
				else
					tmp=-1;
				e_col[count].time=tmp;
				if (temps > tmp && tmp != -1)
				{
					temps=tmp;
					num_t=count;	
				}
				printf("b= %f tmp = %f, temps=%f \n   -------------------\n", b, tmp, temps);
			}
			count++;	
		}
	}
	return (num_t);
}
