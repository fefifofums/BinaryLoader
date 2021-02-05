CXX=g++
CXXFLAGS=-Wall
LIBS=-lbfd
DEPS=loader.h
OBJS=loader.o

TEST=test

.cc.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LIBS)

test: $(OBJS)
	$(CXX) $(CXXFLAGS) test.cc -o $@ $^ $(LIBS)

clean:
	@rm -f $(OBJS) $(TEST)
