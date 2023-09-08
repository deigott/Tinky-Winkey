SVC_NAME = winkey.exe
TARGET_KEYLOGGER = winkey.exe

CC = cl
CFLAGS = /Iinc /EHsc /Wall /WX /Wv:12
LINK = link

SVC_SRCS =  main.cpp Winkey.cpp


SVC_OBJ = $(SVC_SRCS:.cpp=.obj)


.cpp.obj:
	@$(CC) $(CFLAGS) /c $< /Fo: $@ 

all: $(SVC_NAME)

$(SVC_NAME): $(SVC_OBJ)
	@echo "Building {winkey} program."
	@$(LINK) $(SVC_OBJ)  /OUT:$(SVC_NAME) 

clean:
	@del $(OBJ_KEYLOGGER) $(SVC_OBJ)

fclean: clean
	@del $(SVC_NAME)

re: fclean all