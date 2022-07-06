//CONSTANTES
float Tension_de_trabajo = 4.85; // VCC 
float Tref = 298.15;             // Temperatura de referencia 25º

// DIVISOR 1 (NTC47k)
int   Pin_0= A0;        // Pin analogico 0
float  lectura1;        // Lectura en el pin analogico 0
float Voltaje_salida;  // Valor que vamos a calcular
float R1 = 47530;      // Resistencia abajo del NTC
float Resistencia;     // Valor que vamos a calcular
float Temperatura;     // Valor que vamos a calcular
float Resistencia_Tref = 48280; // R a temperatura de referencia 25º (fabricante)
float Cte_B = 3994.52;    // Constante B de temperatura (fabricante)


//DIVISOR 2 (NTC470k)
int   Pin_4 = A4;        // Pin analogico 4
float  lectura2;         // Lectura en el pin analogico 4
float Voltaje_salida_2; // Valor que vamos a calcular
float R3 = 473100;      // Resistencia abajo del NTC
float Resistencia2;     // Valor que vamos a calcular
float Temperatura2;     // Valor que vamos a calcular
float Resistencia_Tref_2 = 503680; // R a temperatura de referencia 25º (fabricante)
float Cte_B_2 = 4783.54;   // Constante B de temperatura (fabricante)


void setup() {
  Serial.begin(9600);
  pinMode(Pin_0, INPUT);
  pinMode(Pin_4, INPUT);
  Serial.println("Medición de resistencia y temperatura:");
  Serial.println();
}

void loop() {
  // Leemos los valores en el pin A0 y A4
  lectura1=0;
  lectura2=0;
  for(int i=0;i<10;i++){
    lectura1+=analogRead(Pin_0);
    lectura2+=analogRead(Pin_4);
  }
  lectura1=lectura1/10;
  lectura2=lectura2/10;

  // Conversión medida análogica a voltaje
  Voltaje_salida=lectura1*(5/1024.0);
  Voltaje_salida_2=lectura2*(5/1024.0);
  
  // Lo imprimimos en el Monitor Serie
  Serial.print("Voltaje salida del divisor NTC47k: ");
  Serial.print(Voltaje_salida,2);
  Serial.println(" Voltios");
  Serial.print("Voltaje salida del divisor NTC470k: ");
  Serial.print(Voltaje_salida_2,2);
  Serial.println(" Voltios");
  
  //Calculamos la resistencia desconocida con las fórmulas
  Resistencia=R1*(Tension_de_trabajo/Voltaje_salida-1);
  Resistencia2=R3*(Tension_de_trabajo/Voltaje_salida_2-1);

  // Lo imprimimos en el Monitor Serie
  Serial.print("La resistencia del NTC47k vale: ");
  Serial.print(Resistencia,2);
  Serial.println(" Ohm.");
  Serial.print("La resistencia del NTC470k vale: ");
  Serial.print(Resistencia2,2);
  Serial.println(" Ohm.");

  //Calculamos la temperatura
  Temperatura=Cte_B*Tref/(Tref*log(Resistencia/Resistencia_Tref) + Cte_B) - 273.15;
  Temperatura2=Cte_B_2*Tref/(Tref*log(Resistencia2/Resistencia_Tref_2) + Cte_B_2) - 273.15;

  // Lo imprimimos en el Monitor Serie
  Serial.print("La temperatura con NTC47k vale: ");
  Serial.print(Temperatura,2);
  Serial.println("ºC.");
  Serial.print("La temperatura con NTC470k vale: ");
  Serial.print(Temperatura2,2);
  Serial.println("ºC.");
  Serial.println();
  
  delay(5000);  
}
