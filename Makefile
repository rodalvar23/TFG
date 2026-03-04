SRCS = $(wildcard src/*.c)
OBJS = ${SRCS:.c=.o}
CC = gcc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror
LIB = $$(pkg-config --cflags --libs sdl2 SDL2_ttf) ./include/Rozum-Servo-Drives-API/c/build/libservo_api.a -lpthread -lm
OUT =  -o a.out
AR = ar rcs
NAME = librobotic_arm.a
TEST_ROOT = ./build/main_tester.c
LINK = -L./build/ -lrobotic_arm

.c.o: 
	 ${CC} ${CFLAGS} -c $< -o $(<:.c=.o)


$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)
	mv librobotic_arm.a build 

all: $(NAME)

test:
	${CC} ${CFLAGS} ${TEST_ROOT} ${LINK} ${LIB} -o execute
	mv execute build

exe:
	./build/execute

clean:
	${RM} ${OBJS}
fclean: clean
	${RM} a.out
	$(RM) ./build/$(NAME)
	${RM} ./build/execute
re: fclean all

setup_ports:
	./tools/setup_ports.sh

full: re test exe

view_ports:
	@echo "Tus puertos actuales disponibles son:"
	@ls -l /dev/rozum_api /dev/rozum_log /dev/u2d2* 2> /dev/null || echo "(No hay dispositivos conectados en este momento)" 

.PHONY: all clean fclean re setup_ports view_ports test exe
