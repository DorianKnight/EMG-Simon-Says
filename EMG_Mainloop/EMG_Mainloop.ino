// Author: Dorian Knight and Sharjil Mohsin
// Date: May 20th 2024
// Description: Main function to run EMG Simon Says

/* Global Defs */
/* output_sequence */

// Legend
// Left bicep: 1
// Right bicep: 2
// Left calf: 3
// Right calf: 4
const char left_bicep_active  = '0';
const char right_bicep_active = '1';
const char left_calf_active   = '2';
const char right_calf_active  = '3';

int left_bicep_led    = PD2;
int right_bicep_led   = PD3;
int left_calf_led     = PD4;
int right_calf_led    = PD5;
char desired_led;
int array_len = 4;
String sequence_array = "0312";

int output_wrong = 0;
int output_right = 1;
int output_pattern = 2;

/* read_emg_input */
float left_bicep_emg_raw;
float right_bicep_emg_raw;
float left_calf_emg_raw;
float right_calf_emg_raw;

int left_bicep_input_pin  = A0;
int right_bicep_input_pin = A1;
int left_calf_input_pin   = A2;
int right_calf_input_pin  = A3;

float threshold = 600;  // Tune to specific situation
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

bool response_check = true;
String prev_sequence = "";
String sequence;
String user_input_sequence;

void loop() {
  // Generate a sequence to output
 sequence = generate_string_sequence(prev_sequence, response_check);

  // Output the sequence
  output_led_sequence(sequence, output_pattern);

  // Read user input
  user_input_sequence = "";
  for (int i=0; i<sequence.length(); i++) {
    Serial.println("Reading Muscle " + (String)i);
    char user_input = read_emg_raw();
    user_input_sequence = user_input_sequence + read_emg_raw();

    // Check if user input matches
    response_check = compare_user_to_sequence(user_input_sequence, sequence, user_input_sequence.length());
    //Serial.println("Response check: " + (String) response_check);
    if (!response_check) {
      break;
    }
  }
  

  if (response_check == false) {
    // Tell the user that they're wrong
    output_led_sequence(sequence, output_wrong);
  }

  else {
    output_led_sequence(sequence, output_right);
  }

  prev_sequence = sequence;
}


void output_led_sequence(String sequence_array, int mode) {
  // Mode 0: Indicate that the user was correct in their input sequence
  // Mode 1: Indicate that the user was incorrect in their input sequence
  // Other: Output the randomized LED sequence sent from main function
  
  if (mode == 0) {
    output_result(left_bicep_led);
  }

  else if (mode == 1) {
    output_result(left_calf_led);
  }

  else {
    for (int i=0; i<sequence_array.length(); i++){
      desired_led = sequence_array.charAt(i);
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

char read_emg_raw() {
  bool muscle_acquired = false;
  char active_muscle;
  while (!muscle_acquired){
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
      muscle_acquired = true;
    }
    else if(emg_right_bicep_avg >= threshold){
      active_muscle = right_bicep_active;
      muscle_acquired = true;
    }
    else if(emg_left_calf_avg >= threshold){
      active_muscle = left_calf_active;
      muscle_acquired = true;
    }
    else if (emg_right_calf_avg >= threshold){
      active_muscle = right_calf_active;
      muscle_acquired = true;
    }
  }
  delay(1000);
  return active_muscle;
}

bool compare_user_to_sequence(String user_input, String sequence, int stop_compare_index){
  bool match_held = true;
  for (int i=0; i<stop_compare_index; i++) {
    match_held = user_input.charAt(i) == sequence.charAt(i);
    if (!match_held) {
      break;
    }
  }
  return match_held;
}

String generate_string_sequence(String prev_sequence, bool response_check) {
  // If the user is wrong, reset sequence
  String new_sequence;
  if (response_check == false) {
    new_sequence = (String)random(4);
  }

  // If the user passes previous level, concatenate new string with previous string
  else {
    String new_number = (String)random(4);
    new_sequence = prev_sequence + new_number; 
  }

  return new_sequence;

}
