terminal:
	@gcc terminal.c "./src/levelmanager.c" "./src/logic.c" -g -o SokobanTerminal.sb -I "./headers/" -Wall -Wextra -Werror -std=c11 -lmenu -lncurses
	
clean:
	@rm *.sb