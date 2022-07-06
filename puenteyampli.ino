//CONSTANTES
float Tension_de_trabajo = 3.3; // VCC 
float Tref = 298.15;             // Temperatura de referencia 25º

//DIVISOR 2 (NTC470k)
const int   Pin_5 = A5;     // Pin analogico 5
int lectura2;              // Lectura en el pin analogico 5
float Voltaje_salida_2;    // Valor que vamos a calcular
float R3 = 473100;         // Resistencia abajo del NTC
float Resistencia2;        // Valor que vamos a calcular
float Temperatura2;        // Valor que vamos a calcular
//float Resistencia_Tref_2 = 470000; // R a temperatura de referencia 25º (fabricante)
//float Cte_B_2 = 5000;    // Constante B de temperatura (fabricante)
float Resistencia_Tref_2 = 503680; // R a temperatura de referencia 25º 
float Cte_B_2 = 4783.54;   // Constante B de temperatura 

//DIVISORES AÑADIDOS PARA EL PUENTE DE WHEATSTONE
float R2_2 = 296000;   // Resistencia arriba divisor (470k)
float R4_2 = 473900;   // Resistencia abajo divisor(470k)
float Var_A_2;         // Valor intermedio para calcular resistencia (470k)

//AMPLIFICADOR DE INSTRUMENTACIÓN
float Voltaje_AB_2;
float Tension_ref_ampli2 = 1.617;
float Ganancia = 2;

void setup() {
  Serial.begin(9600);
  pinMode(Pin_5, INPUT);
  Serial.println("Medición de resistencia y temperatura:");
  Serial.println();
}

void loop() {
  // Leemos los valores en el pin A5
  lectura2=0;
  for(int i=0;i<10;i++){
    lectura2+=analogRead(Pin_5);
    delay(100);
  }
  lectura2=lectura2/10;
  Serial.println(lectura2,2);
  
  // Conversión medida análogica a voltaje 
  Voltaje_salida_2=lectura2*(5/1024.0);
  // Lo imprimimos en el Monitor Serie
  Serial.print("Voltaje salida del amplificador NTC470k: ");
  Serial.print(Voltaje_salida_2,2);
  Serial.println(" Voltios");

  // Calculamos la diferencia de voltaje entre los terminales A y B
  Voltaje_AB_2=(Voltaje_salida_2-Tension_ref_ampli2)/Ganancia;
   Serial.print("Voltaje salida del puente NTC470k: ");
  Serial.print(Voltaje_AB_2,2);
  Serial.println(" Voltios");
  
  //Calculamos la resistencia desconocida con las fórmulas
  Var_A_2= R4_2/(R2_2+R4_2) - (Voltaje_AB_2/Tension_de_trabajo);
  Resistencia2=R3*(1/Var_A_2-1);
  // Lo imprimimos en el Monitor Serie
  Serial.print("La resistencia del NTC470k vale: ");
  Serial.print(Resistencia2,2);
  Serial.println(" Ohm.");

  //Calculamos la temperatura
  Temperatura2=Cte_B_2*Tref/(Tref*log(Resistencia2/Resistencia_Tref_2) + Cte_B_2) - 273.15;
  // Lo imprimimos en el Monitor Serie
  Serial.print("La temperatura con NTC470k vale: ");
  Serial.print(Temperatura2,2);
  Serial.println("ºC.");
  Serial.println();
  
  delay(5000);  
}
