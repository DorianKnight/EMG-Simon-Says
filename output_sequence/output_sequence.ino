int left_bicep_led = PD2;
int right_bicep_led = PD3;
int left_calf_led = PD4;
int right_calf_led = PD5;
int desired_led;
int array_len = 4;
int sequence_array[4] = {0,3,1,2};

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  pinMode(left_bicep_led, OUTPUT);  // Left bicep
  pinMode(right_bicep_led, OUTPUT);  // Right bicep
  pinMode(left_calf_led, OUTPUT);  // Left calf
  pinMode(right_calf_led, OUTPUT);  // Right calf
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  output_led_sequence(sequence_array, array_len);
}



void output_led_sequence(int *sequence_array, int array_len) {
  for (int i=0; i<array_len; i++){
    desired_led = sequence_array[i];
    switch (desired_led) {
      case 0:  // Left bicep
        illuminate_led(left_bicep_led);
        break;
      case 1:  // Right bicep
        illuminate_led(right_bicep_led);
        break;
      case 2:  // Left calf
        illuminate_led(left_calf_led);
        break;
      case 3:  // Right calf
        illuminate_led(right_calf_led);
        break;
      default:
        Serial.println("Default :)");
    }
  }
}

void illuminate_led(int led_pin){
  digitalWrite(led_pin, HIGH);
  delay(1000);
  digitalWrite(led_pin, LOW);
  delay(20);
}
