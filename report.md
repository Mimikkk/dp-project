# Stowarzyszenie umarłych poetów

Sprawozdanie na potrzeby laboratorium z przedmiotu "Przetwarzanie Rozproszone".

Autorzy:

- Jakub Błażejowski 145393
- Daniel Zdancewicz 145317

Grupa: L4, wtorek 15:10

<div style="page-break-before: always;"></div>

## Opis problemu

W pewnym mieście uniwersyteckim grupa młodych ludzi wpadła na pomysł, by zwiększyć swoje zdolności twórcze konsumpcją dużej ilości alkoholu. W tym celu tworzą kółka poetyckie, po czym każdy przynosi alkohol lub zagrychę, przeprowadzają libację, przy okazji usiłując spłodzić dzieła mające przebić Przybyszewskiego. Po libacji poeta jakiś czas odpoczywa. Poeci działają pod natchnieniem chwili, więc mogą zdecydować w sposób losowy, że do jakiegoś kółka nie chcą przynależeć.

Danych jest P poetów. Poeci dobierają się w kółka o dowolnej wielkości. Przy dobieraniu się w kółka poeta może zdecydować, że do danego kółka nie chce należeć. Po zebraniu w kółko poeci decydują, kto przyniesie alkohol, a kto zagrychę, a kto może pić na sępa. Poeci nie lubią być frajerami, więc nie mogą non stop przynosić jednego rodzaju towaru lub sępić. Następnie wynajmują jeden z pokojów pracy twórczej, przeprowadzają libację, rozwiązują kółko. Po libacji powiadamiani są wolontariusze, która doprowadza pokój do porządku. Wolontariuszy jest W, decydują między sobą, kto posprząta pokój. Pokoje wynajmowane są na mieście, można więc przyjąć, że jest ich nieskończenie wiele - poeci z kółka nie mogą jednak przystąpić do kolejnej libacji, dopóki nie mają pewności, że jakiś wolontariusz zajął się pokojem.

## Założenia

- Środowisko jest w pełni asynchroniczne
- Kanały między procesami są FIFO
- Kanały między procesami są niezawodne
- Procesy nie ulegają awarii

## Przeprowadzenie pełnej libacji z U uczestnikami:

- złożoność komunikacyjna: $ 2 \cdot P + 3 \cdot U + 2 \cdot W - 1 $
- złożoność czasowa: $ U + 10 $

<div style="page-break-before: always;"></div>

## Algorytm poetów:

1. Jeśli brałeś udział w libacji (odmawiaj na każde zaproszenie do koła):
   1. Oczekuj na wiadomość o sprzątaniu pokoju
   2. Odpoczywaj przez losową liczbę sekund
2. Utwórz koło naukowe z prawdopodobieństwem
3. Jeżeli jesteś twórcą koła (odmawiaj na każde zaproszenie do koła):
   1. Poinformuj resztę poetów o istnieniu koła
   2. Oczekuj na decyzję o dołączeniu od reszty poetów
   3. Jeżeli lista uczestników jest mniejsza niż 2:
      1. Przejdź do 3.9.1.
   4. Dokonaj decyzji co przyniesiesz na podstawie swojej historii nadając priorytet alkoholowi i zagrysze
   5. Rozpocznij wysyłanie listy uczestników i listy decyzji z załączoną swoją decyzją do następnego uczestnika z listy
   6. Oczekuj na wiadomość od ostatniego uczestnika z listy
   7. Stwierdź czy dalsze działanie koła jest możliwe (czy jest zagrycha i alkohol)
   8. Jeśli na liście decyzji występuje alkohol oraz zagrycha to:
      1. Poinformuj członków koła, że pora zacząć twórczą działalność
      2. Poinformuj losowego wolontariusza o potrzebie posprzątania pokoju
      3. Oczekuj na odpowiedź od wolontariusza
      4. Poinformuj członków koła o sprzątaniu pokoju
   9. W przeciwnym wypadku:
      1. Poinformuj członków koła o rozwiązaniu koła
4. W przeciwnym wypadku:
   1. Oczekuj na propozycję dołączenia do koła
   2. Dokonaj decyzji o dołączeniu do koła z prawdopodobieństwem
   3. Jeśli odmawiasz:
      1. Wróć do 1.
   4. Jeśli akceptujesz (odmawiaj na każde zaproszenie do koła):
      1. Oczekuj na listę uczestników i listę decyzji
      2. Dokonaj decyzji co przyniesiesz na podstawie swojej historii i otrzymanej listy decyzji nadając priorytet
         alkoholowi i zagrysze
      3. Wyślij obie listy do następnego uczestnika z listy
      4. Oczekuj na informację o rozpoczęciu libacji
