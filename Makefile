SRCS = $(wildcard *.c)
OBJS = ${SRCS:.c=.o}
CC = gcc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror
LIB = ./Rozum-Servo-Drives-API/c/build/libservo_api.a -lpthread -lm
OUT =  -o a.out

.c.o: 
	 ${CC} ${CFLAGS} ${SRCS} ${LIB} ${OUT} 

all : .c.o
	

clean:
	${RM} ${OBJS}
fclean: clean
	${RM} a.out
re: fclean all

setup_ports:
	./setup_ports.sh

view_ports:
	@echo "Tus puertos actuales disponibles son:"
	@ls -l /dev/rozum_api /dev/rozum_log /dev/u2d2* 2> /dev/null || echo "(No hay dispositivos conectados en este momento)" 

.PHONY: all clean fclean re setup_ports view_ports
