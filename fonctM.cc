/*Ce fichier contient les fonctions qui permettent de mettre à jour les positions et vitesses des particules
, qui permettent de traiter les collisions des particules avec les murs 
et de calculer le temps avant contact sur un arc de cercle dans le cas du stade
MARTEL Théo		CAROFF Théo		Janvier 2017*/


#include "Graphics.h"
//#define MESURE_TEMPS_VOL 1 
//mettre à 1 pour afficher dans le terminal les valeurs des temps et distances de vol à chaque collision réalisée. 
//Cette option est contrôlée dans le Makefile

void update_pos(Particle *p,int Np,double tau)
//Cette fonction permet de mettre à jour les positions de toutes les particules, en fonction de la valeur de temps donnée
{
	int i;
	for(i=0;i<Np;i++)
	{
		p[i].x=p[i].x+p[i].vx*tau;
		p[i].y=p[i].y+p[i].vy*tau;
		if(MESURE_TEMPS_VOL)
			p[i].time+=tau;
	}
}
double update_vit(Event *e, int nm_e, Particle *p, double Lmax)
//Cette fonction permet de mettre à jour la vitesse d'une particule, en fonction de l'évenement déclenché, repéré par e et nm_e
//Retourne la composante normale à la surface élevée au carré de la vitesse dans le cas de chocs contre un mur, 
//pour pouvoir effectuer les calculs de pression
{
	double x,y,vx,vy,x1,x2,y1,y2,vx1,vy1,vx2,vy2,inter,a,b,det;
	switch(e[nm_e].type)
	{
		case bottom:// dans le cas du choc contre le mur, il suffit d'inverser la bonne composante
		case top:
			if(MESURE_TEMPS_VOL)//Dans le cas où on veut mesurer les temps de vol
			{
				std::cout<<p[nm_e].time<<"\t"<<p[nm_e].time*pow(p[nm_e].vx*p[nm_e].vx+p[nm_e].vy*p[nm_e].vy,0.5)<<std::endl;//Afficher temps et distance depuis dernière collision
				p[nm_e].time=0;//On met le temps depuis collision à zéro
			}
			p[nm_e].vy*=-1;
			return p[nm_e].vy*p[nm_e].vy;
		case left:
		if(STADE){// Dans le cas où on vuet un stade
			if(MESURE_TEMPS_VOL)
			{
				std::cout<<p[nm_e].time<<"\t"<<p[nm_e].time*pow(p[nm_e].vx*p[nm_e].vx+p[nm_e].vy*p[nm_e].vy,0.5)<<std::endl;
				p[nm_e].time=0;
			}
			x=p[nm_e].x;
			y=p[nm_e].y;
			vx=p[nm_e].vx;
			vy=p[nm_e].vy;
			det=x*x+(y-Lmax/2)*(y-Lmax/2);//Calcul des composantes normales et tangentes de la vitesse par rapport au centre du cercle
			a=(vx*x+vy*(y-Lmax/2))/det;
			b=(x*vy-vx*(y-Lmax/2))/det;
			a=-a;
			p[nm_e].vx=a*x-b*(y-Lmax/2);//Mise à jour des vitesses
			p[nm_e].vy=a*(y-Lmax/2)+b*x;
			return a*a;}
		case right:
		if(MESURE_TEMPS_VOL)
			{
				std::cout<<p[nm_e].time<<"\t"<<p[nm_e].time*pow(p[nm_e].vx*p[nm_e].vx+p[nm_e].vy*p[nm_e].vy,0.5)<<std::endl;
				p[nm_e].time=0;
			}
		if(STADE){
			x=p[nm_e].x-Lmax;
			y=p[nm_e].y;
			vx=p[nm_e].vx;
			vy=p[nm_e].vy;
			det=x*x+(y-Lmax/2)*(y-Lmax/2);
			a=(vx*x+vy*(y-Lmax/2))/det;
			b=(x*vy-vx*(y-Lmax/2))/det;
			a=-a;
			p[nm_e].vx=a*x-b*(y-Lmax/2);
			p[nm_e].vy=a*(y-Lmax/2)+b*x;
			return a*a;}
		else {
			p[nm_e].vx*=-1;//On arrive ici dans le cas du carré
			return p[nm_e].vx*p[nm_e].vx;
		}
		case particle:// Pour le choc entre 2 particules, l'évolution des vitesses est moins évidente
			if(MESURE_TEMPS_VOL)
			{
				std::cout<<p[e[nm_e].ia].time<<"\t"<<p[e[nm_e].ia].time*pow(p[e[nm_e].ia].vx*p[e[nm_e].ia].vx+p[e[nm_e].ia].vy*p[e[nm_e].ia].vy,0.5)<<std::endl;
				p[e[nm_e].ia].time=0;
				std::cout<<p[e[nm_e].ib].time<<"\t"<<p[e[nm_e].ib].time*pow(p[e[nm_e].ib].vx*p[e[nm_e].ib].vx+p[e[nm_e].ib].vy*p[e[nm_e].ib].vy,0.5)<<std::endl;
				p[e[nm_e].ib].time=0;
			}
			x1=p[e[nm_e].ia].x; // On récupère toute les valeurs utiles
			y1=p[e[nm_e].ia].y;
			x2=p[e[nm_e].ib].x;
			y2=p[e[nm_e].ib].y;
			vx1=p[e[nm_e].ia].vx;
			vy1=p[e[nm_e].ia].vy;
			vx2=p[e[nm_e].ib].vx;
			vy2=p[e[nm_e].ib].vy;
			inter=((x2-x1)*(vx1-vx2)+(y2-y1)*(vy1-vy2))/((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));//Et on fait les calculs nécessaires
			//printf("inter = %f\n",inter);
			p[e[nm_e].ia].vx=vx1-inter*(x2-x1);
			p[e[nm_e].ia].vy=vy1-inter*(y2-y1);
			p[e[nm_e].ib].vx=vx2+inter*(x2-x1);
			p[e[nm_e].ib].vy=vy2+inter*(y2-y1);
			return 0;					
	}
}

