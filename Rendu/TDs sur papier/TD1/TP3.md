# TD1 d’ordonnancement temps réel

## Exercice 1 - Politique RMS

Soit les trois tâches indépendantes, prêtes à la date t=0 :

- T1 (C=1, D=P=5),
- T2 (C=2, D=P=10),
- T3 (C=7, D=P=29).

En considérant une politique d’ordonnancement RMS :

1. Rappeler la condition d’ordonnancement (préemptif) de cette politique.
2. Calculer le taux d'utilisation du processeur, que peut-on en conclure sur l’ordonnancabilité ?
3. Donner un schéma du séquencement des tâches sur 30 unités de temps dans le cas préemptif puis dans le cas non-préemptif. Quelle conclusion peut-on faire sur ce jeu de tâches ?
4. On considère une tâche supplémentaire T4(2,15). Refaire l’analyse précédente dans le cas préemptif uniquement.
5. On modifie maintenant le système en ne considérant que les tâches T1, T2 et T3 en modifiant T1(3, 5), T2(3,10) et T2(2,30). Le jeu de tâches est dit « harmonique » car les périodes sont multiples. Reprenez l’analyse en mode préemptif.
6. Que déduisez-vous de la condition d’ordonnancement RMS ?
7. Appliquer le théorème de la zone critique pour vérifier ce constat.

### Réponses exercice 1

| | C | P |
|---|---|---|
| T1 | 1 | 5 |
| T2 | 2 | 10 |
| T3 | 7 | 29 |
| T4 | 2 | 15 |

a.  
Plus petite est la période, plus grande est sa priorité  
$$U = \sum_{i=1}^{n} \frac{C_i}{T_i} \leq n(2^{\frac{1}{n}}-1)$$

b. n = nombre de tâches = 3

$$
\begin{align}
U &= \frac{1}{5} + \frac{2}{10} + \frac{7}{29} &\leq &3(2^{\frac{1}{3}}-1) \\
&= 0.2 + 0.2 + 0.24 &\leq &1.78 \\
&= 0.64 &\leq &1.78 \\
\end{align}
$$

c.
non préemptif : si on commence une tache on la finit sans interruption

![diagramme non preemptif et preemptif](./diagrammes-td1_ex1_c.drawio.png)
Vert = non préemptif  
Rouge = préemptif

d.

$$
\begin{align}
U &= \frac{1}{5} + \frac{2}{10} + \frac{7}{29} + \frac{2}{15} &\leq &4(2^{\frac{1}{4}}-1) \\
&= 0.2 + 0.2 + 0.24 + 0.13 &\leq &0.757 \\
&= 0.77 &\leq &0.757 \\
\end{align}
$$

![diagramme non preemptif](./diagrammes-td1_ex1_d.drawio.png)
Rouge = préemptif

e.

| | C | P |
|---|---|---|
| T1 | 3 | 5 |
| T2 | 3 | 10 |
| T3 | 2 | 30 |

$$
\begin{align}
U &= \frac{3}{5} + \frac{3}{10} + \frac{2}{30} &\leq &3(2^{\frac{1}{3}}-1) \\
&= 0.6 + 0.3 + 0.06 &\leq &0.78 \\
&= 0.96 &\leq &0.78 \\
\end{align}
$$
$\Rightarrow$ Faux

On peut tenter un ordonnancement sans garantie qu'il soit correct

![diagramme non preemptif](./diagrammes-td1_ex1_e.drawio.png)
Rouge = préemptif

f.

J'en déduis que la condition d'ordonnancement RMS est valide alors on peut tenter un ordonnancement avec garantie qu'il soit correct.

g.

Formule : $W_j(n) = \sum_{i=1}^{j} C_i \times \left \lceil\frac{T}{T_i} \right \rceil$

$$
i = 1 \Rightarrow T_0 = C_1 = 3 \Rightarrow W_1(0) = 3; W_1(3) = 3 \times \left \lceil\frac{3}{5}\right \rceil = 3 \Rightarrow W_1(3) = 3
$$

$$
\begin{align}
i = 2 \Rightarrow T_0 = C_1 + C_2 = 6 \Rightarrow W_2(0) = 6; &W_2(6) = 3 \times \left \lceil\frac{6}{5} \right \rceil + 3 \times \left \lceil\frac{6}{10} \right \rceil = 9 \\ 
&W_2(9) = 3 \times \left \lceil\frac{9}{5} \right \rceil + 3 \times \left \lceil\frac{9}{10} \right \rceil = 9 \\
\Rightarrow &W_2(9) = 9
\end{align}
$$

