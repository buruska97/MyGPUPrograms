Ez a cuda programrész egy olyan függvényt (cudaCompactEven) valósít meg, amely kap egy egész számokból álló tömböt, kiválogatja a páros számokat, majd visszaadja ezen páros számok tömbjét.

Ezt a lehető legnagyobb párhuzamosítás segítségével a GPU-n teszi meg.
Először végrehajtunk egy map műveletet, vagyis létrehozunk egy ugyanakkora tömböt, amelyen minden indexen 1 van ha az eredeti tömbben páros szám volt.
Ezután a map eredményén lefuttatunk lefuttatunk egy scan műveletet, vagy kiszámoljuk minden indexnél, a 0-tól az indexig levő elemek összegét.
Ezt minél nagyobb párhuzamosítás érdekében az upsweep és downsweep módszerrel csináljuk, így lineáris idő helyett logaritmikus időben lefut. Az alapötlet, hogy részösszegekből bináris fákat építünk.
Miután megvan a scan eredmény, a compact_final függvény létrehozza az eredményt a map és scan részeredményeiből. A map eredményéből tudjuk, hogy az adott elemet, átmásoljuk-e vagy nem, a scan eredményéből pedig tudjuk, hogy hányas indexre kell az eredménytömbben írni az eredményt. Így a másolás művelet is teljesen párhuzamosítható.

Sajnos a CUDA-nak egy tulajdonsága, hogy NVidia fejlesztés, ezért nem fut le alapvetően AMD kártyákon. Mivel a saját PC-mben AMD GPU van ezért, ezt a saját gépemen nem tudom bemutatni működés közben.