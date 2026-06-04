const int PIN_POTENCIOMETRO = 34;
const int PIN_LED = 2;
const int ADC_MAX = 1023;
const int PWM_MAX = 255;
const float VREF = 3.3;
const int CANTIDAD_LECTURAS = 10;
const int PAUSA_ENTRE_LECTURAS_MS = 20;
const int PAUSA_CICLO_MS = 1000;
const int UMBRAL_LED = 33;
void tomarLecturas(int pin, int datos[], int cantidad, int pausaMs) {
/* Verificamos que el arreglo sea válido y que la cantidad tenga sentido. */
if (datos == nullptr || cantidad <= 0) {
return;
}
/* Guardamos una lectura ADC en cada posición del arreglo. */
for (int i = 0; i < cantidad; i++) {
datos[i] = analogRead(pin);
delay(pausaMs);
}
}
void analizarLecturas(int datos[], int cantidad,
int *promedio,
int *minimo,
int *maximo) {
/* Verificamos que las direcciones recibidas sean válidas. */
if (datos == nullptr || promedio == nullptr || minimo == nullptr || maximo ==
nullptr || cantidad <= 0) {
return;
}
/* La suma puede crecer, por eso usamos long. */
long suma = 0;
/* Tomamos el primer dato como referencia inicial. */
*minimo = datos[0];
*maximo = datos[0];
/* Recorremos todas las lecturas. */
for (int i = 0; i < cantidad; i++) {
/* Acumulamos la lectura actual. */
suma = suma + datos[i];
/* Si encontramos una lectura menor, actualizamos el mínimo. */
if (datos[i] < *minimo) {
*minimo = datos[i];
}
/* Si encontramos una lectura mayor, actualizamos el máximo. */
if (datos[i] > *maximo) {
*maximo = datos[i];
}
}
/* Guardamos el promedio en la variable externa apuntada por promedio. */
*promedio = (int)(suma / cantidad);
}
void convertirPromedio(int promedioADC,
float *voltaje,
int *porcentaje,
int *brilloPWM) {
/* Verificamos que los punteros de salida sean válidos. */
if (voltaje == nullptr || porcentaje == nullptr || brilloPWM == nullptr) {
return;
}
/* Convertimos ADC a voltaje aproximado. */
*voltaje = (promedioADC * VREF) / ADC_MAX;
/* Convertimos ADC a porcentaje usando long para evitar desbordamiento. */
*porcentaje = (int)((long)promedioADC * 100L / ADC_MAX);
/* Convertimos ADC a valor PWM entre 0 y 255. */
*brilloPWM = (int)((long)promedioADC * PWM_MAX / ADC_MAX);
}
void decidirEstadoLED(int porcentaje, int umbral, int *estadoLED) {
/* Verificamos que el puntero de salida sea válido. */
if (estadoLED == nullptr) {
return;
}
/* El LED se considera activo si el porcentaje supera el umbral. */
if (porcentaje >= umbral) {
*estadoLED = HIGH;
} else {
*estadoLED = LOW;
}
}
void aplicarSalidaLED(int pinLED, int estadoLED, int brilloPWM) {
/* Si el estado es LOW, apagamos el LED. */
if (estadoLED == LOW) {
analogWrite(pinLED, 0);
} else {
/* Si el estado es HIGH, aplicamos el brillo calculado. */
analogWrite(pinLED, brilloPWM);
}
}
void mostrarLecturas(int datos[], int cantidad) {
/* Verificamos que el arreglo sea válido. */
if (datos == nullptr || cantidad <= 0) {
  return;
}
Serial.print("Lecturas: ");
/* Imprimimos todos los valores del arreglo. */
for (int i = 0; i < cantidad; i++) {
Serial.print(datos[i]);
Serial.print(" ");
}
Serial.println();
}
void mostrarReporte(int promedio, int minimo, int maximo,
float voltaje, int porcentaje,
int brilloPWM, int estadoLED) {
/* Mostramos un reporte completo del ciclo actual. */
Serial.print("Promedio ADC = ");
Serial.println(promedio);
Serial.print("Minimo ADC = ");
Serial.println(minimo);
Serial.print("Maximo ADC = ");
Serial.println(maximo);
Serial.print("Voltaje = ");
Serial.print(voltaje, 2);
Serial.println(" V");
Serial.print("Porcentaje = ");
Serial.print(porcentaje);
Serial.println(" %");
Serial.print("Brillo PWM = ");
Serial.println(brilloPWM);
Serial.print("LED = ");
if (estadoLED == HIGH) {
Serial.println("ENCENDIDO");
} else {
Serial.println("APAGADO");
}
Serial.println("----------------------------------------");
}
void setup() {
/* Iniciamos comunicación serial. */
Serial.begin(115200);
/* Configuramos el pin del LED como salida. */
pinMode(PIN_LED, OUTPUT);
}
void loop() {
/* Arreglo local para almacenar varias lecturas ADC. */
int lecturas[CANTIDAD_LECTURAS];
/* Variables para resultados del análisis. */
int promedio = 0;
int minimo = 0;
int maximo = 0;
/* Variables para resultados convertidos. */
float voltaje = 0.0;
int porcentaje = 0;
int brilloPWM = 0;
/* Variable para el estado del LED. */
int estadoLED = LOW;
/* 1. Tomamos varias lecturas y las guardamos en el arreglo. */
tomarLecturas(PIN_POTENCIOMETRO, lecturas, CANTIDAD_LECTURAS,
PAUSA_ENTRE_LECTURAS_MS);
/* 2. Analizamos el arreglo usando punteros de salida. */
analizarLecturas(lecturas, CANTIDAD_LECTURAS, &promedio, &minimo, &maximo);
/* 3. Convertimos el promedio a voltaje, porcentaje y PWM. */
convertirPromedio(promedio, &voltaje, &porcentaje, &brilloPWM);
/* 4. Decidimos si el LED debe encenderse o apagarse. */
decidirEstadoLED(porcentaje, UMBRAL_LED, &estadoLED);
/* 5. Aplicamos el estado y el brillo al LED. */
aplicarSalidaLED(PIN_LED, estadoLED, brilloPWM);
/* 6. Mostramos el arreglo y el reporte completo. */
mostrarLecturas(lecturas, CANTIDAD_LECTURAS);
mostrarReporte(promedio, minimo, maximo, voltaje, porcentaje, brilloPWM,
estadoLED);
delay(PAUSA_CICLO_MS);
}