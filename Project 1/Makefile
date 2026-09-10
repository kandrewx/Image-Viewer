

OFILES = \
	 base/StarterViewer.o

ROOTDIR = .
LIB = $(ROOTDIR)/lib/libstarter.a

GLLDFLAGS     = -framework glut -framework OpenGL -lm -lOpenImageIO

CXX = g++-16 -Wall -g -O2 -fPIC $(DEFINES) -fopenmp -std=c++17

INCLUDES =  -I ./include/ -I /usr/local/include/ -I/usr/include/ -I/opt/homebrew/include -L/opt/homebrew/lib


.C.o:
	$(CXX) -c $(INCLUDES) $< -o $@

base: $(OFILES)
	ar rv $(LIB) $?
	$(CXX) base/simple_viewer.C $(INCLUDES)  -L./lib -lstarter $(GLLDFLAGS)  -o bin/imgviewer

clean:
	rm -rf bin/imgviewer *.o base/*.o base/*~ include/*~ $(LIB)  *~



