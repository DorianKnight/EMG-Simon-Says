String example_user_input = "0312";
String example_sequence = "0312";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool response_check = compare_user_to_sequence(example_user_input, example_sequence);
  Serial.print("User is correct: ");
  Serial.println(user_correct);
}

bool compare_user_to_sequence(String user_input, String sequence){
  return user_input == sequence;
}
