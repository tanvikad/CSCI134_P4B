all: test_button

test_button: test_buttons.c
	gcc -Wall -Wextra -g  test_buttons.c -o test_buttons -lrobotcontrol -lm
