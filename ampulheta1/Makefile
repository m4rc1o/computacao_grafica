APPLICATION=ampulheta1
CXXFLAGS = -Wall -I. -DVART_OGL -DIL_LIB -std=c++11
LDFLAGS = -L/usr/X11R6/lib
LDLIBS = -lGL -lglut -lGLU -lIL

OBJECTS = point4d.o color.o light.o texture.o material.o boundingbox.o memoryobj.o\
sgpath.o snlocator.o scenenode.o meshobject.o mesh.o transform.o camera.o arrow.o\
picknamelocator.o graphicobj.o joint.o  dof.o modifier.o bezier.o scene.o file.o mousecontrol.o\
viewerglutogl.o ampulheta1.o

# first, try to compile from this project
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

# second, try to compile from the core
%.o: vart/source/%.cpp vart/%.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

# last, try to compile from contribs
%.o: vart/contrib/source/%.cpp vart/contrib/%.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

$(APPLICATION): $(OBJECTS) ampulheta1.o
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

#gdb:
#	CXXFLAGS = $(CXXFLAGS) -ggdb
#	$(APPLICATION)

clean:
	rm -f *.o *~ $(APPLICATION)
