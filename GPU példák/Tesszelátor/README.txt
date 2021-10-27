Ez a projekt a tesszellátor árnyalók működését és egy lehetséges felhasználását mutatja be.

A CPU oldalon egyszer létrehozunk egy 10x10-es négyzetrácsot (egy négyzet két háromszögből áll).
A tesszellátor árnyalókat arra használjuk, hogy ezt a négyzetrácsot a kamerához közelebb levő pontokban további háromszögekre bontsuk fel. Ezzel javíthatunk a megjelenítés minőségén. Majd a részletesebb hálót egy hullámfelületként jelenítjük meg.
A CPU uniform paraméterként jelzi a GPU-nak, hogy mekkora felbontást szeretne a legközelebbi, illetve legtávolabbitávoli háromszögeken.
A pipeline-ban először a Tesselation Control árnyaló fut le (tess.tc). Az előbb említett uniform paramétereket interpolálva beállítja az egyes háromszögekre, hogy mekkora további felbontást (tesszellációt) hajtson végre a GPU.
Miután minden háromszögre tudjuk, hogy mennyire kell felbontani, ezt a lépést elvégzi a GPU, ezután fut le a Tesselation Evaluation árnyaló (tess.te)
Ennek az árnyalónak a feladata, hogy meghatározza az újonnan létrejött vertexeknek mi legyen az új pozíciója. Mivel szeretnék egy hullámfelületet megjelníteni ezért a Z irányba is elmozdítjuk ezeket az újonnan létrejött vertexeket.
Az általáunk megjelenített hullámfelület a sin((x+y)*10)/10. A mellékelt képen látszik, hogy a közeli felület sokkal simább, míg a kamerától távoli rész kevésbé részletes.

A tesszellátor árnyalók egyik lényege felhasználási lehetősége, hogy úgy tudjuk egy felület felbontását növelni, hogy a CPU kevés vertex-et kezel.

a CPU oldali kód létrehozza az alap rácsot, létrehozza az MVP mátrixot, hogy a felületre egy kis szögből nézzünk rá. Továbbá a GPU számára fontos paramétereket beállítja a felhasználói inputok alapján.

