#include "Graphics.h"  // Contains the definition of "Particule" and "Graphics"

// pour choisir si on est en mode stade ou boite carré il faut aller dans Graphics.h

#define LYAPUNOV 1 // mettre à 1 pour calculer l'exposant de Lyapunov.
#define ECART 300 // mettre le décalage voulu pour le calcule de Lyapunov

#define PT 0 // mettre à 1 pour crée un fichier relevant pression et température, ne fonctionne pas si LYAPUNOV = 1
#define DETADIA 0// mettre à 1 pour voir la détente adiabatique, ne fonctionne pas avec Lyapu
#define HISTOGRAMME 0// mettre à 1 pour tracer l'histogramme des vitesse, ne fonctionne pas avec Lyapu 

#define L_MAX 1000 // nombre de pas temporelle que l'on réalise
#define N_PART 12 // nombre de particule
#define VERIF printf("OK\n");
#define Z printf("Z\n");
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

void initparticles_delta(Particle *p, Particle *p_delta, int np, double delta, double* epsi0) // on initialise tous les particules à un delta prés pour calculer l'exposant de Lyaponov.
{
	int i;
	p_delta[0].x = p[0].x*(1+delta/100);
	printf("%f \t %f\n", p_delta[0].x, p[0].x);
	//while(1);
	p_delta[0].y = p[0].y;
	p_delta[0].vx = p[0].vx;	
	p_delta[0].vy = p[0].vy;	
	*epsi0=fabs(p[0].x-p_delta[0].x);

	for(i=1;i<np;i++)
	{ // drand48() -- random double numbers uniform on (0 1)
		p_delta[i].x = p[i].x;//*(1+delta/100);
		p_delta[i].y = p[i].y;//*(1+delta/100);
		p_delta[i].vx = p[i].vx;//*(1+delta/100); 
		p_delta[i].vy = p[i].vy;//*(1+delta/100);
  	}
}

