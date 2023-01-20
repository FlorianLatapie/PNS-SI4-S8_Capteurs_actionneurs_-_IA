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

| Méthode de lecture de périphérique | Avantage ou inconvénient                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  |
|------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Polling                            | Les avantages du polling sont qu'elle est simple à mettre en place et à gérer, et qu'elle  ne nécessite pas de protocole de communication spécifique. Elle permet également de surveiller régulièrement l'état d'un périphérique ou d'une ressource, ce qui peut être utile pour détecter les erreurs ou les anomalies. Les inconvénients du polling sont qu'elle peut entraîner un surcharge de trafic réseau et une utilisation inutile de la bande passante, surtout si elle est utilisée fréquemment et avec des intervalles courts. Elle peut également augmenter la latence et la latence des opérations, car elle nécessite un délai de réponse pour chaque demande de polling. En outre, polling peut ne pas détecter les événements en temps réel et peut manquer des événements importants s'ils se produisent entre les pollings.                                                                                                                              |
| Interruptions                      | Les avantages des interruptions sont qu'elles permettent une détection en temps réel des événements, ce qui réduit la latence et la latence des opérations. Les interruptions permettent également de réduire la charge de traitement sur le processeur en ne nécessitant une attention qu'en cas de besoin. Les interruptions peuvent également réduire la consommation d'énergie en ne nécessitant une surveillance active que lorsque cela est nécessaire. Les inconvénients des interruptions sont qu'elles peuvent rendre le code plus difficile à déboguer et à maintenir, car elles peuvent causer des effets de bord imprévus lorsque plusieurs interruptions sont gérées simultanément. Les interruptions peuvent également causer des conflits d'accès aux ressources partagées, ce qui peut entraîner des erreurs de synchronisation. Enfin, les interruptions peuvent causer des erreurs de matériel et de logiciel si elles ne sont pas gérées correctement. |

### R4 A quoi sert les préfixes suivants, souvent utilisés en embarqué : volatile sur la variable d'état des boutons poussoirs, static sur le routine d'interruption ?

| Préfixe  | Rôle |
|----------|------|
| Volatile | ...  |
| Static   | ...  |
