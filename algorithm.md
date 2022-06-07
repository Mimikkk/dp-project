## Poeci:

1. Jeśli brałeś udział w libacji (odmawiaj na każde zaproszenie do koła):
    1. Oczekuj na wiadomość o sprzątaniu pokoju
    2. Odpoczywaj przez losową liczbę sekund
2. Utwórz koło naukowe z prawdopodobieństwem
3. Jeżeli jesteś twórcą koła (odmawiaj na każde zaproszenie do koła):
    1. Poinformuj resztę poetów o istnieniu koła
    2. Oczekuj na decyzję o dołączeniu od reszty poetów
    3. Dokonaj decyzji tego, co przyniesiesz, na podstawie swojej historii nadając priorytet alkoholowi i zagrysze
    4. Rozpocznij wysyłanie listy uczestników i listy decyzji z załączoną swoją decyzją do następnego uczestnika z listy
    5. Oczekuj na wiadomość od ostatniego uczestnika z listy
    6. Stwierdź czy dalsze działanie koła jest możliwe (czy jest zagrycha i alkohol)
    7. Jeśli na liście decyzji występuje alkohol oraz zagrycha to:
        1. Poinformuj członków koła, że pora zacząć twórczą działalność
        2. Poinformuj losowego wolontariusza o potrzebie posprzątania pokoju
        3. Oczekuj na odpowiedź od wolontariusza
        4. Poinformuj członków koła o sprzątaniu pokoju
    8. W przeciwnym wypadku:
        1. Poinformuj członków koła o rozwiązaniu koła
4. W przeciwnym wypadku:
    1. **Oczekuj na propozycję dołączenia do koła**
    2. Dokonaj decyzji o dołączeniu do koła z prawdopodobieństwem
    3. Jeśli odmawiasz:
        1. Wróć do 1.
    4. Jeśli akceptujesz (odmawiaj na każde zaproszenie do koła):
        1. Oczekuj na listę uczestników i listę decyzji
        2. Dokonaj decyzji tego, co przyniesiesz na podstawie swojej historii i otrzymanej listy decyzji nadając
           priorytet alkoholowi i zagrysze
        3. Wyślij obie listy do następnego uczestnika z listy
        4. Oczekuj na informację o rozpoczęciu libacji
5. Wróć do 1.

<div style="page-break-before: always;"></div>

## Wolontariusze:

1. Utwórz listę pokojów czekających na sprzątanie
2. Ustaw swój zegar Lamport'a na swój identyfikator procesu
3. Utwórz początkową listę par (wartość zegara Lamport'a, id wolontariusza), 
     w postaci [(0, 0), (1, 1), (2, 2), ..., (n-1, n-1)]
4. Czekaj na zdarzenie
5. Jeżeli otrzymałeś wiadomość z prośbą o sprzątanie od poety:
    1. Umieść identyfikator poety na końcu listy pokojów
    2. Jeżeli długość listy pokojów jest równa 1:
        1. Przejdź do uzgadniania
6. Jeżeli otrzymałeś wiadomość z informacją o rozpoczęciu sprzątania przez wolontariusza:
    1. Usuń z listy wolontariuszy wolontariusza, którego id jest równe id nadawcy
    2. Jeżeli id nadawcy jest równe zapisanemu id wolontariusza:
        1. Jeżeli pokój sprzątany przez nadawcę jest pierwszym pokojem z listy pokojów:
            1. Usuń pierwszy element z listy pokojów
            2. Usuń zapisane id wolontariusza
        2. Jeżeli lista pokojów jest niepusta:
            1. Przejdź do uzgadniania
7. Jeżeli otrzymałeś wiadomość z informacją o zakończeniu sprzątania przez wolontariusza:
    1. Dodaj do listy wolontariuszy parę (wartość zegara Lamport'a nadawcy, id nadawcy)
    2. Posortuj rosnąco listę wolontariuszy
8. Jeżeli otrzymałeś wiadomość z prośbą o sprzątanie od wolontariusza i jesteś wolny:
    1. Wyślij informację o rozpoczęciu sprzątania, z załączonym id pokoju, do wszystkich wolontariuszy
    2. Zacznij sprzątać
    3. Jeżeli licznik odmów jest większy od stałej:
        1. Usuń pierwszy element z listy pokojów
        2. Zacznij sprzątać
        3. Jeżeli lista pokojów jest niepusta:
             1. Przejdź do uzgadniania
    4. Zapisz wartość swojego zegara Lamport'a
    5. Wyślij informację o zakończeniu sprzątania, 
         razem z zapisaną wartością zegara Lamport'a, do wszystkich wolontariuszy
9. Wróć do 4.

uzgadnianie:
1. Jeżeli id wolontariusza jest ustawione:
   1. Usuń zapisane id wolontariusza
   2. Inkrementuj licznik odmów
   3. Jeżeli licznik odmów jest większy od stałej:
      1. Wróć do 4.
2. W przeciwnym wypadku:
   1. Zresetuj licznik odmów
3. Zapisz id wolontariusza z początku listy wolontariuszy
4. Poinformuj wolontariusza, którego id zapisałeś, o potrzebie sprzątania pokoju z początku listy pokojów