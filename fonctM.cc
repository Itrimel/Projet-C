#include "Graphics.h"
void update_pos(Particle *p,int Np,double tau)
{
	int i;
	for(i=0;i<Np;i++)
	{
		p[i].x=p[i].x+p[i].vx*tau;
		p[i].y=p[i].y+p[i].vy*tau;
	}
}
void update_vit(Event *e, int nm_e, Particle *p)
{
	switch(e[nm_e].type)
	{
		case bottom:
		case top:
			p[nm_e].vy*=-1;
			break;
		case left:
		case right:
			p[nm_e].vx*=-1;
			break;		
	}
}

int collision_mur(Particle *p,Event *e,int Np, double diameter, double Lmax)
{
	// Utilise la géométrie carrée ( entre 0 et Lmax) pour voir les chocs contre les murs
	int i,cas,e_min;
	double time1,time2,time_min,vx,vy;
	for(i=0;i<Np;i++)
	{
		cas=0;
		vx=p[i].vx;
		vy=p[i].vy;
		if(vx>0)
			cas++;
		if(vy>0)
			cas+=10;
		switch(cas)
		{
			case 0:
				time1=-(p[i].y-diameter/2)/vy;
				time2=-(p[i].x-diameter/2)/vx;
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
				time2=(Lmax-p[i].x-diameter/2)/vx;
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
				time1=(Lmax-p[i].y-diameter/2)/vy;
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
		if(i==0)
		{
			e_min=0;
			time_min=e[0].time;
		}
		else if(e[i].time<time_min)
		{
			e_min=i;
			time_min=e[i].time;
		}
	}
	return e_min;
}
		
		
			