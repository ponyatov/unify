log.log: ./exe.exe
	./exe.exe > $@ && tail $@ 
./exe.exe: cpp.cpp
	g++ -std=gnu++11 -o $@ $<