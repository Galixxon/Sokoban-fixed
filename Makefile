nctest:
	@gcc terminal.c -o nctest -Wall -Wextra -Werror -std=c11 -lncurses
clean:
	@rm nctest main