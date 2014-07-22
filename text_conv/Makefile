
CXXFLAGS=
LDFLAGS=
PROJECT=libtext_conv.a
TEST=test

all: $(PROJECT)

$(PROJECT): text_conv.h text_conv.cpp
	$(CXX) -c text_conv.cpp $(CXXFLAGS)
	$(AR) rcs $(PROJECT) text_conv.o

$(TEST): test.cpp $(PROJECT)
	$(CXX) -o $(TEST) test.cpp -L. -ltext_conv

clean:
	$(RM) $(PROJECT) text_conv.o $(TEST) test.o

