int sequence_array[4];
int array_len = 4;
// int* array;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // if (array_len == 0) {
  //   array = initiate_sequence();
  // }
  // else {
  //   array = add_on_sequence(array, array_len);
  // }  
  int* array = initiate_sequence();
  for (int i=0; i<array_len; i++) {
    Serial.print(array[i]);
    // Serial.println(*(array+i));
  }
  int* newArray = add_on_sequence(array, array_len);
  for (int i=0; i<array_len; i++) {
    Serial.print(newArray[i]);
    // Serial.println(*(array+i));
  }
  Serial.println(array_len);
  Serial.println("\n\n");
  delay(6000);
}

int* initiate_sequence() {
  for (int i=0; i<4; i++) {
    sequence_array[i] = random(4);
  }
  array_len = 4;
  return sequence_array;
}

int* add_on_sequence(int sequence_array[], int array_len) {
  int new_sequence_array[array_len+1];
  for (int i=0; i<array_len; i++) {
    new_sequence_array[i] = sequence_array[i];
  }
  new_sequence_array[array_len] = random(4);
  array_len = array_len + 1;
  return new_sequence_array;
}
