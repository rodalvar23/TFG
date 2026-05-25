SRCS = $(wildcard src/*.c)
OBJS = ${SRCS:.c=.o}
CC = gcc
C++ = g++
RM = rm -f
CFLAGS = -Wall -Wextra -Werror
LIB = $$(pkg-config --cflags --libs sdl2 SDL2_ttf) ./include/Rozum-Servo-Drives-API/c/build/libservo_api.a -lpthread -lm
OUT =  -o a.out
AR = ar rcs
NAME = librobotic_arm.a
TEST_ROOT = ./test/main_tester.c
TEST_ROOT_C++ = ./test/main_tester.cpp
LINK = -L./test/ -lrobotic_arm

.c.o: 
	 ${CC} ${CFLAGS} -c $< -o $(<:.c=.o)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)
	mv librobotic_arm.a test 

all: $(NAME)

test:
	${CC} ${CFLAGS} ${TEST_ROOT} ${LINK} ${LIB} -o execute
	mv execute test

exe:
	./test/execute

clean:
	${RM} ${OBJS}
fclean: clean
	${RM} a.out
	$(RM) ./test/$(NAME)
	${RM} ./test/execute
re: fclean all

setup_ports:
	./tools/setup_ports.sh

full: re test exe

test_c++:
	${C++} ${CFLAGS} ${TEST_ROOT_C++} ${LINK} ${LIB} -o execute
	mv execute test

view_ports:
	@echo "Tus puertos actuales disponibles son:"
	@ls -l /dev/rozum_api /dev/rozum_log /dev/u2d2* 2> /dev/null || echo "(No hay dispositivos conectados en este momento)" 

.PHONY: all clean fclean re setup_ports view_ports test exe
