# TP1 Capteurs / Actionneurs

- *Florian Latapie, Tom Bevan, Vinh Faucher*

- [TP1 Capteurs / Actionneurs](#tp1-capteurs--actionneurs)
  - [1. Gestion des périphériques](#1-gestion-des-périphériques)
    - [R1 Lister l'ensemble des périhpériques](#r1-lister-lensemble-des-périhpériques)
    - [R2 Remplissez la table d'allumage des afficheurs](#r2-remplissez-la-table-dallumage-des-afficheurs)
  - [Programmation des interruptions](#programmation-des-interruptions)
    - [R3 Rappeler les avantages et inconvénients de chaque méthode.](#r3-rappeler-les-avantages-et-inconvénients-de-chaque-méthode)

## 1. Gestion des périphériques

### R1 Lister l'ensemble des périhpériques

| Périphérique     | Addresse  |
|------------------|-----------|
| hex0             | 0x8005080 |
| hex1             | 0x8005070 |
| hex2             | 0x8005020 |
| hex3             | 0x8005040 |
| hex4             | 0x8005050 |
| hex5             | 0x8005060 |
| jtag_uart        | 0x80050b0 |
| key              | 0x8005030 |
| leds             | 0x80050a0 |
| onchip_memory2_0 | 0x8002000 |
| sdram            | 0x4000000 |
| switches         | 0x8005090 |
| timer            | 0x8005000 |

### R2 Remplissez la table d'allumage des afficheurs

| Chiffre décimal | Configuration binaire | Configuration hexadécimale |
|-----------------|-----------------------|----------------------------|
| 0               | 011 1111              | 0x3F                       |
| 1               | 000 0110              | 0x06                       |
| 2               | 101 1011              | 0x5B                       |
| 3               | 100 1111              | 0x4F                       |
| 4               | 110 0110              | 0x66                       |
| 5               | 110 1101              | 0x6D                       |
| 6               | 111 1101              | 0x7D                       |
| 7               | 000 0111              | 0x07                       |
| 8               | 111 1111              | 0x7F                       |
| 9               | 110 1111              | 0x6F                       |
| A               | 111 0111              | 0x77                       |
| B               | 111 1100              | 0x7C                       |
| C               | 011 1001              | 0x39                       |
| D               | 101 1110              | 0x5E                       |
| E               | 111 1001              | 0x79                       |
| F               | 111 0001              | 0x71                       |

## Programmation des interruptions

### R3 Rappeler les avantages et inconvénients de chaque méthode

| Méthode de lecture de périphérique | Avantage ou inconvénient |
|------------------------------------|--------------------------|
| Polling                            | ...                      |
| Interruptions                      | ...                      |

### R3 A quoi sert les préfixes suivants, souvent utilisés en embarqué : volatile sur la variable d'état des boutons poussoirs, static sur le routine d'interruption ?
| Préfixe                            | Rôle                     |
|------------------------------------|--------------------------|
| Volatile                           | ...                      |
| Static                             | ...                      |
