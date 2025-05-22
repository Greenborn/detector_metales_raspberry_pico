#include <hardware/adc.h>

const int cant_ciclos     = 200;
const int PIN_PULSO       = 22;
const int INPUT_OSC       = A0;
const int INPUT_RCV       = A1;
const int MIN_DETEC_PULSO = 3000;

int duty          = 65535/20;
int multiplicador = 1;
int frecuencia    = 200;

int cnt = 0;
int arr_1[cant_ciclos];
int arr_2[cant_ciclos];

int sum = 0;
int max_ = 0;
int min_ = 65000;

void setup() {
  analogReadResolution(16);
  Serial.begin(500000);

  pinMode(PIN_PULSO, OUTPUT);
  analogWriteFreq(frecuencia);
  analogWriteRange(65535);
  analogWriteResolution(16);
  analogWrite(PIN_PULSO, duty);

  int j=0;
  while (1) {
    
    if (analogRead(INPUT_OSC)> MIN_DETEC_PULSO){
      sum = 0;
      for (cnt=0; cnt < cant_ciclos; cnt ++){
        arr_1[cnt] = analogRead(INPUT_OSC);
        arr_2[cnt] = analogRead(INPUT_RCV)*multiplicador;
        if (arr_1[cnt] > 50){
          //arr_2[cnt] = 0;
        }
        sum += arr_2[cnt];
        if (arr_2[cnt] > max_){ max_ = arr_2[cnt]; }
        if (arr_2[cnt] < min_){ min_ = arr_2[cnt]; }
      }  
    }    
    
    for (cnt=0; cnt < cant_ciclos; cnt ++){
      for (j=0; j < 1; j++){
      Serial.print(arr_1[cnt]);
      Serial.print(",");
      Serial.print(arr_2[cnt]);
      Serial.print(",");
      Serial.print(sum/cant_ciclos);
      Serial.print(",");
      Serial.print(min_);
      Serial.print(",");
      Serial.print(max_);
      Serial.print("\n");
      }
    }
    delay(250);
  }
}

void loop() {
  /* Nothing here */
}
