# installation targets
INSTDIR=/usr/local
INSTLIBDIR=$(INSTDIR)/lib
INSTINCDIR=$(INSTDIR)/include
DOCDIR=./html/unit++
#various compilers.
CXX=g++
CXXFLAGS=-Wall
# for those that have doc++, otherwise get it
DOC=doc++

# Below here should not need modification
TARGETLIB=libunit++.a
TARGETHEADERS=unit++.h optmap.h
OBJS=unit++.o tester.o main.o optmap.o
#the files with doc information
DOCSRC=$(TARGETHEADERS) main.h tester.h
DOCOPTS=-d $(DOCDIR)
TSTOBJS=Test_unit++.o Test_optmap.o
TSTLIBS=-L. -lunit++

all: $(TARGETLIB) test++

$(TARGETLIB): $(OBJS)
	ar r $@ $(OBJS)

install:$(TARGETLIB) unit++.h
	[ -d $(INSTLIBDIR) ] || mkdir -p $(INSTLIBDIR)
	[ -d $(INSTINCDIR) ] || mkdir -p $(INSTINCDIR)
	cp $(TARGETLIB) $(INSTLIBDIR)
	cp $(TARGETHEADERS) $(INSTINCDIR)

doc: $(DOCSRC)
	[ -d $(DOCDIR) ] || mkdir -p $(DOCDIR)
	doc++ $(DOCOPTS) $(DOCSRC)

test++: $(TSTOBJS) $(TARGETLIB)
	$(CXX) -o $@ $(TSTOBJS) $(TSTLIBS)

clean:
	rm -f $(OBJS) $(TSTOBJS)

clobber: clean
	rm -f $(TARGETLIB) test++

Test_optmap.o: Test_optmap.cc unit++.h tester.h optmap.h
Test_unit++.o: Test_unit++.cc unit++.h tester.h
main.o: main.cc main.h tester.h unit++.h optmap.h
optmap.o: optmap.cc optmap.h
tester.o: tester.cc tester.h unit++.h
unit++.o: unit++.cc unit++.h
