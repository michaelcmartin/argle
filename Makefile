CXX = g++
CXXFLAGS = 

LD = g++
LDFLAGS = 

CXXOBJS = main.o module_map.o scan.o
OBJS = $(CXXOBJS)

argle: $(CXXOBJS)
	$(LD) $(LDFLAGS) -o argle $(OBJS)

clean:
	rm -f argle *.o

depend:
	find . -name \*.c\* | xargs makedepend -Y.

$(CXXOBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


# Makedepend results below
# DO NOT DELETE

./scan.o: scan.h
./module_map.o: module_map.h
./main.o: module_map.h scan.h
