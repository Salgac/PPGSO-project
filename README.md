# PPGSO Projekt - tím "Myslím že to nemusíme až tak hrotit 😃"

Repozitar urceny pre vypracovanie projektu na temu "Príroda". 

Dokumentacia zameru projektu pri prvom odovzdani je dostupna [tu](https://www.dropbox.com/s/smvd19464vug8hf/PPGSO%20-%20Odovzdanie%201.pdf?dl=1 "Odovzdanie 1").

Repozitar vychadza z implementacie ulohy "task5_3d_origin", vypracovaneho pocas cvicenia z repozitaru [ppgso](https://github.com/kapecp/ppgso)

## Ukazka

Scena 1: \
![scena 1](img/scene1.png)

Scena 2: \
![scena 1](img/scene2.png)

## Graficke demo

Viac nez dvoj minutove video je k dispozicii k nahliadnutiu [tu](https://www.dropbox.com/s/eocod65c46ksq0y/video.mp4?dl=0).

## Spustenie

Pre spustenie v systeme Linux staci spustit script ["run.sh"](run.sh).

## Body zadania


| Uloha                                                 |                                                                                                                                       | Body  |         |
| ----------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------- | ----- | ------- |
| **Grafické demo**                                     |                                                                                                                                       |       |         |
|                                                       | ○ Logický začiatok, priebeh a koniec                                                                                                  | 1     | &#9745; |
|                                                       | ○ Trvanie aspoň 2 minúty, pričom demo nemôže byť “statické”                                                                           | 1     | &#9745; |
| **3D objekty a mapovanie textúr na 3D geometriu**     |                                                                                                                                       |       |         |
|                                                       | ○ Objekty ako unikátne 3D polygonálne modely                                                                                          | 2     | &#9745; |
|                                                       | ○ Unikátne textúrovanie pomocou UV koordinátov                                                                                        | 2     | &#9745; |
| **Scény**                                             |                                                                                                                                       |       |         |
|                                                       | ○ Každá scéna bude mať logické priestorové usporiadanie (podlaha, pozadie, obloha, strop, steny...)                                   | 1     | &#9745; |
|                                                       | ○ Dynamická scéna s objektami, ktoré sú vytvárané a zanikajú v čase (Aspoň 2 rôzne typy objektov, ktoré vznikajú-zanikajú)            | 1     | &#9745; |
|                                                       | ○ Procedurálne generovaná scéna (Obmedzenia a lokácia objektov definovaná (ne-)deterministickým algoritmom)                           | 2     | &#9745; |
|                                                       | ○ Zmena scény a viacero virtuálnych oblastí (Aspoň 2 rôzne scény, môžu byť riešené v rámci jednej sceny napr. vonku-vnútri)           | 2     | &#9745; |
| **Transformácie a animácie objektov**                 |                                                                                                                                       |       |         |
|                                                       | ○ Procedurálna animácia (Uzavretá metóda s parametrami, Logické vetvenie)                                                             | 2     | &#9745; |
|                                                       | ○ Hierarchická transformácia objektov (Aspoň 2 levely hierarchie medzi 3-mi objektmi, Použitie kompozície maticových transformácii)   | 2     | &#9745; |
|                                                       | ○ Animácia na základe dát uložených v kľúčových snímkoch a interpolácie                                                               | 3     | &#9744; |
| **Simulácia**                                         |                                                                                                                                       |       |         |
|                                                       | ○ Efektívna kolízia medzi objektami (Dynamická odozva na kolíziu)                                                                     | 3     | &#9745; |
|                                                       | ○ Simulácia aspoň s dvoma silami s použitím vektorovej algebry (Napr. gravitácia + vietor, časticový systém)                          | 2     | &#9745; |
| **Transformácie kamery**                              |                                                                                                                                       |       |         |
|                                                       | ○ Kamera s perspektívnou projekciou                                                                                                   | 1     | &#9745; |
|                                                       | ○ Animovaná kamera pomocou kľúčových snímkov alebo riadená animačnými krivkami                                                        | 3     | &#9745; |
| **Osvetlenie za pomoci viacerých svetelných zdrojov** |                                                                                                                                       |       |         |
|                                                       | ○ Zmena pozície zdrojov svetla a odtieňov farby osvetlenia                                                                            | 1     | &#9745; |
|                                                       | ○ Správne kombinovať difúzne svetlo z 2 zdrojov svetla s difúznymi materiálmi                                                         | 2     | &#9745; |
|                                                       | ○ Správny Phongov osvetlovací model s viacerými (aspoň 3) zdrojmi svetla, pričom musia byť splnené podmienky                          | 2     | &#9745; |
|                                                       | ○ Tiene implementované (Jednoduchým spôsobom / pomocou shadow-maps)                                                                   | 1/3   | &#9744; |
| **Post-processing**                                   |                                                                                                                                       |       |         |
|                                                       | ○ Zobrazenie scény cez vlastný framebuffer, pomocou ktorého sa aplikuje filter: základný (grayscale) / konvolučný / pokročilý (bloom) | 1/2/4 | &#9745; |
|                                                       |                                                                                                                                       |       |         |
|                                                       |                                                                                                                                       | 40    | 32      |

## Poznamky ku vypracovaniu

Pre zjednodusenie bodovania su tu uvedene niektore priklady ku bodom zadania:

- Graficke demo: 	dostupne v repozitari, scenar podla specifikacie v prvom odovzdani.
- 3D objekty :		objekt hraca ma unikatny polygonialny model, spolu s texturou pomocou UV koordinatov
- Sceny: 			dynamicke objekty su svetlusky a vrany, stromy su v scenach proceduralne generovane
- Transformacie:	body v zadani boli implementovane na zvieratach
- Simulacia:		kolizia pri vlkoch, na hraca posobi gravitacia a vietor v skoku
- Kamera:			animacia kamery cez klucove snimky sluzi na zobrazenie celej sceny
- Osvetlenie:		svetlusky sluzia ako menitelny zdroj svetla, cela scena osvetlena pomocou 3 svetiel
- Post-procesing:	implementovany konvolucny filter