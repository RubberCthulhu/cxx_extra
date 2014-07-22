
CXXFLAGS=
LDFLAGS=
PROJECT=libtime_helper.a
TEST=test

all: $(PROJECT)

$(PROJECT): time_helper.h time_helper.cpp
	$(CXX) -c time_helper.cpp
	$(AR) rcs $(PROJECT) time_helper.o

$(TEST): test.cpp $(PROJECT)
	$(CXX) -o $(TEST) test.cpp -L. -ltime_helper

clean:
	$(RM) $(PROJECT) time_helper.o $(TEST) test.o

