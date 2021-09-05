# TrackJet (cz)
TrackJet je otevřená platforma robotického vozidla vyvinutá pro [Letní technický tábor 2021](http://wp.vedatori.com/pobytove-tabory/) organizovaný spolkem [Vědátoři](http://wp.vedatori.com/). Jedná se o nástupní verzi vozidla [TrackRay](https://github.com/vedatori/TrackRay), ve které byly vylepšeny jeho terénní schopnosti a přidány nové funkcionality. Byla redesignována mehanická konstrukce i řídicí elektronika, pro kterou byl vyvinut odpovídající software. 

Hlavní mechanickou částí je 3D tištěné šasi, které je doplněno dalšími 3D tištěnými díly. Plošné díly jsou laserem vyřezány z překližky. Spojování je prováděno především šrouby M3. Je použito minimum lepených spojů tak, aby bylo možné vrátit jakýkoliv krok při stavbě. Obsahuje zadní pohonná kola a na každé straně dále tři kola pro vedení pásu. Všechna kola jsou uložena pevně. Pásy jsou vytištěny na 3D tiskárně tak, aby se již při tisku vytvořilo kloubové spojení jednotlivých segmentů, což vede ke snížení času potřebného pro stavbu.

Veškerá elektronika je umístěna na jedné DPS. Ze spodní strany je připevněn zdroj energie - akumulátor 18650. Jeho nabíjení je řešeno na DPS, pro nabití není nutné ho vyjmout. Robota řídí ESP32 připájené přímo do DPS. Pro orientaci v prostoru robot využívá snímání pohybu pásů pomocí optických enkodérů a senzoru LiDAR umístěného na otočné konzoli. Vstupy od uživatele je možné zadat pomocí enkodéru, potenciometru a tlačítka, výstupy jsou zobrazeny na LED matici, bzučákem nebo jiných LED. Programování a nabíjení robota je prováděno přes USB-C.

Software je připraven na výuku programování. Tomu je přizpůsobena knihovna, pomocí které uživatelé jednoduše ovládají periferie robota bez nutnosti nízkoúrovňového programování jednotlivých částí. Pro výuku programování je připraven také návod popisující jednotlivé funkce a příklady jejich použití. Vše bylo navrženo s důrazem na rozvoj schopností algoritmizace a programování účastníků.
Součástí softwarového balíku je i kompletní program pro dálkové ovládání přes WiFi. Vozidlo umožňuje komunikaci jak přes vlastní WiFi, tak přihlášení k již existující síti. To mu umožňuje jízdu i v širém prostoru. 

## Licence
* Obsah tohoto repozitáře mimo složku *Software* je sdílen pod licencí [Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)](https://creativecommons.org/licenses/by-nc-sa/4.0/).
* Obsah složky *Software* je sdílen pod licencí [GNU Lesser General Public License v3.0](https://www.gnu.org/licenses/lgpl-3.0.en.html).

# TrackJet (en)

TrackJet is an open-source robotic vehicle platform developed for [Summer technical camp 2021 (cz)](http://wp.vedatori.com/pobytove-tabory/) organised by [Vědátoři (cz)](http://wp.vedatori.com/) group. It is succesor of [TrackRay](https://github.com/vedatori/TrackRay) robotic vehicle. TrackJet introduces enhanced terrain abilities and new functionalities. Mechanical parts, electronics and software were redesigned.

TrackJet consists of main 3D printed part - chassis, several other 3D printed parts and laser cut plywood parts. M3 bolts are used in assembly process, what grants possibility to undo every step in case of assembly error. Rear propulsion wheel and three guiding wheels are placed on every side of the vehichle. Tracks are designed to be 3D printed in one piece with joints between segments which minimises assembly time.

All electonic components are placed on a single PCB. This board contains 18650 battery and a charger circuit so it is not needed to take battery out to charge it. ESP32 soldered to the PCB is used for peripherials control and WiFi connectivity. Spacial orientation of TrackJet is achieved using LiDAR sensor and optical encoders. User inputs can be made using encoder, potentiometer and button. LED matrix, buzzer and other LEDs are used for communication with user. Vehicle is programmed and charged via USB-C.

Electronics and software is designed for programming teaching. TrackJet contains library which allows easy peripherials control without need of low-level programming abilities. Repository contains programming manual (in czcech only) with description of commands and examples.
WiFi remote control is part of software pack in this repository. ESP32 can be connected to external WiFi or it can create its own network so it is possible to drive outdoor without network coverage. 

## License
* The contents of this repository excluding folder *Software* is shared under license [Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)](https://creativecommons.org/licenses/by-nc-sa/4.0/).
* The contents of the folder *Software* is shared under license [GNU Lesser General Public License v3.0](https://www.gnu.org/licenses/lgpl-3.0.en.html).

![alt](Manuals/SupportFiles/IMG_7559-min.jpg)

# TrackJet - Fork
This is a fork of [vedatori/TrackJet](https://github.com/vedatori/TrackJet)
