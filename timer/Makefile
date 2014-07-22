
CXXFLAGS=
LDFLAGS=
PROJECT=libtimer.a
TEST=test

all: $(PROJECT)

$(PROJECT): timer.h timer.cpp
	$(CXX) -c timer.cpp $(CXXFLAGS)
	$(AR) rcs $(PROJECT) timer.o

$(TEST): test.cpp $(PROJECT)
	$(CXX) -o $(TEST) test.cpp -L. -ltimer -lpthread

clean:
	$(RM) $(PROJECT) timer.o $(TEST) test.o

