V tomto dokumentu se nachází rozpis programovacích lekcí a zadání programovacích úkolů. 

**Programovací lekce** nejsou povinné, ale jsou pro začátečníky velmi vhodné. Zájem o lekci budete vyjadřovat tak, že na svůj řádek v tabulce připíšete na konec číslo lekce, o kterou máte zájem. Jakmile jste lekci absolvovali, číslo zakroužkujete tak, aby bylo jasné, které lekce už jste absolvovali. Účast na lekci je ohodnocena 1 Chotěcoinem,

**Programovací úkoly** jsou úkoly, které budete řešit samostatně. Ke každému úkolu potřebujete soubor znalostí, který je obsažen v některých z lekcí. Úkoly je však možné řešit i jen pomocí programovacího manuálu (pro pokročilé). Splnění úkolu bude kontrolovat vedoucí a odmění ho 2 Chotěcoiny. Po splnění úkolu si v tabulce na vašem řádku zaznačte křížkem jeho splnění. 

# Lekce 0
Zprovozníme programovací prostředí VS Code + PlatformIO tak, abyste mohli nahrát kód do TrckJet z vašeho PC.

# Lekce 1
Rozblikáme LED pod panelem na TrackJet.

## Úkol 1 - Blikání LED
Napište program, který bude rychle blikat zelenou LED (LED1, vpravo pod panelem). Doba svícení je 200 ms. Doba zhasnutí je 200 ms.

## Úkol 2 - Záblesky LED
Napište program, který každou vteřinu krátce zableskne LED1. Délka svícení je 50 ms. LED se zableskne každou vteřinu.

## Úkol 3 - Železniční přejezd
Napište program, který bude blikat jako železniční přejezd. Při blikání se střídají alespoň 2 různé světla **na LED panelu**. Délka svícení je volitelná.

# Lekce 2
Přečteme stav tlačítka, ukážeme si binární proměnnou.

## Úkol 4 - Manuální blikání LED
Napište program, který bliká libovolnou LED pouze při stisknutém tlačítku SW2. Doba svícení je 200 ms. Doba zhasnutí je 200 ms.

## Úkol 5 - Stabilní stav LED
Napište program, který bude tlačítkem zapínat a vypínat libovolnou LED.
Po stisku tlačítka SW2 zůstane LED zapnutá. Po opětovném zmáčknutí tačítka se LED vypne.

# Lekce 3
Přečteme stav potenciometru, ukážeme si číselnou proměnnou a nastavíme jas LED.

## Úkol 6 - Rychlost blikání
Napište program, který bude blikat libovolnou LED a rychlost tohoto blikání bude nastavitelná pomocí natočení potenciometru.

# Lekce 4
Roztočíme motory a ukážeme si podmínku "if".

## Úkol 7 - Otáčení motoru dopředu a dozadu
Napiště program, který bude pomocí potenciometru ovládat jeden z motorů tak, aby mohl jezdit na obě strany. V prostřední poloze potenciometru bude tedy motor stát.

## Úkol 8 - Jízda vpřed/vzad
Napište program, který bude jezdit s TrackJet dopředu a dozadu. Doba jízdy dopředu je 1 s, dozadu taky 1 s. Rychlost je libovolná nenulová.

## Úkol 9 - Nastavení jasu LED po stisku tlačítka
Napište program, který nastavý jas libovolné LED podle polohy potenciometru. Nastavení jasu proběhne **jen** po stisku tlačítka SW2. Když není zmáčknuto, je jas stabilní.

# Lekce 5
TrackJet začne komunikovat s počítačem pomocí sériové linky.

## Úkol 10 - Vypsání hodnoty potenciommetru
Napiště program, který bude na sériovou linku vypisovat hodnotu natočení potenciometru.

## Úkol 11 - Vysání času od startu TrackJet bez funkce millis()
Napište program, který bude vypisovat celočíselnou proměnnou každou vteřinu. Tuto proměnnou však každou milisekundu zvyšte o 1. Je chování tohoto programu stejné jako v případě použití funkce `millis()`?

# Lekce 6
Přečteme stav rotačního enkodéru a rozpohybujeme servomotory.

## Úkol 11 - Vypsání stavu enkodéru
Napište program, který na sériovku vypíše akutální hodnotu rotačního enkodéru. Po stisku tlačítku tuto hodnotu resetujte do nuly.

