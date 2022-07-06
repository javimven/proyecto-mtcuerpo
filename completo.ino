#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>

// TARJETA SD
int CS = 10;   //Pin de entrada de la señal D9
File miFile;  //Nombre del archivo

// SENSOR DE RADIACIÓN uv
int UV = A7;          //Pin de entrada de la señal A7
long lecturaUV;      //Lectura del pin analógico
float Voltaje_UV;     // Valor que vamos a calcular
float Intensidad_UV;  // Valor que vamos a calcular

// PUENTE 1 NTC47k
int   NTC1 = A3;       // Pin analógico A3
long  lectura1;        // Lectura del pin analógico
float Vout_1;          // Valor que vamos a calcular
float Voltaje_AB;      // Valor que vamos a calcular
float Var_A;           // Valor intermedio para calcular resistencia
float R1 = 46980;      // Resistencia abajo del NTC
float R2 = 29280;      // Resistencia superior lado derecho
float R4 = 46850;      // Resistencia inferior lado derecho
float Resistencia;     // Valor que vamos a calcular
float Temperatura;     // Valor que vamos a calcular
float Resistencia_Tref = 54000; // R a temperatura de referencia 25º
float Cte_B = 4210;    // Constante B de temperatura

//PUENTE 2 NTC470k
int   NTC2 = A0;        // Pin analógico A0
long  lectura2;         // Lectura del pin analógico 0
float Vout_2;           // Valor que vamos a calcular
float Voltaje_AB_2;     // Valor que vamos a calcular
float Var_A_2;          // Valor intermedio para calcular resistencia
float R1_2 = 472900;    // Resistencia abajo del NTC
float R2_2 = 296000;    // Resistencia superior lado derecho
float R4_2 = 467500;    // Resistencia inferior lado derecho
float Resistencia2;     // Valor que vamos a calcular
float Temperatura2;     // Valor que vamos a calcular
float Resistencia_Tref_2 = 500000; // R a temperatura de referencia 25º
float Cte_B_2 = 4996;   // Constante B de temperatura

// CONSTANTES
float Tension_de_trabajo = 3.00;  // VCC 
float Tension_ref_ampli = 1.46;   // Vref (mitad que Vcc - divisor)
float Tension_ref_ampli2 = 1.34;  // Vref (mitad que Vcc - divisor)
float Ganancia = 2;               // Ganancia del amplificador
float Tref = 298.15;              // Temperatura de referencia 25º

void setup()
{
  Serial.begin(9600);

  pinMode(UV, INPUT);
  pinMode(NTC1, INPUT);
  pinMode(NTC2, INPUT);
  pinMode(CS, OUTPUT);
  
  SD.begin(CS);
    
  miFile = SD.open("archivo.txt", FILE_WRITE);
  if (miFile){
    miFile.println(" ");
    miFile.println("Nueva medida: ");
    miFile.close();
  }
}
 
void loop()

{
  Serial.flush();

  //Leemos los valores de las entradas analógicas 
  lecturaUV = lectura(UV);   //Pin análogico 7
  lectura1 = lectura(NTC1);  //Pin análogico 3
  lectura2 = lectura(NTC2);  //Pin análogico 0
  
  // Conversión A/D
  Voltaje_UV=(Tension_de_trabajo/1023.0)*lecturaUV;
  Vout_1=(Tension_de_trabajo/1023.0)*lectura1;
  Vout_2=(Tension_de_trabajo/1023.0)*lectura2;

  // Calculamos la diferencia de voltaje entre los terminales A y B
  Voltaje_AB=(Vout_1-Tension_ref_ampli)/Ganancia;
  Voltaje_AB_2=(Vout_2-Tension_ref_ampli2)/Ganancia;

   //Calculamos la resistencia desconocida con las fórmulas
  Var_A= R4/(R2+R4) - (Voltaje_AB/Tension_de_trabajo);
  Var_A_2= R4_2/(R2_2+R4_2) - (Voltaje_AB_2/Tension_de_trabajo);
  Resistencia=R1*(1/Var_A-1);
  Resistencia2=R1_2*(1/Var_A_2-1);

  //Calculamos la temperatura
  Temperatura=Cte_B*Tref/(Tref*log(Resistencia/Resistencia_Tref) + Cte_B) - 273.15;
  Temperatura2=Cte_B_2*Tref/(Tref*log(Resistencia2/Resistencia_Tref_2) + Cte_B_2) - 273.15;

  // Calculo de intensidad UV
  Intensidad_UV = Voltaje_UV*6.5; 

  //Abrimos el archivo archivo.txt
  miFile = SD.open("archivo.txt", FILE_WRITE);
 
  // Si el archivo se ha abierto correctamente
  if (miFile) {
    miFile.print(Temperatura);
    miFile.print(", ");
    miFile.print(Temperatura2);
    miFile.print(", ");
    miFile.println(Voltaje_UV);
    miFile.close();
  }

  Serial.print(Temperatura,2);
  Serial.print("|"); 
  Serial.print(Temperatura2,2);
  Serial.print("|"); 
  Serial.println(Voltaje_UV,2);
  
  delay(4000);
}

int lectura(int PinALeer)
{
  byte num_lecturas = 5;
  unsigned int lectura = 0; 
 
  for(int i = 0 ; i < num_lecturas ; i++){
    lectura+=analogRead(PinALeer);
    delay(100);
  }
  lectura=lectura/num_lecturas;
 
  return(lectura);
}
 
 
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
