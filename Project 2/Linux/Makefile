

OFILES = \
	 base/StarterViewer.o \
	 base/ImgProc.o \
	 base/simple_viewer.o

ROOTDIR = .
LIB = $(ROOTDIR)/lib/libstarter.a 

GLLDFLAGS     = -lglut -lGL -lm -lGLU -lOpenImageIO -lOpenImageIO_Util

CXX = g++ -Wall -g -O2 -fPIC $(DEFINES) -fopenmp -std=c++14

INCLUDES =  -I ./include/ -I /usr/local/include/ -I/usr/include/ 


.C.o:
	$(CXX) -c $(INCLUDES) $< -o $@
	
.cpp.o:
	$(CXX) -c $(INCLUDES) $< -o $@

base: $(OFILES)
	ar rv $(LIB) $?
	$(CXX) base/simple_viewer.c $(INCLUDES)  -L./lib -lstarter $(GLLDFLAGS)  -o bin/imgviewer

clean:
	rm -rf bin/imgviewer *.o base/*.o base/*~ include/*~ $(LIB)  *~



