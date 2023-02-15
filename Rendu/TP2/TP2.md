# TP2 Capteurs / Actionneurs

- Tom Bevan
- Vinh Faucher
- Florian Latapie

## 1. Création de tâches

### R1. Rappeler les paramètres des deux fonctions de création de tâches uCOS

| OSTaskCreate Paramètre  | Type              |
|-------------------------|-------------------|
| task                    | void(*)(void *pd) |
| pdata                   | void*)            |
| ptos                    | OS__STK*          |
| prio                    | INT8U             |

| OSTaskCreateExt Paramètre | Type              |
|---------------------------|-------------------|
| task                      | void(*)(void *pd) |
| pdata                     | void*             |
| ptos                      | OS__STK*          |
| prio                      | INT8U             |
| id                        | INT16U            |
| pbos                      | OS_STK*           |
| stk_size                  | INT32U            |
| pext                      | void*             |
| opt                       | INT16U            |

### R2. Citer les différents services disponibles dans uCOS
| Services                      | Communication ou Synchronosation ? |
|-------------------------------|------------------------------------|
| Sémaphores                    | Sychronisation                     |
| Files d'attentes (Queue)      | Communication                      |
| MailBox                       | Communication                      |
| Signaux                       | Sychronisation                     |
| Mutexes                       | Sychronisation                     |
| Rendez-vous                   | Sychronisation                     |
| Event-Flags                   | Sychronisation                     |



### R3. Que constatez vous ?
Quand D1>=D2 il n'y a pas de problème, tous les messages sont bien reçus. En revanche quand D2>D1 tous les messages ne sont pas reçus, et au lieu de recevoir des messages 1 par 1, nous les recevons k par k, avec k>0 et D2 = k * D1.

