
CXXFLAGS=
LDFLAGS=
PROJECT=libjob_queue.a
TEST=test

all: $(PROJECT)

$(PROJECT): job_queue.h job_queue.cpp
	$(CXX) -c job_queue.cpp $(CXXFLAGS)
	$(AR) rcs $(PROJECT) job_queue.o

$(TEST): test.cpp $(PROJECT)
	$(CXX) -o $(TEST) test.cpp -L. -ljob_queue -lpthread

clean:
	$(RM) $(PROJECT) job_queue.o $(TEST) test.o

