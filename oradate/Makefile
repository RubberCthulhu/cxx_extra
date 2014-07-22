
CXXFLAGS=
LDFLAGS=
PROJECT=liboradate.a
TEST=test
ORACLE_INC=/usr/include/oracle/11.2/client64
ORACLE_LIB=/usr/lib/oracle/11.2/client64/lib

all: $(PROJECT)

$(PROJECT): oradate.h oradate.cpp
	$(CXX) -c oradate.cpp $(CXXFLAGS) -I$(ORACLE_INC)
	$(AR) rcs $(PROJECT) oradate.o

$(TEST): test.cpp $(PROJECT)
	$(CXX) -o $(TEST) test.cpp -L. -loradate -I$(ORACLE_INC) -L$(ORACLE_LIB) -locci -lclntsh -lnnz11

clean:
	$(RM) $(PROJECT) oradate.o $(TEST) test.o

