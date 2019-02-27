CC    = gcc
CP    = cp
RM    = rm
MKDIR = mkdir

CFLAGS  = -Wall -fPIE -pie -DVERSION='"$(VERSION)"'
CPFLAGS =
RMFLAGS =
MKDIRFLAGS = -p

VERSION = 3.0.1(Beta)

prefix = /usr/local/bin

NAME = er
OBJ  = main.o er.o 
LIBS = -lshiro

$(NAME):$(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBS)

main.o:main.c er.h
er.o  :er.c er.h

.PHONY:install uninstall clean

install:
	$(MKDIR) $(MKDIRFLAGS) $(prefix)
	$(CP) $(CPFLAGS) $(NAME) $(prefix)/$(NAME)
uninstall:
	$(RM) $(RMFLAGS) $(prefix)/$(NAME)
clean:
	-$(RM) $(RMFLAGS) $(NAME) $(OBJ)
