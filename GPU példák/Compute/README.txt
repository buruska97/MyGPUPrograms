Ez a projekt a OpenGL által felkínált általános GPU-n történő számolásokat használja, ezek a compute árnyalók. 
Ennek segítségével végez egy fizikai részecskeszimulációt. Minden részecskére külön-külön párhuzamosítva számoljuk ki, hogy a következő időpillanatban hol kell lennie.


A fizikai szimulációt egy 64x64 részecskéből álló "rongydarabon" végzi el. A "rongydarab" ráesik egy virtuális körre, majd onnan lecsúszik és leesik.
A szimuláció során 3 fizikai erőt szimulálunk különböző compute árnyalókban.
Az egyik ilyen erő a gravitáció, minden részecskére ha a lefele ható gravitációs erő, amely lefele gyorsítja őket (ezt a gravity.comp valósítja meg)
A másik ilyen erő a gömbön történő alátámasztási erő, ez eltolja a gömbtől a részecskéket. (ezt a collision.comp valósítja meg)
A harmadik ilyen erő rongy részecskéi között levő rugóerő. Minden részecske szeretné megtartani az eredeti távolságát a szomszédjaitól, rugószerűen gyorsul oly módon, hogy az eredeti távolságra kerüljön. (ezt a distance.comp valósítja meg)
Végül a hatékonyság érdekében a végső pozíciók meghatározását is compute árnyalóban végezzük, ez update-eli a részecskék aktuális pozícióját. (ezt a finalUpdate.comp valósítja meg)

Ebben a projektben a vertex és fragmes árnyalók csak a megjelenítésért felelnek.

A CPU oldali program, a compute shaderek irányítását, azokból és azokba történő adatmásolást végzi el. Létrehozza a részecskéket, majd az OpenGL API hívások segítségével megjeleníti a részecskéket a képernyőn.

A compute shaderek nagy előnye, hogy ki tudják használni a GPU által nyújtott feladatpárhuzamosítási és számolási lehetőségeket.