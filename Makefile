OBJS=unit++.o tester.o main.o asserter.o optmap.o
LIB=libunit++.a
DOCSRC=unit++.h optmap.h asserter.h main.h tester.h
DOCOPTS=-d ~/html
TSTOBJS=Test_unit++.o Test_optmap.o
TSTLIBS=-L. -lunit++

CXXFLAGS=-Wall

all: $(LIB) doc test++

$(LIB): $(OBJS)
	ar r $@ $(OBJS)

doc: $(DOCSRC)
	doc++ $(DOCOPTS) $(DOCSRC)

test++: $(TSTOBJS) $(LIB)
	$(CXX) -o $@ $(TSTOBJS) $(TSTLIBS)

clean:
	rm -f $(OBJS) $(TSTOBJS)

clobber: clean
	rm -f $(LIB) test++

Test_optmap.o: Test_optmap.cc unit++.h asserter.h tester.h optmap.h
Test_unit++.o: Test_unit++.cc unit++.h asserter.h tester.h
asserter.o: asserter.cc asserter.h
main.o: main.cc main.h tester.h unit++.h asserter.h optmap.h
optmap.o: optmap.cc optmap.h
tester.o: tester.cc tester.h unit++.h asserter.h
unit++.o: unit++.cc unit++.h asserter.h
