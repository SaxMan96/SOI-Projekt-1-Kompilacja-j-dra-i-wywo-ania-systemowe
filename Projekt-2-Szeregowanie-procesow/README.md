
Ćwiczenie 2. 
Szeregowanie procesów


1. Cel ćwiczenia

Celem  ćwiczenia  jest zaprojektowanie mechanizmu szeregowania w systemie
MINIX. W trakcie ćwiczenia należy zamienić standardow± procedurę szereguj±c±
zgodnie z algorytmem szeregowania o cechach wskazanych przez prowadz±cego.


2. Szeregowanie w systemie MINIX

W systemie MINIX procesy s± podzielone na trzy klasy: 

- zadania systemowe (TASK)

- procesy serwerowe (SERVER)

- procesy użytkowe (USER)

Procesy TASK maja bezwzględny priorytet nad procesami SERVER, a te z kolei nad
procesami USER. Procesy systemowe TASK i SERVER s± realizowane w reżimie
pobłażaj±cym, to znaczy proces realizuje się tak długo, aż sam zrzecze się
procesora.  Proces USER po wyczerpaniu kwantu czasu jest przenoszony na koniec
kolejki procesów gotowych. Procedura szereguj±ca wybiera do realizacji zawsze
proces będ±cy głow± kolejki procesów gotowych.


3. Przykładowy algorytm szeregowania

Poniżej przedstawiono przykładowy algorytm szeregowania. Proszę pamiętać, że
jets to tylko przykład, a wła¶ciwe zadanie do zrealizowania w ramach
laboratorium znajduje się w punkcie 6.

W przykładowym algorytmie szeregowania obowi±zuj± następuj±ce zasady:

- proces ma dwa atrybuty: priorytet bazowy BASE_PRI i priorytet aktualny
  ACT_PRI.

- dwie zmienne systemowe MAX_AGE i MIN_PRI, MAX_AGE>MIN_PRI, dziel±
  szeregowane procesy na trzy kategorie:

	  * priorytet bazowy > MAX_AGE - proces realizowany w reżimie 
	pobłażania (oczywi¶cie z wyj±tkiem procesów klas TASK i SERVER), priorytet
	bież±cy zawsze równy bazowemu,
    
	  * priorytet bazowy > MIN_PRI, <= MAX_AGE - proces 
	realizowany w reżimie starzenia: proces któremu jest zabierany procesor
	otrzymuje priorytet bież±cy równy bazowemu, wszystkie inne z tej grupy
	zwiększaj± priorytet bież±cy o 1 (maksymalnie do MAX_AGE); proces któremu
	jest zabierany procesor jest wstawiany do kolejki za innymi procesami o
	tym samym priorytecie bież±cym,
    
	  * priorytet bazowy < MIN_PRI - proces realizowany w reżimie 
	priorytetów statycznych z podziałem czasu: proces któremu jest zabierany
	procesor jest wstawiany do kolejki za innymi procesami o tym samym
	priorytecie bież±cym; priorytet bież±cy zawsze równy bazowemu.


4. Cechy funkcjonalne przykładowego algorytmu w systemie MINIX:

- realizacja szeregowania zgodnie z przedstawionym algorytmem,

- warto¶ci pocz±tkowe zmiennych: MAX_AGE=1000, MIN_PRI=100,

- każdy nowo tworzony proces otrzymuje priorytet bazowy i bież±cy równe
  MIN_PRI,

- system udostępnia nowe wywołanie - ustaw parametr szeregowania:

	void set_PRI (int nr; unsigned VALUE) 

	nr = 1  nadaj bież±cemu procesowi priorytet VALUE 
	nr = 2  ustaw zmienna MAX_AGE na VALUE 
	nr = 3  ustaw zmienna MIN_PRI na VALUE

5. Wskazówki implementacyjne przy implementacji algorytmu przykładowego

a) plik kernel/proc.h:
	
	- w strukturze 'proc' umie¶cić priorytet bazowy i bież±cy procesu,
	
	- zadeklarować zmienne systemowe MAX_AGE i MIN_PRI.

b) plik kernel/proc.c:
	
	- w funkcji 'sched()', która jest procedur± szereguj±c±, zamiast
	  przenoszenia procesu bież±cego na koniec kolejki procesów gotowych
	  zrealizować zasady algorytmu przykładowego,
	
	- w funkcji 'ready(rp)', która budzi proces, zadbać o umieszczenie go w
	  odpowiednim miejscu struktur algorytmu (jeżeli jest to proces klasy
	  USER).

c) plik kernel/system.c:
	
	- w funkcji 'do_fork(m_ptr)', która tworzy nowy proces, w czę¶ci
	  dotycz±cej procesów klasy USER zadbać o odpowiednie zainicjowanie
	  priorytetu bazowego i bież±cego procesu.

d) plik kernel/main.c:
	
	- w funkcji 'main()', na pocz±tku zadbać o wła¶ciwe zainicjowanie
	  zmiennych systemowych MAX_AGE i MIN_PRI, w pętli powołuj±cej deskryptory
	  procesów (for (rp=BEG...)) zadbać o wła¶ciwe wypełnienie priorytetów, w
	  czę¶ci końcowej powołuj±cej proces INIT zadbać również o odpowiednie
	  warto¶ci priorytetów.

e) dodać do systemu wywołanie systemowe zgodnie ze składni± podan± w punkcie
4, z tym, że odwołanie ma nast±pić do samego mikroj±dra za po¶rednictwem
jednego z modułów MM/FS. Dlatego też funkcja realizuj±ca dodane wywołanie
systemowe (np. o nazwie do_setpri()) w jednym z modułów MM/FS powinna mieć
poniższa postać:

PUBLIC void do_setpri() 
{ 	
	message m; 
	m = mm_in; 
	_taskcall(SYSTASK, SYS_SETPRI, &m); 
}

gdzie SYS_SETPRI jest zdefiniowana przez nas stała w pliku
/usr/include/minix/com.h. 

Ponadto, do pliku /usr/src/kernel/system.c należy dodać obsługę funkcji o
numerze SYS_SETPRI i zdefiniować sam± funkcję obsługuj±c± to wywołanie. W
obsłudze można wykorzystać makro proc_addr( id ) zwracaj±ce wskazanie na
strukturę proc zadanego procesu.


6. Zadanie do zrealizowania

Prowadz±cy poleci studentom zrealizować algorytm szeregowania o zadanych
własno¶ciach funkcjonalnych. Przykład: zrealizować algorytm szeregowania który
zapewni wła¶ciwe traktowanie procesów trzech kategorii: interaktywnych (np.
gra w statki), obliczeniowych (obliczenia numeryczne) oraz masywnego
wej¶cia/wyj¶cia realizowanego w tle (np. proces składowania dysku na ta¶mie).


