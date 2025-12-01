exename=test
imports=-lssl -lpthread -lcrypto

run:
	gcc -o $(exename) src/run.c src/server/server.c src/logic/game.c src/client_mgmt/clnt.c src/auth/auth.c $(imports)