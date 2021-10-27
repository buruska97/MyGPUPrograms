Ez a projekt a geometria árnyalót használja fel arra, hogy az OpenGL pipeline-ban egy tört vonalat folytonos vonalként jelenítsen meg. (Egy Catmul Rom spline-t készít)
Megjegyzés: Mivel egy sajátos Catmul Spline görbét jelenítünk meg, ezért a súlyozást felgyorsítsuk azzal, hogy mátrixszorzás segítségével végezzük el.

A geometria árnyaló (geometry.geom), a vertex árnyalótól megkapja a görbe kontrolpontjait (mint Vertexek). Ezekből további vertexeket generál, amelyek segítségével a fragmens shader már folytonos görbét jelenít meg. (Természetesen az fragmen shader által kapott görbe matematikai értelemben nem folytonos, de lényegeseb több vertexből áll).

A CPU oldali kód, az egérkattintások helyére berak egy kontorlpontot, majd a kontrolpontok listájával meghívja az OpenGL API-t, amely ezután kirajzolja a görbét.