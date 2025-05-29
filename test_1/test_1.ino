#include <hardware/adc.h>

//Configuración de pines
const int PIN_PULSO       = 22;
const int INPUT_OSC       = A2;
const int INPUT_RCV       = A1;

//Configuración de array de datos obtenidos
const int cant_lecturas   = 70;
const int MIN_DETEC_PULSO = 300;
int arr_lec_pulso[cant_lecturas];
int arr_lec_recvd[cant_lecturas];

//configuración de generación de pulsos
const int PWN_RANGE = 65535;
int duty            = round((PWN_RANGE/100)*50);
int frecuencia      = 8000;

//Configuración de visualización
int ZOOM          = 4;
int multiplicador_1 = 1;
int multiplicador_2 = 1;
int corte_max_val = 45000;
int med_skip      = 1;
int p_suavizado   = 0;

//////
int cnt = 0;
int sum = 0;
int max_ = 0;
int max_max_ = 0;
int min_min_ = 65000;
int min_r = 65000;
int min_2 = 65000;

void setup() {
  analogReadResolution(16);
  Serial.begin(500000);

  pinMode(PIN_PULSO, OUTPUT);
  analogWriteFreq(frecuencia);
  analogWriteRange(PWN_RANGE);
  analogWriteResolution(16);
  analogWrite(PIN_PULSO, duty);

  int j=0;
  while (1) {
    
   //if (analogRead(INPUT_OSC)> MIN_DETEC_PULSO){
      sum  = 0;
      max_ = 0;
      min_r = 65000;
      min_2 = 65000;

      for (cnt=0; cnt < cant_lecturas; cnt ++){
        arr_lec_pulso[cnt] = (analogRead(INPUT_OSC));
        arr_lec_recvd[cnt] = (digitalRead(22)*PWN_RANGE);
        /*if (arr_lec_recvd[cnt] > corte_max_val) { arr_lec_recvd[cnt] = corte_max_val; }
        if (arr_lec_pulso[cnt] > corte_max_val) { arr_lec_pulso[cnt] = corte_max_val; }
        if (arr_lec_recvd[cnt] < 0) { arr_lec_recvd[cnt] = 0; }
        if (arr_lec_pulso[cnt] < 0) { arr_lec_pulso[cnt] = 0; }*/
        //sum += arr_lec_recvd[cnt];
        if (arr_lec_recvd[cnt] > max_){ max_ = arr_lec_recvd[cnt]; }
        
        if (arr_lec_recvd[cnt] > max_max_){ max_max_ = arr_lec_recvd[cnt]; }
        if (arr_lec_pulso[cnt] > max_max_){ max_max_ = arr_lec_pulso[cnt]; }
        
        if (arr_lec_recvd[cnt] < min_min_){ min_min_ = arr_lec_recvd[cnt]; }
        if (arr_lec_pulso[cnt] < min_min_){ min_min_ = arr_lec_pulso[cnt]; }
        
        if (arr_lec_recvd[cnt] < min_r){ min_r = arr_lec_recvd[cnt]; }
        if (arr_lec_pulso[cnt] < min_2){ min_2 = arr_lec_pulso[cnt]; }
      }
      //Se suaviza la curva
      int cnt2 = 0;
      for (cnt2=0; cnt2 < p_suavizado; cnt2++){
        for (cnt=1; cnt < cant_lecturas; cnt ++){
          arr_lec_recvd[cnt] = (
            arr_lec_recvd[cnt-1]+
            arr_lec_recvd[cnt])/2;
          arr_lec_pulso[cnt] = (
            arr_lec_pulso[cnt-1]+
            arr_lec_pulso[cnt])/2;
        }
      }
      

       for (cnt=0; cnt < cant_lecturas; cnt +=med_skip){
          for (j=0; j < ZOOM; j++){
          Serial.print(arr_lec_pulso[cnt]);
          Serial.print(",");
          Serial.print(arr_lec_recvd[cnt]);
          Serial.print(",");
          //Serial.print(sum/cant_lecturas);
          //Serial.print(",");
          Serial.print(min_min_);
          //Serial.print(",");
          //Serial.print(max_max_);
          Serial.print("\n");
          }
        }
        delay(200);
    }    
    
  //}
}

void loop() {
  /* Nothing here */
}
