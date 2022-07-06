float Tension_de_trabajo = 3.3; // VCC 

// SENSOR RADIACION SOLAR
float Rf = 507.5;       //Resistencia de ganancia de transimpedancia
int RS = A5;             //Pin análogico 5
float lecturaRS;         // Variable para lectura del pin analogico 5
float Voltaje_salida;    // Valor que vamos a calcular
float Corriente_inversa; // Valor que vamos a calcular
float Radiacion_solar;   // Valor que vamos a calcular
 
void setup(){
  
  Serial.begin(9600);
  pinMode(RS, INPUT);
  Serial.println("Medición de radiación solar:");
  Serial.println();
  
}

void loop(){
  
  lecturaRS=0;
  for(int i=0;i<10;i++){
    lecturaRS+=analogRead(RS);
    delay(100);
  }
  lecturaRS=lecturaRS/10;

  // Conversión medida análogica a voltaje
  Voltaje_salida = (Tension_de_trabajo/1023.0)*lectura;

  //Calculamos la corriente inversa
  Corriente_inversa = Voltaje_salida/Rf;

  //Calculamos la radiacion solar a partir de la corriente inversa
  Radiacion_solar = 31.06*Corriente_inversa-2.11;

  // Imprimimos los resultados por el monitor serie
  Serial.print("El voltaje de salida del amplificador vale ");
  Serial.print(Voltaje_salida,2);
  Serial.println("V");
  Serial.print("La corriente inversa vale ");
  Serial.print(Corriente_inversa,2);
  Serial.println("uA");
  Serial.print("La radiación solar vale ");
  Serial.println(Radiacion_solar,2);
  
  delay(5000);
}
