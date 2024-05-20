// Author: Dorian Knight and Sharjil Mohsin
// Date: May 20th 2024
// Description: Main function to run EMG Simon Says

/* Global Defs */
// Legend
// Left bicep: 1
// Right bicep: 2
// Left calf: 3
// Right calf: 4
const int left_bicep_active = 1;
const int right_bicep_active = 2;
const int left_calf_active = 3;
const int right_calf_active = 4;

/* output_sequence */
int left_bicep_led = PD2;
int right_bicep_led = PD3;
int left_calf_led = PD4;
int right_calf_led = PD5;
int desired_led;
int array_len = 4;
//int sequence_array[4] = {0,3,1,2};

/* read_emg_input */
float left_bicep_emg_raw;
float right_bicep_emg_raw;
float left_calf_emg_raw;
float right_calf_emg_raw;

int left_bicep_input_pin = A0;
int right_bicep_input_pin = A1;
int left_calf_input_pin = A2;
int right_calf_input_pin = A3;

float threshold = 400;  // Tune to specific situation
const int total_samples = 50;

void setup() {
  // put your setup code here, to run once:

  /* output_sequence */
  Serial.begin(9600);
  pinMode(left_bicep_led, OUTPUT);  // Left bicep
  pinMode(right_bicep_led, OUTPUT);  // Right bicep
  pinMode(left_calf_led, OUTPUT);  // Left calf
  pinMode(right_calf_led, OUTPUT);  // Right calf
  pinMode(13, OUTPUT);

  /* read_emg_input */
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Reading Muscle 1");
  int active_muscle1 = read_emg_raw();
  delay(1000);
  Serial.println("Reading Muscle 2");
  int active_muscle2 = read_emg_raw();
  delay(1000);
  Serial.println("Reading Muscle 3");
  int active_muscle3 = read_emg_raw();
  delay(1000);
  Serial.println("Reading Muscle 4");
  int active_muscle4 = read_emg_raw();
  delay(1000);
  
  int mode = 2;
  int sequence_array[4] = {active_muscle1,active_muscle2,active_muscle3,active_muscle4};
  output_led_sequence(sequence_array, sizeof(sequence_array)/sizeof(int), mode);
}



void output_led_sequence(int sequence_array[], int array_len, int mode) {
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
    for (int i=0; i<array_len; i++){
      desired_led = sequence_array[i];
      switch (desired_led) {
        case left_bicep_active:  // Left bicep
          illuminate_led(left_bicep_led);
          break;
        case right_bicep_active:  // Right bicep
          illuminate_led(right_bicep_led);
          break;
        case left_calf_active:  // Left calf
          illuminate_led(left_calf_led);
          break;
        case right_calf_active:  // Right calf
          illuminate_led(right_calf_led);
          break;
        default:
          Serial.println("Default :)");
      }
    }
  }
}

void illuminate_led(int led_pin){
  digitalWrite(led_pin, HIGH);
  delay(1000);
  digitalWrite(led_pin, LOW);
  delay(200);
}

void output_result(int led_pin){
  for (int i=0; i<10; i++){
    digitalWrite(led_pin, HIGH);
    delay(100);
    digitalWrite(led_pin, LOW);
    delay(100);
  }
}

int read_emg_raw() {
  int active_muscle = 0;
  while (active_muscle == 0){
    // Serial.println("Reading emg inputs");
    // Initialize empty list for raw emg samples
    float emg_vals_left_bicep[total_samples];
    float emg_vals_right_bicep[total_samples];
    float emg_vals_left_calf[total_samples];
    float emg_vals_right_calf[total_samples];

    // Collect sample values across all muscle groups of interest
    for (int i=0; i<total_samples; i++){
      emg_vals_left_bicep[i] = analogRead(left_bicep_input_pin);
      emg_vals_right_bicep[i] = analogRead(right_bicep_input_pin);
      emg_vals_left_calf[i] = analogRead(left_calf_input_pin);
      emg_vals_right_calf[i] = analogRead(right_calf_input_pin);
    }

    // Average the raw sample values
    float emg_left_bicep_sum = 0;
    float emg_right_bicep_sum = 0;
    float emg_left_calf_sum = 0;
    float emg_right_calf_sum = 0;

    float emg_left_bicep_avg = 0;
    float emg_right_bicep_avg = 0;
    float emg_left_calf_avg = 0;
    float emg_right_calf_avg = 0;

    for (int j=0; j<total_samples; j++){
      emg_left_bicep_sum += emg_vals_left_bicep[j];
      emg_right_bicep_sum += emg_vals_right_bicep[j];
      emg_left_calf_sum += emg_vals_left_calf[j];
      emg_right_calf_sum += emg_vals_right_calf[j];
    }

    emg_left_bicep_avg = emg_left_bicep_sum/total_samples;
    emg_right_bicep_avg = emg_right_bicep_sum/total_samples;
    emg_left_calf_avg = emg_left_calf_sum/total_samples;
    emg_right_calf_avg = emg_right_calf_sum/total_samples;

    // Serial.println(emg_left_bicep_avg);
    // Serial.println(emg_right_bicep_avg);
    // Serial.println(emg_left_calf_avg);
    // Serial.println(emg_right_calf_avg);
    // Serial.println("\n\n\n");
    
    // Find which muscle was active above threshold
    if (emg_left_bicep_avg >= threshold){
      active_muscle = left_bicep_active;
    }
    else if(emg_right_bicep_avg >= threshold){
      active_muscle = right_bicep_active;
    }
    else if(emg_left_calf_avg >= threshold){
      active_muscle = left_calf_active;
    }
    else if (emg_right_calf_avg >= threshold){
      active_muscle = right_calf_active;
    }
    else{
      // No muscle is active
      active_muscle = 0;
    }
  }
  return active_muscle;
}
