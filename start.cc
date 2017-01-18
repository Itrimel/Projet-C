#include "Graphics.h"  // Contains the definition of "Particule" and "Graphics"

void initparticles( Particle *p, int np, double Lmin, double Lmax, double diameter)
{
	int i;
	for( i=0;i<np;i++)
	{ // drand48() -- random double numbers uniform on (0 1)
		p[i].x = Lmin +diameter/2 + (Lmax-Lmin-diameter)*drand48(); //random positions for intial condition
		p[i].y = Lmin +diameter/2 + (Lmax-Lmin-diameter)*drand48();
		p[i].vx = drand48();// choose random speeds too using drand48();
		p[i].vy = drand48();
  	}
}




int main()
{
	int i,Np=4,n_e_mur; //Number of particles
	double diameter=1;//particle size
	int Pix=800; //Number of pixels for window
	double Lmax=10, Lmin=0,temps=0,tau,temps_mur,delta=0.1,delta_temp; //Physical dimensions of box
	
	Graphics gw(Np,Pix, Lmin ,Lmax,diameter);// Open a window to plot particles in
	srand48(time(NULL));//inititalize random numbers -- to find always the same value // you can replace "1" by time(NULL) 
  
	Particle *p= (Particle *) malloc( Np *sizeof( Particle)); //an array of particles
	initparticles(p,Np,Lmin, Lmax,diameter); //place particles in box
	Event *e_mur = (Event *) malloc( Np* sizeof(Event) ); // Np possible collisions with walls
  
  	delta_temp=delta;
	for (int l=0; l<10000;l++)
	{//long loop
	//find future collision using exact calculations of collision times
	// e[0].type=right;
	// e[0].time = ...
	// find the very first collision in the future by looking at e[].time
	// move particles
		n_e_mur=collision_mur(p,e_mur,Np,diameter,Lmax);
		tau=e_mur[n_e_mur].time;
		if(delta_temp<tau)
		{
			update_pos(p,Np,delta_temp);
			gw.draw(p);
			temps+=delta_temp;
			delta_temp=delta;
		}
		else
		{
			update_pos(p,Np,tau);
			update_vit(e_mur,n_e_mur,p);
			temps+=tau;
			delta_temp-=tau;
		}
	}
	free(p);
	free(e_mur);
  return 0;
}