5. Wróć do 1.

<div style="page-break-before: always;"></div>

## Algorytm wolontariuszy:

### Uzgadnianie:

1. Jeżeli id wolontariusza jest ustawione:
   1. Usuń zapisane id wolontariusza
   2. Inkrementuj licznik odmów
   3. Jeżeli licznik odmów jest większy od stałej:
      1. Wróć do Główny proces:4
2. W przeciwnym wypadku:
   1. Ustaw licznik odmów na 0
3. Jeżeli lista wolontariuszy jest pusta:
   1. Wróć do Główny proces:4
4. Zapisz id wolontariusza z początku listy wolontariuszy
5. Poinformuj wolontariusza, którego id zapisałeś, o potrzebie sprzątania pokoju z początku listy pokojów.

<div style="page-break-before: always;"></div>

### Główny proces:

1. Utwórz listę pokojów czekających na sprzątanie
2. Ustaw swój zegar lamporta na swój identyfikator procesu
3. Utwórz początkową listę par (wartość zegara lamporta, id wolontariusza), w postaci `[(0, 0), (1, 1), (2, 2), ..., (n-1, n-1)]`
4. Czekaj na zdarzenie
5. Jeżeli otrzymałeś wiadomość z prośbą o sprzątanie od poety:
   1. Umieść identyfikator poety na końcu listy pokojów
   2. Jeżeli długość listy pokojów jest równa 1
      1. Przejdź do Uzgadnianie:1.
6. Jeżeli otrzymałeś wiadomość z prośbą o sprzątanie od wolontariusza:
   1. Jeżeli twój stan to: Zajęty:
      1. Wyślij informację o odmowie do nadawcy
      2. Wróć do 4.
   2. Wyślij informację o rozpoczęciu sprzątania, z załączonym id pokoju, do wszystkich wolontariuszy
   3. Wyślij informację o akceptacji do nadawcy.
   4. Zmień stan na: Zajęty
   5. Zacznij sprzątać
7. Jeżeli otrzymałeś wiadomość z informacją o rozpoczęciu sprzątania przez wolontariusza:
   1. Usuń z listy wolontariuszy wolontariusza, którego id jest równe id nadawcy
8. Jeżeli otrzymałeś wiadomość z informacją o zakończeniu sprzątania przez wolontariusza:
   1. Jeżeli ty jesteś nadawcą:
      1. Zmień status na: Wolny
      2. Poinformuj poetę o zakończeniu sprzątania
      3. Jeżeli licznik odmów jest większy od stałej:
         1. Usuń pierwszy element z listy pokojów
         2. Zmień stan na: Zajęty
         3. Zacznij sprzątać
         4. Jeżeli długość listy pokojów jest większa lub równa 1:
            1. Przejdź do Uzgadnianie:1.
      4. W przeciwnym wypadku:
         1. Wyślij informację o zakończeniu sprzątania, razem z zapisaną wartością zegara lamporta, do reszty wolontariuszy.
         2. Dodaj do listy wolontariuszy parę (wartość zegara lamporta nadawcy, id nadawcy)
         3. Jeżeli długość listy wolontariuszy jest równa 1 i długość listy pokojów jest większa lub równa 1:
            1. Przejdź do Uzgadnianie:1.
   2. W przeciwnym wypadku:
      1. Dodaj do listy wolontariuszy parę (odebrana wartość zegara, id nadawcy)
      2. Jeżeli długość listy wolontariuszy jest równa 1 i długość listy pokojów jest większa lub równa 1 i licznik odmów jest mniejszy od stałej i wolontariusz nie jest ustawiony:
         1. Przejdź do Uzgadnianie:1.
   3. Dodaj do listy wolontariuszy parę (wartość zegara lamporta nadawcy, id nadawcy)
   4. Posortuj rosnąco listę wolontariuszy
9. Jeżeli otrzymałeś wiadomość z informacją o akceptacji:
   1. Usuń pierwszy element z listy pokojów
   2. Usuń zapisane id wolontariusza
   3. Jeżeli długość listy pokojów jest większa lub równa 1:
      1. Przejdź do Uzgadnianie:1.
10. Jeżeli otrzymałeś wiadomość z informacją o odmowie:
    1. Przejdź do Uzgadnianie:1.
11. Jeżeli skończyłeś sprzątać:  
    1. Zapisz wartość swojego zegara lamporta
    2. Wyślij do siebie informację o zakończeniu sprzątania
12. Wróć do 4.
