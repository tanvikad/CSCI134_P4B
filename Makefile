all: test_button lab4b

lab4b: lab4b.c
	gcc -Wall -Wextra -g  lab4b.c -o lab4b -lrobotcontrol -lm

test_button: test_buttons.c
	gcc -Wall -Wextra -g  test_buttons.c -o test_buttons -lrobotcontrol -lm

clean:
	rm -f *.o
	rm -f lab4b
	rm -f test_buttons
	rm -f *.gz
	rm -f *.txt
