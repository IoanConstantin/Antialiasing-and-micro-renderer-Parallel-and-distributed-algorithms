APD - TEMA 1
=============

1). Partea intai
----------------
-> Campurile structurii imagine sunt sirul de pe prima linie (P5 sau P6, in functie de care determinam de ce tip este imaginea), latimea, inaltimea, max_value si matricea de inaltime linii si 3*latime coloane, in cazul imaginii color, respectiv doar latime coloane, in cazul imaginii alb-negru.

-> Avem cele 2 functii de citire si de scriere care isi pastreaza rolurile precum prevede enuntul temei.

-> In functia de resize vom lucra cu 2 structuri de tip imagine definite global, prima dintre ele, structura1, este aceeasi cu imaginea citita, iar a doua, structura2, va fi imaginea rezultata dupa prelucrare, ale carei campuri le vom copia apoi in out pentru a le trimite ca parametru la functia de scriere.

-> In functia de resize cream thread-urile si facem join precum la laborator, iar in threadFunction luam pe rand fiecare dintre cele 4 cazuri: imagine color, resize_factor par; imagine color, resize_factor 3; imagine alb-negru, resize_factor par; imagine alb-negru, resize_factor 3.

-> Pentru fiecare caz facem paralelizarea unuia dintre for-urile cu care parcurgem matricea si calculam fiecare element al matricii imaginii noi ca fiind media aritmetica a celor resize_factor^2 elemente din matricea originala, in cazul de resize_factor par si respectiv inmultirea fiecarui element din matricea originala cu fiecare element din Kernel-ul Gaussian impartit la suma elementelor Kernel-ului, pentru resize_factor 3.

-> Deoarece fiecare element al matricii este un unsigned char, este nevoie sa facem cast la int atunci cand inmultim acest element cu un numar natural mai mare decat 1 sau atunci cand il adunam cu un alt element si respectiv sa facem cast inapoi la unsigned char atunci cand adaugam in noua matrice elementul obtinut din impartirea sumei reprezentata ca int la resize_factor^2 sau la 16, in functie de valoarea lui resize_factor.