// We use the casio graphics library
// documentation: https://www.cairographics.org/manual/cairo-Paths.html
#include <unistd.h>
#include <assert.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define STADE 0// mettre STADE à 1 pour le stade et 0 pour un boite carré

typedef struct {
  double x,y,vx,vy;
} Particle; //this is our definition of  a particle, position plus speed

enum  col_type
{
 	bottom,
 	right,
	top,
  	left,
	//animation,
	particle
}; //different types of collision

typedef struct
{ // a structure describing each collision -- one might want an array of Events
	enum col_type type;
	int ia,ib;//ia numéro particule concernée, ib numéro particule rencontrée
 	double time;
} Event;

class Graphics{
 private:
  int Np;
  double Dim,diam;
  double *lmin, *lmax;
  cairo_surface_t *sfc;
  cairo_t *cr;
  Display *dsp;
  Drawable da;
 public:
  Graphics(int N, int Pix, double *dmin, double *dmax,double diameter){
    Np=N;
    assert( *dmin < *dmax );
    assert(Np >0) ;
    lmin=dmin;
    lmax=dmax;
    Dim=Pix;
    diam=diameter;

    if (( dsp = XOpenDisplay(NULL)) == NULL)      exit(1);//window management X11, and cairo graphics
    int screen = DefaultScreen(dsp);
    if(STADE)
   	da = XCreateSimpleWindow(dsp, DefaultRootWindow(dsp), 0, 0, 2*Dim, Dim, 0, 0, 0);
    else
   	da = XCreateSimpleWindow(dsp, DefaultRootWindow(dsp), 0, 0, Dim, Dim, 0, 0, 0);
    XMapWindow(dsp, da);
    if(STADE){
    sfc = cairo_xlib_surface_create(dsp, da, DefaultVisual(dsp, screen), 2*Dim , Dim);
    cairo_xlib_surface_set_size(sfc, 2*Dim, Dim);}
    else{
    sfc = cairo_xlib_surface_create(dsp, da, DefaultVisual(dsp, screen), Dim , Dim);
    cairo_xlib_surface_set_size(sfc, Dim, Dim);}
    
    cr = cairo_create (sfc);
  } //empty window now on screen

  //some error messages on trying to duplicate the window
  Graphics & operator=(Graphics &g){// stop the program when copying the window
    printf("Don't use = with graphics objects  %p \n", &g ); exit(1);
  }
  Graphics(const Graphics &g ){// stop the program when passing windows as argument
    printf("Don't pass graphics objects: use a pointer  %p \n" , &g); exit(2);
  }
  
  void draw(Particle*);//draw the particles
  void frame(double , double , double , double);//draw a square or stadium
  ~Graphics(){cairo_destroy (cr);cairo_surface_destroy (sfc); } //clean up function
};


// Prototypage des fonction
void update_pos(Particle *p, int Np,double tau, double Lmax, double diameter);
int collision_mur(Particle *p,Event *e,int Np, double diameter, double Lmax);
double update_vit(Event *e, int nm_e, Particle *p,double Lmax);
int collision_part(Particle *p,Event *e_col,int Np,double diameter);
double cercle(Particle p, double Lmax, double diameter,int cas);
double* allocdouble(int Np);
void creathist(Particle *p, int Np, double* normeV, double* compoVx, double* compoVy, int GO);
double temperature(Particle *p, int Nb, double m);
