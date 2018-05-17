CC	:= g++

RM	:= del /S

SFML_FLAGS	+= -Llibraries/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

SFML_INCLUDE	+= -Ilibraries/SFML/include

CLIENT_SRCS	:= scr/Client.cpp\
				src/ClientManager.cpp\
				src/ClientNetworkManager.cpp\
				src/Graphics.cpp

SERVER_SRCS := src/Server.cpp\
				src/ServerManager.cpp

CLIENT_OBJS	:= out/Client.o\
				out/ClientManager.o\
				out/ClientNetworkManager.o\
				out/Graphics.o

SERVER_OBJS	:= out/Server.o\
				out/ServerManager.o

CLIENT_NAME	:= out/Client.exe

SERVER_NAME	:= out/Server.exe

all: $(CLIENT_NAME) $(SERVER_NAME)

$(CLIENT_NAME): $(CLIENT_OBJS)
	$(CC) -std=c++11 $(CLIENT_OBJS) -o $(CLIENT_NAME) $(SFML_FLAGS)

$(CLIENT_OBJS): $(CLIENT_SRCS)
	$(CC) -std=c++11 -o $(CLIENT_OBJS) -c $(CLIENT_SRCS) $(SFML_INCLUDE)

$(SERVER_NAME): $(SERVER_OBJS)
	$(CC) -std=c++11 $(SERVER_OBJS) -o $(SERVER_NAME) $(SFML_FLAGS)

$(SERVER_OBJS): $(SERVER_SRCS)
	$(CC) -std=c++11 -o $(SERVER_OBJS) -c $(SERVER_SRCS) $(SFML_INCLUDE)

#clean:
#	$(RM) $(OBJS)
#
#fclean: clean
#	$(RM) $(NAME)
#
#re: fclean all
#
#.PHONY: all re clean fclean