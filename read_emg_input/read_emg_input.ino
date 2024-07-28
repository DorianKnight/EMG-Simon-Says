// Author: Dorian Knight
// Date: May 5th 2024
// Description: Blocking call to read active muscle

float left_bicep_emg_raw;
float right_bicep_emg_raw;
float left_calf_emg_raw;
float right_calf_emg_raw;

int left_bicep_input_pin = A0;
int right_bicep_input_pin = A1;
int left_calf_input_pin = A2;
int right_calf_input_pin = A3;

const char left_bicep_active  = '0';
const char right_bicep_active = '1';
const char left_calf_active   = '2';
const char right_calf_active  = '3';

float threshold = 600;  // Tune to specific situation
const int total_samples = 50;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  char active_muscle = read_emg_raw();
  Serial.println(active_muscle);
}

char read_emg_raw() {
  bool muscle_acquired = false;
  char active_muscle;
  while (!muscle_acquired){
    Serial.println("Reading emg inputs");
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

    Serial.println(emg_left_bicep_avg);
    Serial.println(emg_right_bicep_avg);
    Serial.println(emg_left_calf_avg);
    Serial.println(emg_right_calf_avg);
    Serial.println("\n\n\n");
    
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


