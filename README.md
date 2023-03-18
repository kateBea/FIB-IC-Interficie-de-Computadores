# FIB-IC [Interficie de Computadores]
Repositorio del curso de Interficie de Computadores en el grado de Ingeniería Informática en la FIB UPC

# Notas:
| Laboratorio  | Nota Evaluación |
| ------------- | ------------- |
| [ L1 ]  &nbsp;[Electrònica](Laboratorios/L1) | 7.21  |
| [ L2 ]  &nbsp;[Entrda/Sortida](Laboratorios/L2) | 9.70 |
| [ L3 ]  &nbsp;[7 Segments](Laboratorios/L3) | 7.82  |
| [ L4 ]  &nbsp;[GLCD](Laboratorios/L4) | 6.18 |
| [ L5 ]  &nbsp;[GLCD Individual](Laboratorios/L5) | 9.00  |
| [ L6 ]  &nbsp;[CCP i ADC](Laboratorios/L6) | 9.13  |

Les transparències al directori <b>Temario</b> no constituïexen el temari principal de l'assignatura,
només serveixen de guia per les classes. Es aconsellable adquirir el llibre suggerit a l'apartat de bibliografia de
la primera transparència d'introducció al temari. A part d'això, el [Datasheet](Extras/Datasheet) conté tota la informació
relativa al micro amb què es treballa als Labs.

- Estructura general dels programes en C:

```c
/**
* Info sobre el source file...
*/

#include <xc.h>     // llibreria del PIC
#include <config.h> // fitxer amb configuració básica del chip

// definitions
// funcions, interrupcions, ...

/**
* Inicialitza el context del PIC abans 
* d'entrar al bucle principal
*/
void configPic(void) {
    // configuració de Pins
    // configuració de PORTS
    // Inicializació registres
}

void main(void) {
    configPic();
    
    // bucle principal
    while(1) {
        // PIC mai ha de sortir d'aquest bucle
        // quan està encés
    }

}
```
