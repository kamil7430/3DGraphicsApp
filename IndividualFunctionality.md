## Lustro na scenie

Zaimplementowanie lustra na scenie to proces, który składa się z kilku etapów.

### Etap 1

Najpierw należało wskazać OpenGL-owi, że będzie potrzebny *Stencil Buffer*. Jest to bufor, który pozwala rysować tylko po wybranych pikselach na ekranie. Piksele te wskazujemy, po prostu rysując w odpowiednim trybie powierzchnię lustra.

### Etap 2

Następnie, mając już zainicjowany *Stencil Buffer*, tworzymy macierz widoku, która jest lustrzanym odbiciem **względem punktu, w którym jest lustro**. Włączony *Stencil Buffer* działa w tym przypadku jak filtr, który pozwala rysować tylko po pikselach, które są na powierzchni lustra. Rysujemy scenę, korzystając z wspomnianej macierzy widoku.

### Etap 3

W tym momencie mamy już narysowaną odbitą scenę. Pozostaje jedynie wyłączyć *Stencil Buffer* i narysować scenę jeszcze raz, tym razem *normalnie*, tzn. wykorzystując pierwotną macierz widoku.

## Suwak do zmiany składowej zwierciadlanej

Dodanie tej funkcjonalności sprowadziło się do dodania suwaka (dostępnego w bibliotece `ImGui`), a następnie przekazania zmiennej do uniforma w fragment shaderze. Jako, że uniform ten jest aktualizowany przed wyrenderowaniem każdej klatki, wartość jest zawsze aktualna.