float Tension_de_trabajo = 3.3; // VCC 

// SENSOR UV
float Rf = 102800;       //Resistencia de ganancia de transimpedancia
int UV = A5;             //Pin análogico 5
float lecturaUV;         // Variable para lectura del pin analogico 5
float Voltaje_salida;    // Valor que vamos a calcular
float Corriente_inversa; // Valor que vamos a calcular
float IndiceUV;          // Valor que vamos a calcular

void setup(){
  
  Serial.begin(9600);
  pinMode(UV, INPUT);
  Serial.println("Medición de índice UV:");
  Serial.println();
  
}

void loop(){
  
  lecturaUV=0;
  for(int i=0;i<10;i++){
    lecturaUV+=analogRead(UV);
    delay(100);
  }
  lecturaUV=lecturaUV/10;

  // Conversión medida análogica a voltaje
  Voltaje_salida = (Tension_de_trabajo/1023.0)*lecturaUV;

  //Calculamos la corriente inversa
  Corriente_inversa = Voltaje_salida/Rf;

  //Calculamos el índice UV a partir de la corriente inversa
  IndiceUV = 0.9142*Corriente_inversa-2.1364;

  // Imprimimos los resultados por el monitor serie
  Serial.print("El voltaje de salida del amplificador vale ");
  Serial.print(Voltaje_salida,2);
  Serial.println("V");
  Serial.print("La corriente inversa vale ");
  Serial.print(Corriente_inversa,2);
  Serial.println("uA");
  Serial.print("El índice UV vale ");
  Serial.println(IndiceUV,2);
  
  delay(5000);
}