int main()
{
	if (HISTOGRAMME)
	{	
		system("rm HISTO/*.dat"); // on suprime les fichers précédament crée
		system("rm HISTO/*.jpeg");
		system("rm HISTO/*.gif");
	}
	int i,Np=N_PART,n_e_mur,n_e_col; //Number of particles
	double diameter=0.5;//particle size
	int Pix=500; //Number of pixels for window
	double Lmax=20, Lmin=0,temps=0,tau1,tau2,temps_mur,delta=1./8,delta_temp=delta; //Physical dimensions of box or stadium
	//Lmin DOIT RESTER A ZERO !!!!!
	int count=0, eq=0; // on compte le nombre d'affichage et le nombre de collision entre particule pour savoir si on est à l'équlibre
	double T=0, P=0, V=0; //variable qui mesure les constantes physiques

	Graphics gw1(Np,Pix, &Lmin ,&Lmax,diameter);// Open a window to plot particles in
	Graphics gw2(Np,Pix, &Lmin ,&Lmax,diameter);// Open a window to plot particles in
	srand48(1);//inititalize random numbers -- to find always the same value // you can replace "1" by time(NULL) 
  	
	double m = 3.14159*diameter*diameter; // on prend pour la masse la surface de la particule

	double *normeV=allocdouble(Np); // on récupère trois tableau initialisé à zero pour tracer l'histograme des vitesse
	double *compoVx=allocdouble(Np);
	double *compoVy=allocdouble(Np);
	int GO=0;
	
	int l=0; // l gère le nombre de pas temporel que l'on fait

	double  tau1_delta, tau2_delta, delta_lyapu=ECART;// valeur en % d'écart lorsqu'on veux calculer l'exposant de Lyapunov
	double t_normal, t_delta, shift_t_normal=0, shift_t_delta=0;
	int n_e_mur_delta, n_e_col_delta;
	double epsi0, epsn, epsnmoins, poubelle; //écart
	
	FILE *fp=NULL;
	Particle *p= (Particle *) malloc(Np *sizeof(Particle)); //an array of particles
	Event *e_mur = (Event *) malloc(Np* sizeof(Event) );
	/*Ce tableau contiendra les événements collision avec mur, qui sont au nombre de Np, car ils sont sélectionnés pour chaque particule afin de n'avoir que le suivant dans le temps*/
	Event *e_col = (Event *) malloc(Np*Np*sizeof(Event)); /*Ce tableau contiendra les événements collision entre particules, qui sont au nombre de Np(Np-1)/2 On prend un peu plus par sécurité*/
	
	Particle *p_delta = NULL;
	Event *e_mur_delta = NULL;
	Event *e_col_delta = NULL;

	if(e_mur==NULL || e_col==NULL || p==NULL)//Sécurité
	{
		printf("Allocation mémoire échouée. Fin du programme\n");
		return (-2);
	}
	initparticles(p,Np,Lmin, Lmax,diameter); //place particles in box

	if(PT)
	{
		fp=fopen("pression_temperature","w");
		if (fp == NULL)
		{
			printf("creation/acces fichier echouer\n");
			return (-1);
		}
	}
	if(LYAPUNOV) // on duplique les variables précédente mais avec la première  décaler de x%
	{
		fp=fopen("LYAPUNOV", "w");
		p_delta= (Particle *) malloc(Np *sizeof(Particle)); //an array of particles
		e_mur_delta = (Event *) malloc(Np* sizeof(Event) );
		e_col_delta = (Event *) malloc(Np*Np*sizeof(Event));
		
		if(e_mur_delta==NULL || e_col_delta==NULL || p_delta==NULL ||fp == NULL)//Sécurité
		{
			printf("Allocation mémoire échouée. Fin du programme\n");
				return 0;
		}
		
		initparticles_delta(p, p_delta, Np, delta_lyapu, &epsi0); //place particles in box
		epsn=epsi0;
		fprintf(fp, "%f\n",epsn);
	}
	while(l<L_MAX)
	{
		//printf("%d\n", l);
		if(LYAPUNOV)
		{
			n_e_col=collision_part(p,e_col,Np,diameter);//On modifie les tableaux des événements physiques, des collisions et celui des rebonds, on recupère dans n_e le numéro de l'envoi le plus proche
			n_e_mur=collision_mur(p,e_mur,Np,diameter,Lmax);
			tau1=e_col[n_e_col].time;// On récupère les temps des événements physiques
			tau2=e_mur[n_e_mur].time;

			//parti delta
			n_e_col_delta=collision_part(p_delta,e_col_delta,Np,diameter);
			n_e_mur_delta=collision_mur(p_delta,e_mur_delta,Np,diameter,Lmax);
			tau1_delta=e_col_delta[n_e_col_delta].time;// On récupère les temps des événements physiques
			tau2_delta=e_mur_delta[n_e_mur_delta].time;

			if (tau1 <tau2)
			{
				if(tau1<= 1e-14)
					t_normal=tau2;
				else
					t_normal=tau1;
			}
			if(tau2 <= tau1)
			{
				if(tau2<= 1e-14)
					t_normal=tau1;
				else
					t_normal=tau2;
			}
	
			if (tau1_delta <tau2_delta)
			{
				if(tau1_delta<= 1e-14)
					t_delta=tau2_delta;
				else
					t_delta=tau1_delta;
			}
			if(tau2_delta <= tau1_delta)
			{
				if(tau2_delta<= 1e-14)
					t_delta=tau1_delta;
				else
					t_delta=tau2_delta;
			}
			
			if(delta_temp<t_normal && delta_temp<t_delta)//Si la prochaine étape d'animation est plus proche que les autres événements
			{
				update_pos(p,Np,(delta_temp+shift_t_normal), Lmax, diameter);//On met à jour la position de toutes les particules et on affiche
				update_pos(p_delta,Np,(delta_temp+shift_t_delta),Lmax, diameter);//On met à jour la position de toutes les particules et on affiche
				
				epsnmoins=epsn;
				epsn=fabs(p_delta[0].x-p[0].x);
				if(epsn-epsnmoins < 1e-14)
					//while(1);
				
				gw2.draw(p_delta);
				gw1.draw(p);
				temps+=delta_temp;
				delta_temp=delta;
				shift_t_delta=0;
				shift_t_normal=0;
				l++;
				fprintf(fp, "%f\n",epsn);

			}	
			else if(t_normal<t_delta) // on réalise l'évenement le plus cour
			{
				if(tau1<tau2) // Si la collision entre particules est plus proche que le rebond sur mur
				{
					update_pos(p,Np,tau1+shift_t_normal,Lmax, diameter);
					update_vit(e_col,n_e_col,p,Lmax);//On met à jour la vitesse des 2 particules
					temps+=tau1;
					delta_temp-=tau1;/* On diminue le temps jusqu'à la prochaine animation de tau1. 
					A la prochaine boucle, les temps des événements physiques seront donc comparés à delta - tau*/
					shift_t_delta+=tau1;
					shift_t_normal=0;	
					eq++;
				}
				else //Le prochain événement est nécessairement un rebond à ce point
				{
					update_pos(p,Np,tau2+shift_t_normal, Lmax, diameter);
					update_vit(e_mur,n_e_mur,p,Lmax);
					temps+=tau2;
					delta_temp-=tau2;
					shift_t_delta+=tau2;
					shift_t_normal=0;
				}
			}
			else
			{
				if(tau1_delta<tau2_delta) // Si la collision entre particules est plus proche que le rebond sur mur
				{
					update_pos(p_delta,Np,tau1_delta+shift_t_delta, Lmax, diameter);
					update_vit(e_col_delta,n_e_col_delta,p_delta,Lmax);//On met à jour la vitesse des 2 particules
					temps+=tau1_delta;
					delta_temp-=tau1_delta;
					shift_t_delta=0;
					shift_t_normal+=tau1_delta;
				}
				else //Le prochain événement est nécessairement un rebond à ce point
				{
					update_pos(p_delta,Np,tau2_delta+shift_t_delta, Lmax, diameter);
					update_vit(e_mur_delta,n_e_mur_delta,p_delta,Lmax);
					temps+=tau2_delta;
					delta_temp-=tau2_delta;
					shift_t_delta=0;
					shift_t_normal+=tau2_delta;
				}	
			}
		}
		else // cas ou on ne calcule pas l'exposant
		{
			n_e_col=collision_part(p,e_col,Np,diameter);//On modifie les tableaux des événements physiques, des collisions et celui des rebonds, on recupère dans n_e le numéro de l'envet le plus proche
			n_e_mur=collision_mur(p,e_mur,Np,diameter,Lmax);
			tau1=e_col[n_e_col].time;// On récupère les temps des événements physiques
			tau2=e_mur[n_e_mur].time;
			
			if(delta_temp<tau2 && delta_temp<tau1)//Si la prochaine étape d'animation est plus proche que les 2 autres événements
			{
				update_pos(p,Np,delta_temp, Lmax, diameter);//On met à jour la position de toutes les particules et on affiche
				gw1.draw(p);
				temps+=delta_temp;
				delta_temp=delta;
				if(PT)
					T=T+temperature(p, Np, m);
				
				if(l%30==0) // affiche tous les 30 L      /////////////// ICI pour gérer le moyennage de mesure
				{ // on affiche pression et température moyennée sur un certain nombre de  delta t
					if(PT)
					{
						fprintf(fp,"%f %f\n", T, V/(2*m*30*delta));
						T=0;
						V=0;
					}
					if (DETADIA)
						Lmax*=1.001; // on augmente la taille de la boite
				}
				if(HISTOGRAMME)
				{
					if(l==(L_MAX-1)) // on sauvegarde les données pour l'histrograme des vitesses;
						GO=1;
					if(eq>Np) // on se place à l'équilibre thermo
						creathist(p, Np, normeV, compoVx, compoVy, GO);
				}
				l++;
			}	
			else if(tau1<tau2) // Si la collision entre particules est plus proche que le rebond sur mur
			{
				update_pos(p,Np,tau1, Lmax, diameter);
				update_vit(e_col,n_e_col,p,Lmax);//On met à jour la vitesse des 2 particules
				temps+=tau1;
				delta_temp-=tau1;/* On diminue le temps jusqu'à la prochaine animation de tau1. 
				A la prochaine boucle, les temps des événements physiques seront donc comparés à delta - tau*/
				eq++;
			}
			else //Le prochain événement est nécessairement un rebond à ce point
			{
				update_pos(p,Np,tau2, Lmax, diameter);
				V=V+update_vit(e_mur,n_e_mur,p,Lmax);
				temps+=tau2;
				delta_temp-=tau2;
			}
		}
	}
	free(p);// On libère les tableaux
	free(e_mur);
	free(e_col);
	if(LYAPUNOV)
	{
		free(p_delta);
		free(e_mur_delta);
		free(e_col_delta);
	}
	return 0;
}
