#include <SoftwareSerial.h>
#include <SD.h>

// TARJETA SD
int CS = 10;   //Pin de entrada de la señal D9
File miFile;  //Nombre del archivo

//CONSTANTES
float Tension_de_trabajo = 3.3; // VCC 
float Tref = 298.15;             // Temperatura de referencia 25º

// DIVISOR 1 (NTC47k)
int   Pin_7= A7;        // Pin analogico 7
float  lectura1;        // Lectura en el pin analogico 7
float Voltaje_salida;  // Valor que vamos a calcular
float R1 = 47530;      // Resistencia abajo del NTC
float Resistencia;     // Valor que vamos a calcular
float Temperatura;     // Valor que vamos a calcular
//float Resistencia_Tref = 47000; // R a temperatura de referencia 25º (fabricante)
//float Cte_B = 4090;             // Constante B de temperatura (fabricante)
float Resistencia_Tref = 48280;   // R a temperatura de referencia 25º 
float Cte_B = 3994.52;            // Constante B de temperatura 

//DIVISOR 2 (NTC470k)
int   Pin_6 = A6;        // Pin analogico 6
float  lectura2;         // Lectura en el pin analogico 6
float Voltaje_salida_2;  // Valor que vamos a calcular
float R3 = 473100;       // Resistencia abajo del NTC
float Resistencia2;      // Valor que vamos a calcular
float Temperatura2;      // Valor que vamos a calcular
//float Resistencia_Tref_2 = 470000; // R a temperatura de referencia 25º (fabricante)
//float Cte_B_2 = 5000;              // Constante B de temperatura (fabricante)
float Resistencia_Tref_2 = 503680;   // R a temperatura de referencia 25º 
float Cte_B_2 = 4783.54;             // Constante B de temperatura 
 
//DIVISORES AÑADIDOS PARA EL PUENTE DE WHEATSTONE
float R2 = 29260;      // Resistencia arriba divisor (47k)
float R4 = 46940;      // Resistencia abajo divisor (47k)
float R2_2 = 296000;   // Resistencia arriba divisor (470k)
float R4_2 = 473900;   // Resistencia abajo divisor(470k)
float Var_A;           // Valor intermedio para calcular resistencia (47k)
float Var_A_2;         // Valor intermedio para calcular resistencia (470k)

//AMPLIFICADOR DE INSTRUMENTACIÓN
float Voltaje_AB;    // Valor que vamos a calcular
float Voltaje_AB_2;  // Valor que vamos a calcular
float Tension_ref_ampli2 = 1.617;   //Tension de referencia de los amplificadores
float Ganancia = 2;   // Ganancia del amplificador

// SENSOR UV
float Rf = 102.8;        //Resistencia de ganancia de transimpedancia
int Pin_5 = A5;          //Pin análogico 5
float lecturaUV;         // Variable para lectura del pin analogico 5
float Voltaje_salidaUV;  // Valor que vamos a calcular
float Corriente_inversa; // Valor que vamos a calcular
float IndiceUV;          // Valor que vamos a calcular


void setup() {
  Serial.begin(9600);
  pinMode(Pin_5, INPUT);
  pinMode(Pin_6, INPUT);
  pinMode(Pin_7, INPUT);
  pinMode(CS, OUTPUT);
  
  SD.begin(CS);
  miFile = SD.open("archivo.txt", FILE_WRITE);
  if (miFile){
    miFile.println(" ");
    miFile.println("Nueva medida: ");
    miFile.close();
  }
  
}

void loop() {
  // Leemos los valores en el pin A7, A6, A1 y A0
  lectura1=0;
  lectura2=0;
  lecturaUV=0;
  for(int i=0;i<10;i++){
    lectura1+=analogRead(Pin_6);
    lectura2+=analogRead(Pin_7);
    lecturaUV+=analogRead(Pin_5);
    delay(100);
  }
  lectura1=lectura1/10;
  lectura2=lectura2/10;
  lecturaUV=lecturaUV/10;
  
  // Conversión medida análogica a voltaje
  Voltaje_salida=lectura1*(5/1024.0);
  Voltaje_salida_2=lectura2*(5/1024.0);
  Voltaje_salidaUV = (Tension_de_trabajo/1023.0)*lecturaUV;

  // Calculamos la diferencia de voltaje entre los terminales A y 
  Voltaje_AB=(Voltaje_salida-Tension_ref_ampli2)/Ganancia;
  Voltaje_AB_2=(Voltaje_salida_2-Tension_ref_ampli2)/Ganancia;
  
  //Calculamos la resistencia desconocida con las fórmulas
  Var_A= R4/(R2+R4) - (Voltaje_salida/Tension_de_trabajo);
  Var_A_2= R4_2/(R2_2+R4_2) - (Voltaje_salida_2/Tension_de_trabajo);
  Resistencia=R1*(1/Var_A-1);
  Resistencia2=R3*(1/Var_A_2-1);

  //Calculamos la temperatura
  Temperatura=Cte_B*Tref/(Tref*log(Resistencia/Resistencia_Tref) + Cte_B) - 273.15;
  Temperatura2=Cte_B_2*Tref/(Tref*log(Resistencia2/Resistencia_Tref_2) + Cte_B_2) - 273.15;

  //Calculamos la corriente inversa en uA
  Corriente_inversa = Voltaje_salidaUV/Rf*1000;

  //Calculamos el índice UV a partir de la corriente inversa
  IndiceUV = 0.9142*Corriente_inversa-2.1364;

   if (IndiceUV<0) 
      IndiceUV=0.0;

  //Abrimos el archivo archivo.txt
  miFile = SD.open("archivo.txt", FILE_WRITE);
 
  // Si el archivo se ha abierto correctamente se envían las medidas
  if (miFile) {
    miFile.print(Temperatura);
    miFile.print(", ");
    miFile.print(Temperatura2);
    miFile.print(", ");
    miFile.println(IndiceUV);
    miFile.close();
  }

  // Transmisión de los datos por BT
  Serial.print(Temperatura,2);
  Serial.print("|"); 
  Serial.print(Temperatura2,2);
  Serial.print("|"); 
  Serial.println(IndiceUV,2);

  delay(5000);  
}
