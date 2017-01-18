#include "Graphics.h"
void update_pos(Particle *p,int Np)
{
	int i;
	for(i=0;i<Np;i++)
	{
		p[i].x=p[i].x+p[i].vx;
		p[i].y=p[i].y+p[i].vy;
	}
}