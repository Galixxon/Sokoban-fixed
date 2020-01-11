Projekt z C. Gra Sokoban w 2 wersjach graficznych (ncurses i SDL) ze wspólną logiką w osobnym module.


PROJEKT NA C: Sokoban

GRAFIKA-> SDL / Ncurses:
  ->Rysowanie planszy
  ->
Logika:
  ->zapisanie pól w postaci tablicy
  ->poruszanie:
	. Przyjęcie inputu DONE
	. Sprawdzenie czy pole jest dostępne
	. Jeśli tak to przejdź
	. Wpp: sprawdź czy to ściana czy skrzynia
	. Jeśli to skrzynia to spróbuj przesunąć, wpp nie rób nic
  ->kontakt ze skrzyniami (przesuwanie):
	. Skrzynia rusza w tym samym kierunku co dotykający jej gracz
	. Sprawdzenie pola
	. Jeżeli jest puste to przesuń i zajmij miejsce skrzyni
	. wpp nie rób nic
  ->win condition:
	. sprawdź po umieszczeniu skrzyni w dziurze czy wszystkie skrzynie są na miejscach
	. Jeżeli tak to skończ gre, wpp nie rób nic
  ->Tablica:
	. Typy pól: puste, do chodzenia, ściana, dziura na skrzynie
	. Wskaźniki: gracz, skrzynie

----------------------------------------------------------
logic:
	. File Handling
	. Level select -> getLevels(), selectLevel(), getLevelInfo()
	. Game handling -> canMove(), moveBox(), movePlayer(), gameReset(), checkWinCon()
graphic:
	. 2 windows (level select & game)
	. Player input
	. MainLoop -> getPlayerInput(),  UpdateDisplay()
	. Display -> Init-Display()
	





















