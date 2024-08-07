int left_bicep_led = PD2;
int right_bicep_led = PD3;
int left_calf_led = PD4;
int right_calf_led = PD5;
int desired_led;
int array_len = 4;
String sequence_array = "0312";
int char_offset = 48; // Subtract 48 when converting from char to int

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
  int mode = 2;
  output_led_sequence(sequence_array, mode);
}



void output_led_sequence(String sequence_array, int mode) {
  // Mode 0: Indicate that the user was correct in their input sequence
  // Mode 1: Indicate that the user was incorrect in their input sequence
  // Other: Output the randomized LED sequence sent from main function
  
  if (mode == 0) {
    output_result(left_bicep_led);
  }

  else if (mode == 1) {
    output_result(right_bicep_led);
  }

  else {
    for (int i=0; i<sequence_array.length(); i++){
      desired_led = (int)sequence_array.charAt(i) - 48;
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
          Serial.print("Default: desired LED: ");
          Serial.println(desired_led);
      }
    }
  }
}

void illuminate_led(int led_pin){
  digitalWrite(led_pin, HIGH);
  delay(1000);
  digitalWrite(led_pin, LOW);
  delay(20);
}

void output_result(int led_pin){
  for (int i=0; i<10; i++){
    digitalWrite(led_pin, HIGH);
    delay(100);
    digitalWrite(led_pin, LOW);
    delay(100);
  }
}
