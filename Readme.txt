
Æwiczenie 1.
Kompilacja j¹dra i wywo³ania systemowe


1. Cel æwiczenia

Celem æwiczenia jest zapoznanie siê z mechanizmem realizacji wywo³añ
systemowych (ang. system calls, t³umaczone równie¿ jako funkcje
systemowe). W trakcie æwiczenia nale¿y dodaæ do systemu nowe
wywo³anie systemowe, zwracaj¹ce numer (indeks) procesu w tablicy
procesów. Do przeprowadzenia æwiczenia niezbêdna jest dodatkowa
dyskietka.


2. Wiadomoœci ogólne

Wywo³ania systemowe s¹ to funkcje realizowane przez j¹dro systemu
operacyjnego w kontekœcie danego procesu. Jest to jedyny dopuszczalny
dla procesów u¿ytkowych sposób wejœcia do j¹dra systemu. Przyk³adowymi
wywo³aniami systemowymi s¹: READ, WRITE, FORK, EXIT. Nale¿y odró¿niæ
wywo³ania systemowe od realizuj¹cych je funkcji bibliotecznych: read(),
write(), fork(), exit().


3. Wywo³ania systemowe w systemie MINIX

W systemie MINIX wywo³ania systemowe s¹ realizowane wewn¹trz jednego z
modu³ów-serwerów: MM lub FS, w zale¿noœci od rodzaju samego wywo³ania.
Wszystkie wywo³ania dotycz¹ce szeroko rozumianych operacji plikowych
(np. OPEN, READ, WRITE, IOCTL) s¹ obs³ugiwane przez FS. Pozosta³e
wywo³ania (np. GETPID, FORK, EXIT) s¹ obs³ugiwane przez MM. W obu
modu³ach, w plikach table.c, znajduj¹ siê tablice call_vec, które
okreœlaj¹, które wywo³ania systemowe s¹ w danym module obs³ugiwane i
jaka funkcja tym siê zajmuje. Procesy u¿ytkowe maja do swojej
dyspozycji odpowiednie procedury biblioteczne (np. open(), fork() itd.).

4. Zapoznaj siê z plikami Ÿród³owymi potrzebnymi do generacji
j¹dra systemu Minix 2.0 (SM). Pliki znajduj¹ siê w nastêpuj¹cych
katalogach:

/usr/src/kernel- Ÿród³a wszystkich sterowników oraz kodu j¹dra

/usr/src/mm - Ÿród³a modu³u Memory Menager (MM)

/usr/src/fs - Ÿród³a modu³u File System (FS)

/usr/src/tools - Ÿród³a programów s³u¿¹cych do ³¹czenia czêœci SM w
jeden bootowalny program oraz do inicjacji systemu.


5. Dodanie do systemu nowego wywo³ania systemowego

Dodaj do systemu nowe wywo³anie systemowe GETPROCNR, obs³ugiwane
wewn¹trz modu³u MM i zwracaj¹ce numer procesu w tablicy procesów dla
procesu, którego identyfikator (PID) bêdzie argumentem tego wywo³ania.

