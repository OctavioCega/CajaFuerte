//Creado por Ryutaku versión 2.0
// ryutaku.com
//Programa para una caja fuerte en arduino
//Para diagramas visitar ryutaku.com
//200415
#include <Keypad.h> // Controla el teclado
#include <LiquidCrystal.h>  //controla el LCD
#include <Servo.h>  //Control del servomotor
#include <EEPROM.h>  //COntrola E/S EEPROM
#define CERRAR 90 //Calibrar a gusto la pos del servomotor cerrado
#define ABRIR 180 //Calibrar a gusto la pos del servomotor abierto
//*****************************************
//*** Declaracion de variables locales*****
//*****************************************
Servo seguro;   //servomotor
LiquidCrystal lcd(2 , 3, 4, 5, 6, 7); //display LCD
const byte filas = 4;
const byte columnas = 4;
byte pinsFilas[filas] = {9, 10, 11, 12};
byte pinsColumnas[columnas] = {14, 15, 16, 17};
char teclas[filas][columnas] =  {
  {'1', '2', '3', 'A'},                       // Declaración del teclado
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};
Keypad teclado = Keypad(makeKeymap(teclas), pinsFilas, pinsColumnas, filas, columnas);
char password[5]; //almacena la contraseña en eeprom
char ingreso;  //ingreso del usuario
char passUser[4];
char confirmPass[4];  //confirmacion de la contraseña
//char ca[7] = {'3', '7', '1', '9', '2', 'A'}; //clave admin establecida
//char cal[7];   // Clave admin leida
//int contador = 0; //Lleva la posicion del array
//int cursorr = 6; //Lleva la posicion del cursor
//int comp;  // comparacion entre 2 arrays
int i=0;
int a; //aux
void setup() {
  pinMode(19, OUTPUT); // LEDS QUE INDICAN ABIERTO(13) O CERRADO(19)
  pinMode(13, OUTPUT);
  digitalWrite(19, HIGH); //enciende el led de cerrado
  seguro.attach(18);  // Pin del servomotor
  seguro.write(CERRAR);
  lcd.begin(16, 2);   //Configuracion lcd 16X2 (columnas,fila)
  seguro.write(90);  //Cerrar puerta
  lcd.setCursor(0, 0);
  lcd.print("  **PASSWORD** ");
  lcd.setCursor(5, 1);
  lcd.print("____");
  //LEER CONTRASEÑA DE LA EEPROM 

//        error if(password[4]!='Z'){     
    //correción
    if(EEPROM.read(4)!='Z'){
    EEPROM.write(0,'1');
    EEPROM.write(1,'2');
    EEPROM.write(2,'3');
     EEPROM.write(3,'4');
    EEPROM.write(4,'Z'); 
  }
   for (int i = 0; i <= 4; i++) { 
    password[i] = EEPROM.read(i);
  }              
  
}//fin del setup

void loop() {
    leerIngreso(1);
}
 




////////////////////////////////////////////

void leerIngreso(int a) {
  ingreso = teclado.getKey();
  if (ingreso != NO_KEY)
    switch (ingreso) {
      case 'A':    // es como el "enter" para introducir la password
        if(evaluar(1)==1)
        correcto();
        else{
        msgError();
        }
        reset();
        break;
        
      case 'B':
        informacion();  //muestra en el lcd las opcionesde la caja fuerte
        reset();
        break;
        
      case 'C':
        cambioPass();
        reset();
        break;
      case 'D':
        lcd.setCursor(0, 0);
        lcd.print("Presionaste D");
        delay(300);
        seguro.write(CERRAR);
        reset();
        break;
      case '*':
        lcd.setCursor(0, 0);
        lcd.print("Presionaste *");
        delay(300);
        reset();
        break;
      case '#':
        lcd.setCursor(0, 0);
        lcd.print("Presionaste #");
        delay(300);
        reset();
        break;
      default: //si es un numero debe imprimirlo en el LCD y ademas guardarlo en el arreglo passUser
        if(a==1){
        passUser[i] = ingreso;
        printPass(passUser[i], 5 + i, 1);
        }
        if(a==2){
        confirmPass[i] = ingreso;
        printPass(confirmPass[i], 5 + i, 1);
        }
        i++;
        if (i > 3)
          i = 0;
   
}
}

