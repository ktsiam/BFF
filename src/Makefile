CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

bff: bff.o
	$(CXX) -o bff bff.o

bff.o : bff.cpp

clean:
	rm -rf *.dSYM
	rm -f bff
	rm -f *.o
