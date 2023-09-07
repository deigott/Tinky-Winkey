SVC_NAME = svc.exe
TARGET_KEYLOGGER = winkey.exe

CC = cl
CFLAGS = /Iinc /EHsc /Wall /WX /Wv:12
LINK = link

SVC_SRCS =  main.cpp Tinky.cpp


SVC_OBJ = $(SVC_SRCS:.cpp=.obj)


.cpp.obj:
	@$(CC) $(CFLAGS) /c $< /Fo: $@ 

all: $(SVC_NAME)

$(SVC_NAME): $(SVC_OBJ)
	@echo "Building {tinky} service."
	@$(LINK) $(SVC_OBJ) Advapi32.lib  /OUT:$(SVC_NAME) 

clean:
	@del $(OBJ_KEYLOGGER) $(SVC_OBJ)

fclean: clean
	@del $(SVC_NAME)

re: fclean all