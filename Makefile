SRC = main.cpp rsa.cpp excep.cpp utils.cpp decode.cpp encode.cpp generate_keys.cpp init.cpp parse_arguments.cpp 
RSA_simple: $(SRC)
	g++ -Wall -c $(SRC)
	g++ -Wall -o RSA_simple *.o
