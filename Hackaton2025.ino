// Definir pines de los sensores ultrasónicos (ejemplo con 2 sensores)
const int TRIG_PINS[] = {1, 41, 33}; // Pines GPIO para Trigger
const int ECHO_PINS[] = {2, 42, 34}; // Pines GPIO para Echo
const int NUM_SENSORS = sizeof(TRIG_PINS) / sizeof(TRIG_PINS[0]);

// Estructura para almacenar los datos del sensor
struct SensorData {
  int sensorIndex;
  float distance;
  String estado;
};

// Función para obtener la distancia medida por un sensor ultrasónico
float readSensor(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long pulseDuration = pulseIn(echoPin, HIGH);
  float distance = (pulseDuration * 0.0343) / 2; // Conversión a cm
  return distance;
}
float valor = 0;
// Simulación de predicción basada en reglas simples
String simplePrediction(float distance) {
  if (distance > 50) {
    return "Vacío";
  } else if (distance > 20) {
    valor ++;
    return "Medio";
  } else {
    valor += 2;
    return "Lleno";
  }
}

// Función de comparación para ordenar los sensores por distancia (ascendente)
int compareSensors(const void* a, const void* b) {
  float distanceA = ((SensorData*)a)->distance;
  float distanceB = ((SensorData*)b)->distance;
  if (distanceA < distanceB) return -1;
  if (distanceA > distanceB) return 1;
  return 0;
}

void setup() {
  Serial.begin(9600); // Inicializar la comunicación serial
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(TRIG_PINS[i], OUTPUT);
    pinMode(ECHO_PINS[i], INPUT);
  }
}

void loop() {
  SensorData sensorData[NUM_SENSORS]; // Arreglo para almacenar los datos de los sensores

  // Leer datos de los sensores y almacenarlos
  for (int i = 0; i < NUM_SENSORS; i++) {
    sensorData[i].sensorIndex = i + 1;
    sensorData[i].distance = readSensor(TRIG_PINS[i], ECHO_PINS[i]);
    sensorData[i].estado = simplePrediction(sensorData[i].distance);
  }

  // Ordenar los datos por distancia ascendente
  qsort(sensorData, NUM_SENSORS, sizeof(SensorData), compareSensors);
  valor = valor / NUM_SENSORS;
  // Imprimir los datos ordenados
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Sensor ");
    Serial.print(sensorData[i].sensorIndex);
    Serial.print(": ");
    Serial.print(sensorData[i].distance);
    Serial.print(" cm - Estado: ");
    Serial.println(sensorData[i].estado);
  }
  Serial.print("Contenedor con prioridad de ");
  Serial.print(valor);
  Serial.println(" puntos");
  valor = 0;
  delay(10000); // Leer cada 10 segundos
}