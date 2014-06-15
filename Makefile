CC = clang
CXX = clang++
CXXFLAGS = -std=c++11
LIBS = -lboost_unit_test_framework
.PHONY:clean
test:test.o
	$(CXX) -o $@ $^ $(LIBS)
clean:
	rm -f test *.o