$$
\begin{align}
i = 3 \Rightarrow T_0 = \sum_{i=1}^{3} C_i = 8 \Rightarrow W_3(0) = 8; &W_3(8) = 3 \times \left \lceil\frac{8}{5} \right \rceil + 3 \times \left \lceil\frac{8}{10} \right \rceil + 2 \times \left \lceil\frac{8}{30} \right \rceil = 11 \\
&W_3(11) = 3 \times \left \lceil\frac{11}{5} \right \rceil + 3 \times \left \lceil\frac{11}{10} \right \rceil + 2 \times \left \lceil\frac{11}{30} \right \rceil = 11 \\
&W_3(17) = 3 \times \left \lceil\frac{17}{5} \right \rceil + 3 \times \left \lceil\frac{17}{10} \right \rceil + 2 \times \left \lceil\frac{17}{30} \right \rceil = 20 \\
&W_3(20) = 3 \times \left \lceil\frac{20}{5} \right \rceil + 3 \times \left \lceil\frac{20}{10} \right \rceil + 2 \times \left \lceil\frac{20}{30} \right \rceil = 20 \\
\Rightarrow &W_3(20) = 20
\end{align}
$$

$\Rightarrow W_3(20) < T_3 = 30$ on peut en conclure que le théorème de la zone critique est vérifié. Donc l'ordonnancement est correct.

## Exercice 2 – Politique EDF

On considère à présent 3 tâches T1(5,12), T2(2,6) et T3(5,24).
Calculer le taux d'utilisation. Concluez sur l’ordonnancabilité.
Déterminer le nombre d’unités de temps libre sur la période totale d’ordonnancement.
Dessinez sur cette période d’étude le séquencement généré par EDF d’abord en mode préemptif puis en mode non-préemptif.
Quelle seraient les caractéristiques maximum (temps d’exécution et fréquence) d’une tâche apériodique à exécuter en plus de ces 3 tâches pour que le système reste ordonnancable ?

| | C | T |
|---|---|---|
| T1 | 5 | 12 |
| T2 | 2 | 6 |
| T3 | 5 | 24 |

$$
\begin{align}
U &= \frac{5}{12} + \frac{2}{6} + \frac{5}{24} &\leq &1 \\
&= 0.42 + 0.33 + 0.21 &\leq &1 \\
&= 0.96 &\leq &1 \\
\end{align}
$$

**hyper période :** la plus grande période des tâches périodiques

Il y a 1 unité de temps libre sur la période totale d'ordonnancement car nous avons une hyper période de 24.

![diagramme non preemptif et preemptif](./diagrammes-td1_ex2.drawio.png)
Vert = non préemptif
Rouge = préemptif

# TD2 d’ordonnancement temps réel

## Exercice 3 - Comparaison d'ordonnancements

Soient deux tâches T1 et T2 définies par les paramètres suivants : S = 0, P1 = 8, P2 = 10, C1 = 4 et C2 = 5.
Les délais critiques sont égaux aux périodes (soient ∀ i : Di = Pi ).

- Dessinez sur les 24 premières unités de temps l’ordonnancement généré par EDF (en mode  préemptif). Existe il des échéances manquées ?
- Dessinez sur les 24 premières unités de temps l’ordonnancement généré par un algorithme à priorités fixes RM (en mode préemptif). Existe il des échéances manquées ?
- Concluez

EDF:

$$
\begin{align}
U &= \frac{4}{8} + \frac{5}{10} &\leq &1 \\
&= 0.5 + 0.5 &\leq &1 \\
&= 1 &\leq &1 \\
\end{align}
$$

|    | C | (D)  | P  |
|----|---|------|----|
| T1 | 4 | (8)  | 8  |
| T2 | 5 | (10) | 10 |

RMS :

$$
\begin{align}
U &= \sum_{i=1}^{n} \frac{C_i}{T_i} &\leq n(2^{\frac{1}{n}} - 1) \\
&= \frac{4}{8} + \frac{5}{10} &\leq 2(2^{\frac{1}{2}} - 1) \\
&= 0.5 + 0.5 &\leq 0.828 \\
&= 1 &\leq 0.828 \\
\end{align}
$$
Faux

![diagramme non preemptif et preemptif](./diagrammes-td2_ex3.drawio.png)
Vert = EDF
Rouge = RMS

À l'unité de temps 10, la tâche devrait s'être exécuté pendant 5 unité de temps, hors elle n'en a fait que 4, nous avons donc des échéances manquées.

## Exercice 4 - Politique LLF

Soient trois tâches T1(1,8), T2(2,4) et T3(4,10).
Les délais critiques sont égaux aux périodes (soient $\forall i : D_i = P_i $).

- Dessinez sur les 24 premières unités de temps l’ordonnancement généré par RMS (en mode préemptif). Existe il des échéances manquées ?
- Dessinez sur les 24 premières unités de temps l’ordonnancement généré par un algorithme à LLF (en mode préemptif). Existe il des échéances manquées ?
- Concluez

![LLF & RMS](./diagrammes-td2_ex4.drawio.png)

Pour RMS :

À l'unité de temps 10, la tâche $T_3$ devrait s'être exécuté pendant 4 unité de temps, hors elle n'en a fait que 3, nous avons donc des échéances manquées.
