Treball final de l'assignatura d'eines informàtiques per les matemètiques
Universitat Autònoma de Barcelona, curs 2021-22
A revisar i millorar

Programa escrit en c que codifica i descodifica (corregint errors) fitxers de tipus .dat, .txt i .bin
utilitza l'algorisme de Reed-Solomon i de Berlekamp-Welch

per defecte, els valors de p i de k, són 257 i 200 respectivament
p ha de ser un primer, treballarem a Z/pZ, és a dir, als enters amb mòdul p
k defineix el tamany dels blocs que gestionem
cal que 2 ≤ n − k = p − k − 1 on n = p - 1
els valors es poden modificar en executar -p=257 -k=200

per codifificar, en executar després dels valors p i k si es volen modificar, s'afegeix codifica i el nom del fitxer
per descodificar, el mateix, però amb descodifica.

exemples:
./a.out codifica file.dat
./a.out descodifica file.RS.dat
./a.out -p=97 -k=60 codifica file.txt
./a.out -p=97 -k=60 descodifica file.RS.txt
