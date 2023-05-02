INCL=/home/cp/src/v2.34.0dev.clients/include/
CXXFLAG=-Wall -pedantic -std=c++17 -O3
dlomix: dlomix.cc
	g++ -o dlomix dlomix.cc  -I $(INCL) -L /home/cp/src/v2.34.0dev.clients/lib/ -lgrpcclient $(CXXFLAG)
dlomixProsit: dlomixProsit.cc
	g++ -o dlomixProsit dlomixProsit.cc  -I $(INCL) -L /home/cpanse/src/v2.34.0dev.clients/lib/ -lgrpcclient $(CXXFLAG)
clean:
	$(RM) dlomix

hello:
	g++ -o simple_grpc_keepalive_client simple_grpc_keepalive_client.cc  -I $(INCL) -L /home/cpanse/src/v2.34.0dev.clients/lib/ -lgrpcclient $(CXXFLAG)
