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


Źródło map: https://www.sourcecode.se/sokoban/levels

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
	



5. 	Zaprogramować w miarę prostą grę np. sokoban, może to być też inna znana gra ale z jakimiś
	oryginalnymi modyfikacjami kształtów lub logiki. Gra powinna być napisana w taki sposób aby
	rozdzielić logikę od wyświetlania i napisać dwie wersje wyświetlania: graficzną przy użyciu
	biblioteki SDL i tekstową działającą na terminalu. Obie wersje mają mieć te same moduły (pliki
	.c) z logiką, a różne tylko moduły do wyświetlania (SDL/text) np. zawierające funkcje Init-
	Display(), UpdateDisplay(). W zależności co linkujemy to otrzymamy wersje tekstową
	(ncurses) lub graficzną (SDL).



pliki: mapy.txt

1.Level select: 
	-> Wczytaj z pliku listę poziomów:
		-> Czytaj linia po linii aż do ';' z pliku maps.txt
		-> Licz linie by znaleźć wysokość
		-> 
	-> Narysuj okno wyboru (zależnie od grafiki)
	-> Pozwól ruszać wskaźnikiem wyboru
	-> Pobierz dane wybranego poziomu i zapisz w pamięci
	-> Rozpocznij grę z tym poziomem

2.Game process:
	-> Pobierz ruch od gracza
	-> Sprawdź na jakie pole chce przejść gracz
	-> Sprawdź czy to pole jest podłogą
	-> Sprawdź czy jest tam skrzynia
	-> Sprawdź czy można ją przesunąć
	-> Jeżeli można to przesuń skrzynię i przejdź na jej pole

3.Game finish:
	-> Jeżeli przesunięto skrzynię to sprawdź czy wszystkie są na swoich miejcach




