W tym celu nale¿y:

  1. W pliku /usr/include/minix/callnr.h dodaæ identyfikator nowego
  wywo³ania systemowego GETPROCNR i ewentualnie zwiêkszyæ o jeden
  sta³¹ N_CALLS.
  
  2. Napisaæ procedurê obs³ugi do_getprocnr i umieœciæ ja na przyk³ad
  w pliku /usr/src/mm/main.c
  
  Prototyp tej funkcji umieœciæ w pliku /usr/src/mm/proto.h. Funkcja
  ta powinna porównaæ przekazany do niej identyfikator procesu z
  identyfikatorami procesów u¿ytkowych znajduj¹cymi siê w tablicy
  mproc, plik /usr/src/mm/mproc.h
  
  Je¿eli identyfikatory te s¹ takie same, to funkcja musi zwróciæ
  numer (indeks) procesu w tablicy procesów, w przeciwnym razie nale¿y
  zasygnalizowaæ b³¹d ENOENT zdefiniowany w pliku /usr/include/errno.h
  
  W zdefiniowanej przez nas procedurze do_getprocnr mamy mo¿liwoœæ
  odwo³ywania siê do zmiennych typu message:
  
  mm_in - zawiera dane przekazywane do wywo³ania,
  
  mm_out - zawiera dane zwracane do procesu wywo³uj¹cego.
  
  W pliku /usr/src/mm/param.h znajduj¹ siê definicje u³atwiaj¹ce
  odwo³ania do elementów wy¿ej wymienionych struktur. Wartoœæ zwracana
  przez nasz¹ funkcjê do_getprocnr umieszczana jest w strukturze
  mm_out automatycznie (w polu m_type). Zapoznaj siê ze znaczeniem
  pola mp_flags struktury mproc, zwróæ uwagê na flagê IN_USE.
  
  
  3. W pliku /usr/src/mm/table.c w tablicy call_vec w odpowiednim
  miejscu wstawiæ adres (nazwê) funkcji do_getprocnr, zaœ w pliku
  /usr/src/fs/table.c w tym samym miejscu umieœciæ adres pusty
  funkcji, no_sys.
  
  
  4. Dokonaæ rekompilacji i prze³adowania systemu Minix z nowym
  j¹drem.  Procedura ta ma ró¿ny przebieg w zale¿noœci od tego, czy
  æwiczenie wykonywane jest w systemie Minix zainstalowanym na twardym
  dysku, w systemie Minix pracuj¹cym pod kontrol¹ emulatora Bochs, czy
  te¿ wreszcie wykonywane jest w specjalnie przygotowanej dystrybucji
  systemu Minix w ca³oœci ³adowanej do RAM-dysku.

  Poni¿ej zostanie opisany proces kompilacji nowego j¹dra w ostatniej 
  z wymienionych konfiguracji.
  
  Rekompilacja i restart systemu Minix w œrodowisku ze specjaln¹
  wersj¹ systemu Minix ³aduj¹c¹ siê w ca³oœci do RAM-dysku
  
	a. przejœcie do katalogu /usr/src/tools
    
	b. zapoznanie siê z akceptowalnymi zleceniami dla programu make
	(czyli z zawartoœci¹ pliku Makefile) oraz z zawartoœci¹ skryptu
	mkboot,
    
	c. kompilacja nowego j¹dra wraz z utworzeniem dyskietki startowej
	z nowym j¹drem:
    
	 $ make fdboot
    
	(inne mo¿liwoœci, m.in.: "make hdboot" kompilacja i instalacja na 
	twardym dysku/obrazie, sam "make", kompilacja bez instalacji bloku 
	³aduj¹cego, zobacz zawartoœæ Makefile)
    
	d. bardzo wa¿nym krokiem, który nale¿y wykonaæ najpóŸniej w tym
	momencie, jest zachowanie na zewnêtrznym noœniku wszelkich zmian w
	Ÿród³ach, dokonanych w celu wykonania æwiczenia laboratoryjnego.
	Nale¿y pamiêtaæ, ¿e po prze³adowaniu systemu odtwarzana jest w
	pamiêci RAM wersja systemu bez jakichkolwiek zmian wprowadzonych
	przez u¿ytkownika.
    
	e. Po zachowaniu zmienionych wersji plików Ÿród³owych nale¿y
	wykonaæ restart systemu z wykorzystaniem utworzonego j¹dra. 
	
	 $ cd 
	 $ shutdown  (lub wciœniêcie <Ctrl-Alt-Del>)
	 $ boot
    
	f. Testowanie w³asnoœci nowego j¹dra. Ewentualne za³adowanie z
	dyskietki uprzednio zachowanych zmian Ÿróde³ systemu, a w
	szczególnoœci zawartoœci plików nag³ówkowych, które powinny byæ
	spójne dla j¹dra i przygotowywanych pod system z nowym j¹drem
	aplikacji.
	
    
6. Program testuj¹cy 

Napisz krótki program testuj¹cy poprawnoœæ dzia³ania zaimplementowanego
wywo³ania systemowego. Zadaniem programu jest wywo³anie zdefiniowanej
funkcji systemowej dla zadanego jako argument wywo³ania programu
testowego identyfikatora procesu oraz dla 10 kolejnych identyfikatorów
liczbowych procesów.  Przyk³adowo, wywo³anie programu testowego z
argumentem 100 powinno odpytaæ o pozycje w tablicy procesów dla procesów
o identyfikatorach od 100 do 110.  Dla ka¿dego wywo³ania funkcji
systemowej, program testuj¹cy powinien wyœwietlaæ albo zwrócony indeks w
tablicy procesów, albo te¿, w przypadku wyst¹pienia b³edu, kod b³êdu
ustawiany przez funkcjê _syscall() w zmiennej errno.

Aby wywo³aæ funkcjê systemow¹ nale¿y skorzystaæ z funkcji bibliotecznej
_syscall (/usr/include/lib.h). Jej pierwszym argumentem jest numer
serwera (MM lub FS), drugim numer wywo³ania systemowego, trzecim
wskaŸnik na strukturê message, w której umieszczamy dane dla wywo³ania.
Deklaracja struktury message znajduje siê w pliku
/usr/include/minix/type.h. Do przekazania numeru PID procesu zaleca siê
u¿ycie pola m1_i1 tej struktury.  Funkcjê _syscall() zdefiniowano w
pliku /usr/src/lib/others/syscall.c 

W programie testuj¹cym wskazane jest zaimplementowanie funkcji o
sygnaturze

	 int getprocnr( int )

wewnêtrznie wykorzystuj¹cej _syscall() do uzyskania pozycji procesu w
tablicy procesów. Poprawnoœæ rezultatu zwracanego przez wywo³anie
zdefiniowanej funkcji systemowej mo¿na weryfikowaæ analizuj¹c aktualn¹
zawartoœæ tablicy procesów oraz wykorzystuj¹c komendê ps. Tablicê
procesów mo¿na wyœwietliæ na pierwszej konsoli poprzez naciœniêcie
klawisza F1. Analogicznie poprzez wciœniêcie F2 uzyskuje siê aktualn¹
mapê pamiêci.

