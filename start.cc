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
	system("rm HISTO/*.dat"); // on suprime les fichers précédament crée
	system("rm HISTO/*.jpeg");
	system("rm HISTO/*.gif");
	system("rm graph");

	int i,Np=900,n_e_mur,n_e_col; //Number of particles
	double diameter=0.5;//particle size
	int Pix=500; //Number of pixels for window
	double Lmax=20, Lmin=0,temps=0,tau1,tau2,temps_mur,delta=0.3,delta_temp=delta; //Physical dimensions of box or stadium
	//Lmin DOIT RESTER A ZERO !!!!!
	int count=0; // on compte le nombre d'affichage
	double T=0, P=0, V=0; //variable qui mesure les constantes physiques

	//Graphics gw(Np,Pix, &Lmin ,&Lmax,diameter);// Open a window to plot particles in
	srand48(time(NULL));//inititalize random numbers -- to find always the same value // you can replace "1" by time(NULL) 
  	double m = 3.14159*diameter*diameter; // on prend pour la masse la surface de la particule
	double *normeV=allocdouble(Np); // on récupère trois tableau initialisé à zero pour tracer l'histograme des vitesse
	double *compoVx=allocdouble(Np);
	double *compoVy=allocdouble(Np);
	int GO;

	int l=0; // l gère le nombre de pas temporel que l'on fait


	//printf("Pression\tTemperature !\n");
	
	FILE *fp=NULL;
	Particle *p= (Particle *) malloc( Np *sizeof( Particle)); //an array of particles
	initparticles(p,Np,Lmin, Lmax,diameter); //place particles in box
	Event *e_mur = (Event *) malloc( Np* sizeof(Event) );
	/*Ce tableau contiendra les événements collision avec mur, qui sont au nombre de Np, 
	car ils sont sélectionnés pour chaque particule afin de n'avoir que le suivant dans le temps*/
	Event *e_col = (Event *) malloc( Np*Np*sizeof(Event));
	/*Ce tableau contiendra les événements collision entre particules, qui sont au nombre de Np(Np-1)/2
	On prend un peu plus par sécurité*/
	fp=fopen("graph","w");
	if(e_mur==NULL || e_col==NULL || p ==NULL || fp==NULL)//Sécurité
	{
		printf("Allocation mémoire échouée. Fin du programme\n");
		free(p);
		free(e_mur);
		free(e_col);
		return 0;
	}

	while(l<10000)
	{
		n_e_col=collision_part(p,e_col,Np,diameter);//On modifie les tableaux des événements physiques, celui des collisions et celui des rebonds, on recupère dans n_e le numéro de l'envet le plus proche
		n_e_mur=collision_mur(p,e_mur,Np,diameter,Lmax);
		tau1=e_col[n_e_col].time;// On récupère les temps des événements physiques
		tau2=e_mur[n_e_mur].time;
		
		if(delta_temp<tau2 && delta_temp<tau1)//Si la prochaine étape d'animation est plus proche que les 2 autres événements
		{
			GO=0;
			update_pos(p,Np,delta_temp);//On met à jour la position de toutes les particules et on affiche
			//gw.draw(p);
			temps+=delta_temp;
			delta_temp=delta;
			//T=T+temperature(p, Np, m);
			//P = V/(2*m*delta); // la pression est donné par P=V_perp.
			if(l%30==0)
			{ // on afficher pression et température moyenner sur 10 delta t
				fprintf(fp,"%f\n",V/(2*m*30*delta));
				//P=0;
				//T=0;
				V=0;
				Lmax*=1.001;
			}
			/*//printf("GO= %d, l=%d\n", GO, l);
			if(l==999) // on sauvegarde les données;
				GO=1;
			//if() // on se place à l'équilibre thermo
			
			//creathist(p, Np, normeV, compoVx, compoVy, GO);
			// on reset la valeur de vitesse*/
			l++;
		}	
		else if(tau1<tau2) // Si la collision entre particules est plus proche que le rebond sur mur
		{
			update_pos(p,Np,tau1);
			update_vit(e_col,n_e_col,p,Lmax);//On met à jour la vitesse des 2 particules
			temps+=tau1;
			delta_temp-=tau1;/* On diminue le temps jusqu'à la prochaine animation de tau1. 
			A la prochaine boucle, les temps des événements physiques seront donc comparés à delta - tau*/
		}
		else //Le prochain événement est nécessairement un rebond à ce point
		{
			update_pos(p,Np,tau2);
			V=V+update_vit(e_mur,n_e_mur,p,Lmax);
			temps+=tau2;
			delta_temp-=tau2;
		}
	}
	free(p);// On libère les tableaux
	free(e_mur);
	free(e_col);
  return 0;
}
