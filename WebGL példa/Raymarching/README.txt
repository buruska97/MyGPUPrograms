Jelen projekt egy Kotlin nyelven írt WebGL-t felhasználó program. (A kotlin Javascriptre fordul és böngészőben fut)

A web könyvtárban található index.html-t böngészőben megnyitva elindul a program. (Engedélyezni kell a lokális fájlok betöltését a böngésző indításakor vagy egy lokális webszerven kell futtatni, a CORS policy miatt)
Ezt például Google Chromeban úgy érhetjük el, hogy parancssoros indítás (vagy shortcuton keresztül) során használjuk a --allow-file-access-from-files kapcsolót. A projektben levő google chorme shortcut ezt végzi el, amennyiben az alapértelmezett helyre van telelpítve a google chrome.

A program a teljes canvason egy voxelrács néhány különböző megjelnítési módját valósítja meg. A mi példánkban a voxelrács egy agyról készített 256x256x256-os felvétel. Minél világosabb egy voxel, annál sűrűbb abban a pontban levő anyag.
Ezt mint szintfelületet értelmezzük egy bizonyos sűrűséghatárnál, ezt jelenítjük meg.
Minden képernyőn levő pixelre kiszámoljuk, hogy a kamerából azon keresztül kilőtt virtuális sugarunk hol metszi a szintfelületünket (raymarching).

A raymarchingot a hatékonyság érdekében bináris kereséssel gyorsítjuk.
Mivel a szintfelület normálvektorát nem ismerjük ezeket is ki kell számoljuk, ezt úgy valósítjuk meg, hogy a kamera fölött és mellett is kilövünk egy-egy sugarat, így kapunk két merőleges felületen fekvő irányvektort (pontosabban ennek közelítőjét), ezek vektoriális szorzatának normalizáltja egy normálvektor.

A három megjelenítés ezután már kicsit másképpen működik.
Az első (trace-fs.glsl) az egyszerű Phong árnyalást valósít meg, egy beeső fény esetén. Minden szintfelületi ponton megvizsgálja, hogy a fény irányában van-e másik pont amelyet a fény már megvilágít (raymarch a fény irányába), így elérjük, hogy a felület önmagára árnyékot vessen.
A második (matcap-fs.glsl) Material Capture technológiát használ, amely egy textúrát tölt be (egyszerű kép), majd a felület színét egyszerűen a normálvektor és a textúra alapján határozza meg. Ez egy hatékony módszer egy szintfelület kiszínezésére, ha nem szükséges magas minőségű megvilágítást szimulálni.
A harmadik (layered-fs.glsl) egyszerre két különböző sűrűséghatáron levő szintfelületet jelenít meg. Úgy hogy mindkét felület egy-két karakterisztikáját ki lehessen venni. Ezt úgy valósítjuk meg, hogy aszerint színezzük ki a felületet, hogy mekkora szöget zár be a normálvektor a nézeti szögből. Ez minél nagyobb, annál kevésbé áltlátszó a pixel, így mindkét felületnek a kontúrját emeljük ki leginkább.

A programban a kamerát a WASD, QE ogmbokkal tudjuk mozgatni, illetve az egérkattintás utáni egérmozgással tudjuk forgatni. (Roll forgatást nem tudok végezni) A megjelenítési módok között a J,K,L gombokkal lehet váltogatni.