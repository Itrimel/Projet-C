//https:www.cairographics.org/manual/cairo-Paths.html

#include "Graphics.h"

void
Graphics::draw(Particle *p){
  double v;  
  const int FPS=50;//frames per second
  struct timespec tm={0,1000000000/FPS}; //Sleep in nanoseconds between frames
  nanosleep( &tm , NULL);
  XEvent event;//check if window closed and finish
  if( XCheckWindowEvent(dsp,da, DestroyNotify , &event)){ XCloseDisplay(dsp); exit(1);}

  double gamma = Dim/(lmax-lmin + diam);//scaling between physical units and pixels
  double alpha= gamma*diam/2.;
  
  cairo_push_group(cr); //start drawing
  cairo_set_source_rgb(cr, 0.0, 0.19, 0);//dark green background
  cairo_paint (cr); //clear screen with green

  cairo_set_source_rgb(cr, 0.5, 0.5, 1);//dark red for particles
  for(int i=0;i<Np;i++){// place the particles in the graphics buffer, without drawing
  	v=(double)i/Np;
    cairo_new_sub_path(cr) ;
    cairo_arc(cr,  alpha + gamma* (p[i].x -lmin) ,  alpha + gamma*(p[i].y - lmin), alpha, 0, 2 * M_PI);
  }
  cairo_fill(cr);//draw all particles with solid color

  /*cairo_set_source_rgb(cr, 0, 0.19, 0.69);//dark blue for particles
  for(int i=0;i<Np;i++){
    cairo_new_sub_path(cr) ;
    cairo_arc(cr,  alpha + gamma* (p[i].x -lmin) ,  alpha + gamma*(p[i].y - lmin), 1.4*alpha, 0, 2 * M_PI);
  }
  cairo_stroke (cr); // hollow particles*/

  
  frame(alpha, Dim-alpha, alpha, Dim-alpha);//draw square border
  cairo_pop_group_to_source(cr); //finished drawing operations for this set of positions
  cairo_paint(cr);//send to screen
  
}

void Graphics::frame(double xmin, double xmax, double ymin, double ymax){
  cairo_set_source_rgb (cr, 1, 1, 0);//yellow border
  cairo_rectangle (cr,xmin, ymin, xmax-xmin, ymax-ymin);
  cairo_stroke (cr);
}