void cleanlcd() {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}


void printPass(char a, int columna, int fila ) {
  lcd.setCursor(columna, fila);
  lcd.print(a);
  delay(100);
  lcd.setCursor(columna, fila);
  lcd.print("*");
}

int evaluar(int a) {
  int j = 0;
  if (a==1){
  for (int i = 0; i <= 3; i++) {
    if (password[i] == passUser[i]) {
        j++;
    }
  }
  }
  if(a==2){
  for (int i = 0; i <= 3; i++) {
    if (passUser[i] == confirmPass[i]) {
        j++;
    }
  }
  }
  if (j == 4) {
    return j=1;
  }
  else {
    return j=0;
  }
}


void reset() {
  lcd.setCursor(0, 0);
  lcd.print("  **PASSWORD** ");
  lcd.setCursor(5, 1);
  lcd.print("____");
  for(int i =0;i<=3;i++){
  passUser[i]=NO_KEY;
  confirmPass[i]=NO_KEY;
  }
  i=0;
}

void msgError(){
  
        lcd.setCursor(0, 0);
        lcd.print("      ERROR      ");
        delay(500);
      
    
  }

void correcto(){
  
        lcd.setCursor(0, 0);
        lcd.print("     CORRECTO      ");
        //Abrir servomotor
        seguro.write(ABRIR);
        delay(500);
}

  
void informacion() {
  lcd.setCursor(0, 0);
  lcd.print("'A' para introdu");
  delay(200);
  lcd.setCursor(0, 0);
  lcd.print("cir la pass        ");
  delay(200);
  lcd.setCursor(0, 0);
  lcd.print("'C' para cambiar");
  delay(200);
  lcd.setCursor(0, 0);
  lcd.print(" la pass             ");
  delay(200);
}



void cambioPass() {
   lcd.setCursor(0, 0);
  lcd.print("Cambio de pass    ");
  delay(200);
  lcd.setCursor(0, 0);
  lcd.print("Introduce pass     ");
  delay(200);
  lcd.setCursor(0, 0);
  lcd.print("anterior        ");
  delay(200);  
  reset();
   while(passUser[3]==NO_KEY){
    leerIngreso(1);}
  if (evaluar(1) == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Introduce           ");
    delay(200);
    lcd.setCursor(0, 0);
    lcd.print("la pass nueva            ");
    delay(200);
    reset();
    while(passUser[3]==NO_KEY){
      leerIngreso(1);
  }
    lcd.setCursor(0, 0);
    lcd.print("Vuelve a intro     ");
    delay(200);
    lcd.setCursor(0, 0);
    lcd.print("cirla           ");
    delay(200);    
    lcd.setCursor(0, 0);
    lcd.print("  **PASSWORD**      ");
    lcd.setCursor(5, 1);
    lcd.print("____");
    lcd.setCursor(0, 0);
    i=0;
    while(confirmPass[3]==NO_KEY){
      leerIngreso(2);
  }
    if(evaluar(2)==1){
      // funcion de EEPROM
      for (int i = 0; i <= 3; i++) { 
         EEPROM.write(i, passUser[i]);
    }
      for (int i = 0; i <= 3; i++) { 
         password[i] = EEPROM.read(i);
  }
      lcd.setCursor(0, 0);
      lcd.print("Contraseña cam  ");
      delay(200);
      lcd.setCursor(0, 0);
      lcd.print("biada           ");
      delay(200);
    }
    else{
      lcd.setCursor(0, 0);
      lcd.print("Error las pass   ");
      delay(200);
      lcd.setCursor(0, 0);
      lcd.print("no coinciden        ");
      delay(200);
    }
    
  }
  else {
    msgError();
  }
  reset();
}

void passChange(){
 for(int i=0;i<=3;i++){
   password[i]=passUser[i];
 }
  
}