## Úkol 12 - Nastavení polohy serva podle potenciometru
Napište program, který přečte polohu potenciometru a podle ní nastaví polohu libovolného serva.

## Úkol 13 - Mávání servem
Napište program, který nastaví rychlost serva 2 (LIDAR) na **polovinu** jeho maximální rychlosti. Touto rychlostí jezděte se servem periodicky mezi krajmíni polohami.

# Lekce 7
Budeme měřit vzdálenost mezi překážkou a TrackJet pomocí senzoru LIDAR.

## Úkol 14 - Parkovací senzory
Napište program, který bude na sériovou linku posílat sekvenci znaků tak dlouhou, kolik desítek centimetrů je mezi LIDAR senzorem a překážkou. Např. při měření vzdálenosti 40 cm bude program vypisovat na jedden řádek eskvenci `####`. 

## Úkol 15 - Držení si odstupu
Napište program, který zajistí, že si TrackJet bude udržovat odstup od překážky (ruky programátora) na vzdálenost 30 cm. Když je vzdálenost větší, pojede dopředu a když menší, bude couvat. Pozor, aby nezačal kmitat, nemusí být udržovaná vzdálenost úplně přesná.

# Lekce 8
Rozezníme bzučák na TrackJet a ukážeme si jak nastavit jeho frekvenci na konkrétní tóny.

## Úkol 16 - Hraní melodie
Napište program, který zahraje vaši oblíbenou melodii (oblíbená píseň, film, opera) nebo jakoukoliv jinou melodii o alespoň 5 notách.

## Úkol 17 - Klakson
Napište program, který *zatroubí* bzučákem v případě, že je čtení vzdálenosti z LIDARu menší než 5 cm.

## Úkol 18- Theremin
Napište program, který bude číst vzdálenost od překážky z LIDARu a podle této vzdálenosti bude nastavovat frekvenci bzučáku. Zahrejte vedoucímu hezkou melodii.

# Lekce 9
Ukážeme si spuštění WiFi knihovny a použití vzdáleného příkazového řádku.

## Úkol 19 - Vzdálené měření vzdálenosti
Napište program, který po zadání příkazu "dist" do vzdáleného příkazového řádku vrátí měřenou vzdálenost od překážky LIDARem.

# Lekce 10
Rozblikáme LED bez použití příkazu delay(). Ukážeme si cyklus.

## Úkol 20 - Vlna
Napište program, který bude postupně vyblikávat všechny LEDky v jednom řádku LED panelu. Vždy svítí jen jedna LED.

## Úkol 21 - K.I.T.T.
Napište program, který bude postupně vyblikávat všechny LEDky v jednom řádku LED panelu. Vlna se bude symstricky zpět. Vždy svítí jen jedna LED. Blikání bude připomínat vzor na [slavném inteligentním autě K.I.T.T.](https://www.youtube.com/watch?v=oNyXYPhnUIs&ab_channel=NBCClassics).

# Lekce 11
Změříme ujetou vzdálenost pomocí *enkodérů*. Ty měří pohyb pásů. K úspěšnému měření je třeba enkodéry zkalibrovat.

## Úkol 22 - Změření metru
Napište program, který po zmáčknutí tlačítka rozjede TrackJet směrem dopředu libovolnou rychlostí. Po ujetí 1 metru se TrackJet zastaví.

## Úkol 23 - Autonomní jízda
Napište program, který bude s TrackJet jezdit po prostoru. Při detekování překážky LIDARem se TrackJet zastaví. Otočí a pokračuje v jízdě.

## Úkol 24 - Autonomní jízda s rozhlížením
Napište program, který vylepší autonomní jízdu z minulého úkolu o *rozlížení*. Při detekování překážky TrackJet zastaví, rozhlédne se do stran pomocí serva natáčejího LIDAR senzor. Program vybere nejvhodnější směr pro další jízdu. Tam se TrackJet otočí a rozjede.

## Úkol 25 - Otočení o zadaný úhel
Napište program, který bude číst kroky otočení rotačního enkodéru a podle nich bude na LED panelu zobrazovat linku svítících LED. Tato linka bude vždy začínat ve středu panelu a může být natočena v 8 možných úhlech (0, 45, 90, 135, 180, 225, 270, 315)°. Po stisku tlačítka enkodéru se TrackJet začne otáčet. Otočí se o zadaný úhel co nejpřesněji.