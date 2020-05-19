Bah Amadou
Calmettes Pierre
3 MIC


Organistaion du git : 

Obj1 : programmation_dft_simulation

obj2 : programmation_dft_réel

obj3 : son

obj4 : projet_final



DFT Challenge :
Rapidité : 1162 states.


Calcul de M2Tir :
Km = 1/2^32
Ka = 4095/3300
Ks = 32
Kt = 32768

Test objectif 2 : utiliser le logic analyser et regarder compteurOccurence pour 0,1,2,3,4,5, normalement on observe une incrémentation des compteurs.


Objectif 3 :

Marche à suivre pour  obtenir la courbe : lancer le débug, ouvrir le logic analyser, dans setup ajouter TIM3_CCR3 en Analog et avec un min 0 et un max de 654 (la résolution).
Lancer le programme et attendre que le max time du logic analyser ait dépasser les 0,6 secondes à peu près.  Zoomer pour avoir une grid de 50 ms et se mettre au début, on obtient
bien la courbe.

Vérifier qu'il y a bien 91µs entre deux échantillons successifs : Il faut placer un point d'arrêt au début de la fonction systeme_callback dans l'assembleur, lancer le programme 
jusqu'au point d'arrêt, remettre l'horloge t1 à zéro en bas à droite, run le programme une fois, t1 = 91 us.