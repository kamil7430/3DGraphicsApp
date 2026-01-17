Aplikacja jest kontunuacją aplikacji rozpoczętej na laboartorium, ma pokazywać prostą scenę składającą się kul, sześcianów itp., ale można też (a nawet warto) wczytać złożone modele 3D w popularnych formatach (OBJ, FBX, 3DS, itp.). Można wykorzystać bibliotekę do wczytywania modeli 3D (np. Assimp).

W każdym projekcie:

- [x] jeden obiekt poruszający się (przesuwanie + obroty)
- [x] kilka stałych obiektów. Jeden z nich gładki — kula, torus lub powierzchnia Beziera
- [x] Jeśli nie ustalono inaczej, w każdym projekcie min. 3 kamery (przełączanie):
  - [x] nieruchoma obserwująca scenę
  - [x] nieruchoma śledząca ruchomy obiekt
  - [x] związana z ruchomym obiektem (FPP — First Person Perspective lub TPP — Third Person Perspective)
- [x] W każdym projekcie kilka źródeł światła (min. 3):
  - [x] min. jeden reflektor na poruszającym się obiekcie (np. światła samochodu)
  - [x] musi istnieć możliwość zmiany (ręcznej) względnego kierunku świecenia reflektora/ów umieszczonego na obiekcie ruchomym
  - [x] min. jedno stałe (nieporuszające się) źródło światła (punktowe lub reflektor)
- [x] rzutowanie perspektywiczne
- [x] model cieniowania Phonga (Interpolacja wektorów normalnych)
- [x] mgła — płynna zmiana
- [ ] noc/dzień
- [ ] "zanikanie" światła wraz z odległością

Dla mojego zakresu nazwisk:

- [x] model oświetlenia liczony jest w układzie kamery

Dodatkowo nalezy uzupełnić projekt o:

- [ ] Lustro na scenie (na ruchomym obiekcie)
- [ ] możliwość zmiany składowej zwierciadlanej Phong/Blinn
- [ ] krótka dokumentacja dotycząca zaimplementowanej indywidualnej funkcjonalności

Proszę pamiętać o:

- Poprawnym potoku renderowania pod kątem transformacji
- Macierze modelu dla każdego obiektu, macierze widoku dla każdej z kamer, macierz rzutowania
- Poprawnym przekształcaniu wektora normalnego (innym niż przekształcanie punktów!)
- Projekty należy oddawać osobiście.