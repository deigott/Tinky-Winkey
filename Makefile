SVC_NAME = svc.exe
WIN_NAME = winkey.exe

CC = cl
CFLAGS = /Iinc /EHsc /Wall /WX /Wv:12
LINK = link

SVC_SRCS =  tinky/main.cpp tinky/Tinky.cpp
WIN_SRCS = winkey/main.cpp winkey/Winkey.cpp


SVC_OBJ = $(SVC_SRCS:.cpp=.obj)
WIN_OBJ = $(WIN_SRCS:.cpp=.obj)

.cpp.obj:
	@$(CC) $(CFLAGS) /c $< /Fo: $@ 


all: $(SVC_NAME) $(WIN_NAME)

$(SVC_NAME): $(SVC_OBJ)
	@echo Building {tinky} program.
	@$(LINK) $(SVC_OBJ) /OUT:$(SVC_NAME) 

$(WIN_NAME): $(WIN_OBJ)
	@echo Building {winkey} program.
	@$(LINK) $(WIN_OBJ) /OUT:$(WIN_NAME)

clean:
	@rm -rf $(SVC_OBJ)
	@rm -rf $(WIN_OBJ)

fclean: clean
	@rm -rf $(SVC_NAME)
	@rm -rf $(WIN_NAME)

re: fclean all