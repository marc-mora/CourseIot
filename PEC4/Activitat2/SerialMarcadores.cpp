#include <Arduino.h>

extern boolean estadoLed;

const byte charCantidad = 32;
char charRecibidos[charCantidad];
boolean datosNuevos = false;


void marcadoresRecibidos() {
    static boolean recibiendoDatos = false;
    static byte ndx = 0;
    char marcadorInicial = '<';
    char marcadorFinal = '>';
    char rc;
 
    while (Serial.available() > 0 && datosNuevos == false) {
        rc = Serial.read();

        if (recibiendoDatos == true) {
            if (rc != marcadorFinal) {
                charRecibidos[ndx] = rc;
                ndx++;
                if (ndx >= charCantidad) {
                    ndx = charCantidad - 1;
                }
            }
            else {
                charRecibidos[ndx] = '\0';
                recibiendoDatos = false;
                ndx = 0;
                datosNuevos = true;
            }
        }

        else if (rc == marcadorInicial) {
            recibiendoDatos = true;
        }
    }
}

void revisarDatos() {
    if (datosNuevos == true) {
      if (strcmp("ON", charRecibidos) == 0){
          Serial.println("On recibido");
          estadoLed = true;
      }
      if (strcmp("OFF", charRecibidos) == 0){
          Serial.println("Off recibido");
          estadoLed = false;
      }
      datosNuevos = false;
    }
}

void buscaMarcadores() {
    marcadoresRecibidos();
    revisarDatos();
}
