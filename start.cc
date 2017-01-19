#include "Graphics.h"  // Contains the definition of "Particule" and "Graphics"

void initparticles( Particle *p, int np, double Lmin, double Lmax, double diameter)
{
	int i;
	for( i=0;i<np;i++)
	{ // drand48() -- random double numbers uniform on (0 1)
		p[i].x = Lmin +diameter/2 + (Lmax-Lmin-diameter)*drand48(); //random positions for intial condition
		p[i].y = Lmin +diameter/2 + (Lmax-Lmin-diameter)*drand48();
		p[i].vx = 2*drand48()-1;// choose random speeds too using drand48();
		p[i].vy = 2*drand48()-1;
  	}
}




int main()
{


	int i,Np=300,n_e_mur,n_e_col; //Number of particles
	double diameter=0.1;//particle size
	int Pix=800; //Number of pixels for window
	double Lmax=10, Lmin=0,temps=0,tau1,tau2,temps_mur,delta=0.3,delta_temp=delta; //Physical dimensions of box

	Graphics gw(Np,Pix, Lmin ,Lmax,diameter);// Open a window to plot particles in
	srand48(time(NULL));//inititalize random numbers -- to find always the same value // you can replace "1" by time(NULL) 
  	printf("Coucou !\n");
	Particle *p= (Particle *) malloc( Np *sizeof( Particle)); //an array of particles
	initparticles(p,Np,Lmin, Lmax,diameter); //place particles in box
	Event *e_mur = (Event *) malloc( Np* sizeof(Event) );
	/*Ce tableau contiendra les événements collision avec mur, qui sont au nombre de Np, 
	car ils sont sélectionnés pour chaque particule afin de n'avoir que le suivant dans le temps*/
	Event *e_col = (Event *) malloc( Np*Np*sizeof(Event));
	/*Ce tableau contiendra les événements collision entre particules, qui sont au nombre de Np(Np-1)/2
	On prend un peu plus par sécurité*/
	if(e_mur==NULL || e_col==NULL || p ==NULL)//Sécurité
	{
		printf("Allocation mémoire échouée. Fin du programme\n");
		free(p);
		free(e_mur);
		free(e_col);
		return 0;
	}
	//for (int l=0; l<10000;l++)
	while(1)
	{

		n_e_col=collision_part(p,e_col,Np,diameter);//On modifie les tableaux des événements physiques, celui des collisions et celui des rebonds, on recupère dans n_e le numéro de l'envet le plus proche
		n_e_mur=collision_mur(p,e_mur,Np,diameter,Lmax);
		tau1=e_col[n_e_col].time;// On récupère les temps des événements physiques
		//printf("%f\t",tau1);
		tau2=e_mur[n_e_mur].time;
		//printf("%f\n",tau2);
		if(delta_temp<tau2 && delta_temp<tau1)//Si la prochaine étape d'animation est plus proche que les 2 autres événements
		{
			update_pos(p,Np,delta_temp);//On met à jour la position de toutes les particules et on affiche
			gw.draw(p);
			temps+=delta_temp;
			delta_temp=delta;
		}	
		else if(tau1<tau2) // Si la collision entre particules est plus proche que le rebond sur mur
		{
			update_pos(p,Np,tau1);
			update_vit(e_col,n_e_col,p);//On met à jour la vitesse des 2 particules
			temps+=tau1;
			delta_temp-=tau1;/* On diminue le temps jusqu'à la prochaine animation de tau1. 
			A la prochaine boucle, les temps des événements physiques seront donc comparés à delta - tau*/
		}
		else //Le prochain événement est nécessairement un rebond à ce point
		{
			update_pos(p,Np,tau2);
			update_vit(e_mur,n_e_mur,p);
			temps+=tau2;
			delta_temp-=tau2;
		}
	}
	free(p);// On libère les tableaux
	free(e_mur);
	free(e_col);
  return 0;
}