int collision_mur(Particle *p,Event *e,int Np, double diameter, double Lmax)
{
	// Permet de trouver, pour chaque particule, le temps avant la prochaine collision, ainsi que le mur où se fera la collision.
	// Modifie le tableau d'événements e et retourne le numéro de l'événement le plus proche dans le temps
	int i,cas,e_min;
	double time1,time2,time3,time_min,vx,vy;
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
		if(STADE)// Dans le cas du stade, on doit rajouter 2 cas : la particule est dans l'arc de disque droit (cas 3) ou gauche (cas 4)
		{
			if(p[i].x>Lmax)
				cas=3;
			else if(p[i].x<0)
				cas=4;
		}
		switch(cas)/* Pour chaque cas, on regarde lequel des murs la particule rencontre en premier, 
							et on attribue alors le bon évenement et le bon temps. 
							Le switch permet de gérer efficacement les nombreux cas soulevés*/
		{
			case 0:// Pour les 4 premiers cas, la particule n'a le choix qu'entre 2 murs, avec des temps avant collision positifs
				time1=-(p[i].y-diameter/2)/vy;//Choc avec le mur du bas.
				time2=-(p[i].x-diameter/2)/vx;//choc avec le mur de gauche
				if(time1<time2)// On vérifie quel mur est rencontré en premier, et on crée le bon événement correspondant
				{
					e[i].time=time1;
					e[i].ia=i;
					e[i].type=bottom;
				}
				else
				{
					if(STADE)//Dans tous les cas où la particule doit rencontrer le mur de droite ou gauche, on change le calcul dans le cas du stade
						e[i].time=cercle(p[i],Lmax,diameter,4);
					else
						e[i].time=time2;
					e[i].ia=i;
					e[i].type=left;
				}
				break;
			case 1:
				time1=-(p[i].y-diameter/2)/vy;//Choc avec le mur du bas
				time2=(Lmax-p[i].x-diameter/2)/vx;//choc avec le mur de droite
				if(time1<time2)
				{
					e[i].time=time1;
					e[i].ia=i;
					e[i].type=bottom;
				}
				else
				{
					if(STADE)
						e[i].time=cercle(p[i],Lmax,diameter,3);
					else
						e[i].time=time2;
					e[i].ia=i;
					e[i].type=right;
				}
				break;
			case 10:
				time1=(Lmax-p[i].y-diameter/2)/vy;//choc avec le mur du haut
				time2=-(p[i].x-diameter/2)/vx;//Choc avec le mur de gauche
				if(time1<time2)
				{
					e[i].time=time1;
					e[i].ia=i;
					e[i].type=top;
				}
				else
				{
					if(STADE)
						e[i].time=cercle(p[i],Lmax,diameter,4);
					else
						e[i].time=time2;
					e[i].ia=i;
					e[i].type=left;
				}
				break;
			case 11:
				time1=(Lmax-p[i].y-diameter/2)/vy;//Choc avec le mur du haut
				time2=(Lmax-p[i].x-diameter/2)/vx;// choc avec le mur de droite
				if(time1<time2)
				{
					e[i].time=time1;
					e[i].ia=i;
					e[i].type=top;
				}
				else
				{
					if(STADE)
						e[i].time=cercle(p[i],Lmax,diameter,3);
					else
						e[i].time=time2;
					e[i].ia=i;
					e[i].type=right;
				}
				break;
			case 3:
			//Dans les cas où la particule se trouve dans un arc de disque, le calcul est plus compliqué : 
			//on doit trouver si la particule se cogne contre l'arc de cercle, et sinon vers quel autre mur elle va se cogner
			//IL n'est pas possible de passer dans ces cas lorsque STADE est mis à 0
				if(cercle(p[i],Lmax,diameter,cas)*vx+p[i].x>Lmax)//Si la boule se dirige bien vers le bord du cercle (choc contre la bonne partie du cercle)
																//C'est à dire si la position du choc contre le cercle se trouve à l'extérieur du carré
				{
					e[i].time=cercle(p[i],Lmax,diameter,cas);
					e[i].ia=i;
					e[i].type=right;
				}
				else// Sinon on regarde les autres cas
				{
					time1=((Lmax-p[i].y-diameter/2)/vy>0 ? (Lmax-p[i].y-diameter/2)/vy : 10e42);//temps avant collision sur mur du haut
					time2=(-(p[i].y-diameter/2)/vy>0 ? -(p[i].y-diameter/2)/vy : 10e42);//temps avant collisison sur mur du bas
					time3=(-(p[i].x-diameter/2)/vx>0 ? -(p[i].x-diameter/2)/vx : 10e42);//temps avant de rentrer dans l'autre arc de cercle 
																					//,qui correspondrait au temps avant choc contre le mur de gauche
					if(time1<time2 && time1<time3)// On initialise l'événement de la bonne façon en fonction des différents temps
					{
						e[i].time=time1;
						e[i].ia=i;
						e[i].type=top;
					}
					else if(time2<time3)
					{
						e[i].time=time2;
						e[i].ia=i;
						e[i].type=bottom;
					}
					else
					{
						e[i].time=cercle(p[i],Lmax,diameter,4);
						e[i].ia=i;
						e[i].type=left;
					}
				}
				break;
			case 4:
				if(cercle(p[i],Lmax,diameter,cas)*vx+p[i].x<0)//Si la boule se dirige bien vers le bord du cercle (choc contre la bonne partie du cercle)
				{
					e[i].time=cercle(p[i],Lmax,diameter,cas);
					e[i].ia=i;
					e[i].type=left;
				}
				else// Sinon on regarde les autres cas
				{
					time1=((Lmax-p[i].y-diameter/2)/vy>0 ? (Lmax-p[i].y-diameter/2)/vy : 10e42);//temps avant collision sur mur du haut
					time2=(-(p[i].y-diameter/2)/vy>0 ? -(p[i].y-diameter/2)/vy : 10e42);//temps avant collisison sur mur du bas
					time3=((Lmax-p[i].x-diameter/2)/vx>0 ? (Lmax-p[i].x-diameter/2)/vx : 10e42);//temps avant de rentrer dans l'autre arc de cercle
																							//,qui correspondrait au temps avant choc contre le mur de droite
					if(time1<time2 && time1<time3)
					{
						e[i].time=time1;
						e[i].ia=i;
						e[i].type=top;
					}
					else if(time2<time3)
					{
						e[i].time=time2;
						e[i].ia=i;
						e[i].type=bottom;
					}
					else
					{
						e[i].time=cercle(p[i],Lmax,diameter,3);
						e[i].ia=i;
						e[i].type=right;
					}
				}
				break;	
			}
		if(i==0)/* Au début de la boucle for , on doit initialiser le temps minimum à la première valeur, 
				afin de ne pas avoir de problèmes. En effet, si on initialise time_min à une valeur donnée, 
				il est toujours possible que le minimum effectif soit supérieur à cette valeur*/
		{
			e_min=0;
			time_min=e[0].time;
		}
		else if(e[i].time<time_min)// Si le rebond considéré arrive plus vite que le minimum précédent, on change le minimum et le numéro de l'événement qui sera retourné
		{
			e_min=i;
			time_min=e[i].time;
		}
	}
	return e_min;// On retourne l'emplacement dans le tableau de l'événement arrivant le plus vite
}
		
double cercle(Particle p, double Lmax, double diameter,int cas)
//Cette fonction prend les informations sur la particule ainsi que le cercle considérés
//Elle retourne le temps avant choc
{
	double x=p.x,y=p.y,vx=p.vx,vy=p.vy,a,b,c;
	if(cas==3)//Si on considère le choc de la particule avec le cercle de droite
	{
		a=vx*vx+vy*vy;// On calcule les coefficients de l'équation, qui est du second degré
		b=2*(vx*(x-Lmax)+vy*(y-Lmax/2));
		c=(x-Lmax)*(x-Lmax)+(y-Lmax/2)*(y-Lmax/2)-(Lmax-diameter)*(Lmax-diameter)/4;
	}
	else//Si on considère le choc de la particule avec le cercle de gauche
	{
		a=vx*vx+vy*vy;
		b=2*(vx*x+vy*(y-Lmax/2));
		c=x*x+(y-Lmax/2)*(y-Lmax/2)-(Lmax-diameter)*(Lmax-diameter)/4;
	}
	return (-b+sqrt(b*b-4*a*c))/(2*a);// On retourne la valeur du temps la plus élevée, qui correspond à un choc sur l'intérieur du cercle
}

	
			
