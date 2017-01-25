NPART= -D N_PART=100
LMAX = -D L_MAX=10000
DIA = -D DIAMETER=0.5
CXXFLAGS= -O -g -Wextra -Wall -Wshadow  -Wno-unused-variable -I/opt/local/include -I/opt/X11/include $(LMAX) $(NPART) $(DIA)

all : simu_simple stade mesure_vol mesure_pt histogramme detente detente_stade lyapunov

#STADE=1 permet de choisir le stade
#MESURE_TEMPS_VOL=1 permet de retourner dans le terminal les temps de vol libre des particules
#LYAPUNOV=1 permet de mesurer l'exposant de Lyapunov du sytème considéré
#PT=1 permet de retourner dans un fichier les valeurs de pression et température
#DETADIA=1 permet de mettre en place la détente adiabatique
#HISTOGRAMME=1 permet de mesurer les vitesse sous forme d'histogramme

#Simulation sans aucun ajout
INIT = start_0.o graphics_0.o fonctM_0.o fonctC_0.o

simu_simple: $(INIT)
	$(CXX) $(INIT) -L/opt/local/lib -g -L/opt/X11/lib -lcairo  -lX11 -o $@

%_0.o : %.cc Graphics.h
	$(CXX) -c $*.cc -o $*_0.o $(CXXFLAGS) -D STADE=0 -D MESURE_TEMPS_VOL=0 -D LYAPUNOV=0  -D PT=0 -D DETADIA=0 -D HISTOGRAMME=0

#Faire le stade
STADIUM = start_stade.o graphics_stade.o fonctM_stade.o fonctC_stade.o

stade : $(STADIUM)
	$(CXX)  $(STADIUM)  -L/opt/X11/lib -lcairo  -lX11 -o $@

%_stade.o : %.cc Graphics.h
	$(CXX) -c $*.cc -o $*_stade.o $(CXXFLAGS)  -D STADE=1 -D MESURE_TEMPS_VOL=0 -D LYAPUNOV=0  -D PT=0 -D DETADIA=0 -D HISTOGRAMME=0

#Faire la mesure des temps de vol
VOL = start_vol.o graphics_vol.o fonctM_vol.o fonctC_vol.o

mesure_vol : $(VOL)
	$(CXX) $(VOL)  -L/opt/X11/lib -lcairo  -lX11 -o $@

%_vol.o : %.cc Graphics.h
	$(CXX) -c $*.cc -o $*_vol.o $(CXXFLAGS) -D STADE=0 -D MESURE_TEMPS_VOL=1 -D LYAPUNOV=0  -D PT=0 -D DETADIA=0 -D HISTOGRAMME=0
	
#Faire la mesure de la pression et de la température
PR_TEMP = start_pt.o graphics_pt.o fonctM_pt.o fonctC_pt.o

mesure_pt : $(PR_TEMP)
	$(CXX) $(PR_TEMP) -L/opt/X11/lib -lcairo  -lX11 -o $@

%_pt.o : %.cc Graphics.h
	$(CXX) -c $*.cc -o $*_pt.o $(CXXFLAGS) -D STADE=0 -D MESURE_TEMPS_VOL=0 -D LYAPUNOV=0  -D PT=1 -D DETADIA=0 -D HISTOGRAMME=0	
	
#Faire la mesure des vitesses sous forme d'histogramme
HISTO = start_hist.o graphics_hist.o fonctM_hist.o fonctC_hist.o

histogramme : $(HISTO)
	$(CXX) $(HISTO)  -L/opt/X11/lib -lcairo  -lX11 -o $@

%_hist.o : %.cc
	$(CXX) -c $*.cc -o $*_hist.o $(CXXFLAGS) -D STADE=0 -D MESURE_TEMPS_VOL=1 -D LYAPUNOV=0  -D PT=0 -D DETADIA=0 -D HISTOGRAMME=1
	
#Activer la détente adiabatique : on pourra chercher à mesurer la pression en même temps en mettant PT à 1
DET = start_det.o graphics_det.o fonctM_det.o fonctC_det.o

detente : $(DET)
	$(CXX) $(DET) -L/opt/X11/lib -lcairo  -lX11 -o $@

%_det.o : %.cc Graphics.h
	$(CXX) -c $*.cc -o $*_det.o $(CXXFLAGS) -D STADE=0 -D MESURE_TEMPS_VOL=0 -D LYAPUNOV=0  -D PT=0 -D DETADIA=1 -D HISTOGRAMME=0

#Mesure de l'exposant de Lyapunov
LYAP = start_lya.o graphics_lya.o fonctM_lya.o fonctC_lya.o

lyapunov: $(LYAP)
	$(CXX) $(LYAP) -L/opt/local/lib -g -L/opt/X11/lib -lcairo  -lX11 -o $@

%_lya.o : %.cc Graphics.h
	$(CXX) -c $*.cc -o $*_lya.o $(CXXFLAGS) -D STADE=0 -D MESURE_TEMPS_VOL=0 -D LYAPUNOV=1  -D PT=0 -D DETADIA=0 -D HISTOGRAMME=0


#Compilation personnalisée : ici on va part exemple activer dans les flags -D le stade, la détente adiabatique et la mesure de la température et de la pression
PERS = start_pers.o graphics_pers.o fonctM_pers.o fonctC_pers.o

detente_stade : $(PERS)
	$(CXX) $(PERS) -L/opt/X11/lib -lcairo  -lX11 -o $@

%_pers.o : %.cc Graphics.h
	$(CXX) -c $*.cc -o $*_pers.o $(CXXFLAGS) -D STADE=1 -D MESURE_TEMPS_VOL=0 -D LYAPUNOV=0  -D PT=1 -D DETADIA=1 -D HISTOGRAMME=0
	
	
	