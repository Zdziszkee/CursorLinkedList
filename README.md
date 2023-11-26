Pytania
1. Jakie są zalety implementacji wskaźnikowej, a jakie implementacji tablicowej?
    
    -Liste wskaznikowa nie ma ograniczenia liczby elementow - mozemy dodawac elementy bez koniecznosci kopiowania ich do nowej tablicy
    -Lista tablicowa umozliwia szybki dostep po indexie w czasie O(1)
    -wstawianie elementow w linkedliscie w dane miejsce jest O(1)
2. Jakie są zalety implementacji kursorowej?
    -implementacja kursorowa jest pomocna jezeli pracujemy w jezyku nie wspierajacym wskaznikow np Fortran

Kompilacja: make all
Uruchamianie ./ArrayList.x, ./CursorList.x