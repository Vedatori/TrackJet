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
Napište program, který bude blikat libovolnou LED a rychlost tohoto blikání bude nastavitelná pomocí natočení potenciometru. Nejvyšší perioda blikání bude 1s, nejvyšší bude 50ms. 

# Lekce 4
Roztočíme motory a ukážeme si podmínku "if".

## Úkol 7 - Otáčení motoru dopředu a dozadu
Napiště program, který bude pomocí potenciometru ovládat jeden z motorů tak, aby mohl jezdit na obě strany. V prostřední poloze potenciometru bude tedy motor stát.

## Úkol 8 - Jízda vpřed/vzad
Napište program, který bude jezdit s TrackJet dopředu a dozadu. Doba jízdy dopředu je 1 s, dozadu taky 1 s. Rychlost je libovolná nenulová.

# Lekce 5
TrackJet začne komunikovat s počítačem pomocí sériové linky.

# Lekce 6
Přečteme stav rotačního enkodéru a rozpohybujeme servomotory.

# Lekce 7
Rozblikáme LED bez použití příkazu delay(). Vytvoříme vlastní funkci.
