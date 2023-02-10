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