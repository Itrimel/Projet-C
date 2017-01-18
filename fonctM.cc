#include "Graphics.h"
void update_pos(Particle *p,int Np,double tau)
//Cette fonction permet de mettre à jour les positions de toutes les particules, en fonction de la valeur de temps donnée
{
	int i;
	for(i=0;i<Np;i++)
	{
		p[i].x=p[i].x+p[i].vx*tau;
		p[i].y=p[i].y+p[i].vy*tau;
	}
}
void update_vit(Event *e, int nm_e, Particle *p)
//Cette fonction permet de mettre à jour la vitesse d'une particule, en fonction de l'évenement considéré, repéré par e et nm_e
{
	int x1,x2,y1,y2,vx1,vy1,vx2,vy2,inter;
	switch(e[nm_e].type)
	{
		case bottom:// dans le cas du choc contre le mur, il suffit d'inverser la bonne composante
		case top:
			p[nm_e].vy*=-1;
			break;
		case left:
		case right:
			p[nm_e].vx*=-1;
			break;
		case particle:// Pour le choc entre 2 particules, l'évolution est moins évidente
			x1=p[e[nm_e].ia].x; // On récupère toute les valeurs utiles
			y1=p[e[nm_e].ia].y;
			x2=p[e[nm_e].ib].x;
			y2=p[e[nm_e].ib].y;
			vx1=p[e[nm_e].ia].vx;
			vy1=p[e[nm_e].ia].vy;
			vx2=p[e[nm_e].ib].vx;
			vy2=p[e[nm_e].ib].vy;
			inter=((x2-x1)*(vx1-vx2)+(y2-y1)*(vy1-vy2))/((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));//Et on fait les calculs nécessaires
			p[e[nm_e].ia].vx=vx1-inter*(x2-x1);
			p[e[nm_e].ia].vy=vy1-inter*(y2-y1);
			p[e[nm_e].ib].vx=vx2+inter*(x2-x1);
			p[e[nm_e].ib].vy=vy1+inter*(y2-y1);
			break;
					
	}
}

int collision_mur(Particle *p,Event *e,int Np, double diameter, double Lmax)
{
	// Utilise la géométrie carrée (x et y compris entre 0 et Lmax) pour voir les chocs contre les murs
	int i,cas,e_min;
	double time1,time2,time_min,vx,vy;
	for(i=0;i<Np;i++)
	{
		cas=0;/* On regarde dans quelle direction la particule se déplace : 00 signifie vers bas gauche, 01 signifie
		vers bas droite, 10 vers haut gauche, 11 vers haut droite */
		vx=p[i].vx;
		vy=p[i].vy;
		if(vx>0)
			cas++;
		if(vy>0)
			cas+=10;
		switch(cas)/* Pour chaque cas, on regarde lequel des 2 murs la particule rencontre en premier, 
							et on attribue alors le bon évenement et le bon temps*/
		{
			case 0:
				time1=-(p[i].y-diameter/2)/vy;//Choc avec le mur du bas. Le signe moins est présent, car la vitesse est négative
				time2=-(p[i].x-diameter/2)/vx;//choc avec le mur de gauche
				if(time1<time2)
				{
					e[i].time=time1;
					e[i].ia=i;
					e[i].type=bottom;
				}
				else
				{
					e[i].time=time2;
					e[i].ia=i;
					e[i].type=left;
				}
				break;
			case 1:
				time1=-(p[i].y-diameter/2)/vy;
				time2=(Lmax-p[i].x-diameter/2)/vx;//choc avec le mur de droite
				if(time1<time2)
				{
					e[i].time=time1;
					e[i].ia=i;
					e[i].type=bottom;
				}
				else
				{
					e[i].time=time2;
					e[i].ia=i;
					e[i].type=right;
				}
				break;
			case 10:
				time1=(Lmax-p[i].y-diameter/2)/vy;//choc avec le mur du haut
				time2=-(p[i].x-diameter/2)/vx;
				if(time1<time2)
				{
					e[i].time=time1;
					e[i].ia=i;
					e[i].type=top;
				}
				else
				{
					e[i].time=time2;
					e[i].ia=i;
					e[i].type=left;
				}
				break;
			case 11:
				time1=(Lmax-p[i].y-diameter/2)/vy;
				time2=(Lmax-p[i].x-diameter/2)/vx;
				if(time1<time2)
				{
					e[i].time=time1;
					e[i].ia=i;
					e[i].type=top;
				}
				else
				{
					e[i].time=time2;
					e[i].ia=i;
					e[i].type=right;
				}
				break;
			}
		if(i==0)/* Au début de la boucle for , on doit initialiser le temps minimum à la première valeur, 
																						afin de ne pas avoir de problèmes*/
		{
			e_min=0;
			time_min=e[0].time;
		}
		else if(e[i].time<time_min)// Si le rebond considéré arrive plus vite que le minimum précédent, on change le minimum
		{
			e_min=i;
			time_min=e[i].time;
		}
	}
	return e_min;// On retourne l'emplacement dans le tableau de l'événement arrivant le plus vite
}
		
		
			
