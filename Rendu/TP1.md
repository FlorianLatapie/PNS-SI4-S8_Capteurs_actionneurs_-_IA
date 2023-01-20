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
| 0               | 100 0000              | 0x40                       |
| 1               | 111 1001              | 0x79                       |
| 2               | 010 0100              | 0x24                       |
| 3               | 011 0000              | 0x30                       |
| 4               | 001 1001              | 0x19                       |
| 5               | 001 0010              | 0x12                       |
| 6               | 000 0010              | 0x02                       |
| 7               | 111 1000              | 0x78                       |
| 8               | 000 0000              | 0x00                       |
| 9               | 001 0000              | 0x10                       |
| A               | 000 1000              | 0x08                       |
| B               | 000 0011              | 0x03                       |
| C               | 100 0110              | 0x46                       |
| D               | 010 0001              | 0x21                       |
| E               | 000 0110              | 0x06                       |
| F               | 000 1110              | 0x0E                       |

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
