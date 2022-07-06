//CONSTANTES
float Tension_de_trabajo = 3.3; // VCC 
float Tref = 298.15;             // Temperatura de referencia 25º

// DIVISOR 1 (NTC47k)
int   Pin_1= A1;        // Pin analogico 1
float  lectura1;        // Lectura en el pin analogico 1
float Voltaje_salida;  // Valor que vamos a calcular
float R1 = 47530;      // Resistencia abajo del NTC
float Resistencia;     // Valor que vamos a calcular
float Temperatura;     // Valor que vamos a calcular
//float Resistencia_Tref = 47000; // R a temperatura de referencia 25º (fabricante)
//float Cte_B = 4090;    // Constante B de temperatura (fabricante)
float Resistencia_Tref = 48280; // R a temperatura de referencia 25º 
float Cte_B = 3994.52;    // Constante B de temperatura 

//DIVISOR 2 (NTC470k)
int   Pin_7 = A7;        // Pin analogico 7
float  lectura2;         // Lectura en el pin analogico 7
float Voltaje_salida_2; // Valor que vamos a calcular
float R3 = 473100;      // Resistencia abajo del NTC
float Resistencia2;     // Valor que vamos a calcular
float Temperatura2;     // Valor que vamos a calcular
//float Resistencia_Tref_2 = 470000; // R a temperatura de referencia 25º (fabricante)
//float Cte_B_2 = 5000;   // Constante B de temperatura (fabricante)
float Resistencia_Tref_2 = 503680; // R a temperatura de referencia 25º 
float Cte_B_2 = 4783.54;   // Constante B de temperatura 


//DIVISORES AÑADIDOS PARA EL PUENTE DE WHEATSTONE
float R2 = 29260;      // Resistencia arriba divisor (47k)
float R4 = 46940;      // Resistencia abajo divisor (47k)
int   Pin_0= A0;       // Pin analogico 0 (divisor puente 47k)
float R2_2 = 296000;   // Resistencia arriba divisor (470k)
float R4_2 = 473900;   // Resistencia abajo divisor(470k)
int   Pin_6= A6;       // Pin analogico 6 puente (divisor puente 470k)
float Var_A;           // Valor intermedio para calcular resistencia (47k)
float Var_A_2;         // Valor intermedio para calcular resistencia (470k)


void setup() {
  Serial.begin(9600);
  pinMode(Pin_1, INPUT);
  pinMode(Pin_7, INPUT);
  pinMode(Pin_0, INPUT);
  pinMode(Pin_6, INPUT);
  Serial.println("Medición de resistencia y temperatura:");
  Serial.println();
}

void loop() {
  // Leemos los valores en el pin A7, A6, A1 y A0
  lectura1=0;
  lectura2=0;
  for(int i=0;i<10;i++){
    lectura1+=analogRead(Pin_0)-analogRead(Pin_1);
    lectura2+=analogRead(Pin_6)-analogRead(Pin_7);
    delay(100);
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
  Var_A= R4/(R2+R4) - (Voltaje_salida/Tension_de_trabajo);
  Var_A_2= R4_2/(R2_2+R4_2) - (Voltaje_salida_2/Tension_de_trabajo);
  Resistencia=R1*(1/Var_A-1);
  Resistencia2=R3*(1/Var_A_2-1);

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
