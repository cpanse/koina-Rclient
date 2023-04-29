INCL=/home/cpanse/src/v2.34.0dev.clients/include/
CXXFLAG=-Wall -pedantic
dlomix: dlomix.cc
	g++ -o dlomix dlomix.cc  -I $(INCL) -L /home/cpanse/src/v2.34.0dev.clients/lib/ -lgrpcclient $(CXXFLAG)
hello:
	g++ -o simple_grpc_keepalive_client simple_grpc_keepalive_client.cc  -I $(INCL) -L /home/cpanse/src/v2.34.0dev.clients/lib/ -lgrpcclient $(CXXFLAG)
