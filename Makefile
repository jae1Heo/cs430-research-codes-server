# Variables
CC = gcc
EXE = test
# -I for headers, -L for library paths, -l for specific libraries
LDFLAGS = -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto -lpthread
CFLAGS = -I/opt/homebrew/opt/openssl@3/include

#LDFLAGS = -lssl -lcrypto -lpthread
# for Linux client

# The source files
SRCS = src/run.c src/server/server.c src/logic/game.c src/client_mgmt/clnt.c src/auth/auth.c

# The build rule
all:
	$(CC) -o $(EXE) $(SRCS) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(EXE)