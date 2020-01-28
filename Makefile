
DEF_OBJCTS = "./src/levelmanager.c" "./src/logic.c" -g -I "./headers/" -Wall -Wextra -std=c11

terminal:
	@gcc terminal.c $(DEF_OBJCTS) -o SokobanTerminal.sb -lmenu -lncurses

graphical:
	@gcc graphical.c $(DEF_OBJCTS) -o SokobanInSDL.sb -lSDL2 -lSDL2_image -lSDL2_ttf

clean:
	@rm *.sb