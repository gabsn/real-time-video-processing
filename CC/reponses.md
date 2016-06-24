---
lang      : fr
title     : Contrôle de connaissances SE207 "SystemC"
date      : 24 juin 2016
---

# Instructions

Ce contrôle de connaissances est strictement individuel.
Vous devez modifier ce fichier pour y inclure vos réponses. Seules les parties entre *---* sont à modifier.

# Questions

### Question 1

En SystemC, pour représenter une donnée signée sur 32bits vous pouvez utiliser l'un des types suivants:

- `int32_t` (`unsigned int`),
- `sc_int<32>`
- `sc_bv<32>`
- `sc_lv<32>`

Quels critères vous feraient choisir un type plutôt qu'un autre?

---

Si l'on souhaite effectuer des opération arithmétiques, il vaut mieux choisir un sc_int<32> ou un int32_t (les types C++ étant à privilégier). 
Si non, il vaut mieux utiliser un sc_bv<32> si c'est pour une utilisation purement logique et un sc_lv<32> si l'on veut modéliser du matériel avec les 4 possibilités d'état (0,1,Z,X).

---

### Question 2

Pourquoi ne peut-on pas connecter *directement* la sortie d'une module (par exemple `sc_out<bool> o`)  à l'entrée d'un autre module (par exemple `sc_in<bool> i`)?

---

Les entrées/sorties des modules sont des ports et prennent en argument un sc_signal. 
Il faut donc relier chacun des deux ports à au même sc_signal si l'on veut les connecter.

---

### Question 3

- Pourquoi doit-on avoir une boucle infinie dans un `SC_THREAD`?
- Que se passe-t-il s'il n'y a pas cette boucle?

---

Un SC_THREAD est un processus qui se lance au début de la simulation (sauf si l'on ajoute l'instruction dont_initialize()). 
Ainsi, s'il n'y a pas de boucle infinie, celui-ci se lancera une fois, puis se terminera, ce qui n'est pas le comportement recherché a priori.

---

### Question 4

Nous voulons modéliser un bloc matériel synchrone (à une horloge `clk`) dans lequel une étape de traitement doit attendre le passage à `true` d'un signal de validation `start`.
Pour ce fait, on utilise un `SC_THREAD` sensible au seul front montant de l'horloge (`sensitive << clk.pos();`).

Nous proposons les deux implémentations suivantes pour cette attente (le reste du code n'est pas montré):

**Version 1**
```{.cpp}
 …
 // attente du passage à 1 de start
 wait(start.posedge_event());
 // passage à l'étape suivante
 …
```

**Version 2**
```{.cpp}
 …
 // attente du passage à 1 de start
 while(!start)
    wait();
 // passage à l'étape suivante
 …
```

- Expliquez brièvement la différence de comportement entre ces deux versions.
- Pourquoi choisiriez-vous l'une par rapport à l'autre?

```

---

La Version 2 est synchrone au front montant de l'horloge. 
Autrement dit, si start passe à true entre deux coups d'horloge, le thread devra attendre le coup d'horloge suivant pour se débloquer.
Dans la Version 1, si start passe à true entre deux coups d'horloge, le thread se débloquera sans attendre le prochain coup d'horloge.

Ainsi, je choisirai l'une ou l'autre version en fonction de si j'ai besoin de réagir de manière synchrone ou asynchrone à mon signal start.

---


### Question 5

- Pourquoi devons-nous prendre des précautions quand nous utilisons une `sc_fifo` dans une `SC_METHOD`?


---

Normalement, si l'on cherche à écrire dans une fifo pleine, ou à lire dans une fifo vide, une méthode bloquante de la fifo est alors appelée afin d'attendre que la fifo perde un élément ou en gagne un.
Or, contrairement à un thread, on ne peut pas "bloquer" une SC_METHOD. 
Il faut donc pour cela utiliser des méthodes adaptées non bloquantes pour accéder aux données de la fifo.

---

