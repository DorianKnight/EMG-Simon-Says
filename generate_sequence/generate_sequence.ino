
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

String sequence = "";
void loop() {
  sequence = generate_string_sequence(sequence);
}

String generate_string_sequence(String prev_sequence, bool response_check) {
  // If the user is wrong, reset sequence
  if (response_check == false) {
    String new_sequence = (String)random(4);
  }

  // If the user passes previous level, concatenate new string with previous string
  else {
    String new_number = (String)random(4);
    String new_sequence = prev_sequence + new_number; 
  }

  return new_sequence;

}

void test_read_sequence(String sequence){
  for (int i=0; i<sequence.length(); i++){
    Serial.print("Charater: ");
    Serial.println(sequence.charAt(i));
  }
}
