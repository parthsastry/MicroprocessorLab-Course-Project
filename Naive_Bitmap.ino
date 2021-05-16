
uint8_t val_read;
const int num_reads = 80;
byte analogPin = 3;
int counter = 0;
float delay_val = 1.024;

uint8_t bit_array[10*80] = {0};
// Vector<int> bit_array;

inline uint8_t get_bits(uint8_t data, int y){return ((data>>y) & 1);}

void setup() {
  Serial.begin(19200);
  for (counter = 0; counter < num_reads*num_reads; counter++) {
      val_read = analogRead(analogPin) & 1;
      bit_array[counter/8] += (val_read<<(counter%8));
      delay(delay_val);
  }

  for (int iter1 = 0; iter1<num_reads;iter1++) {
    for(int iter2 = 0; iter2<num_reads/8; iter2++){
        for (int bits = 0; bits<8; bits++) {
          char c = get_bits(bit_array[iter2*num_reads + iter1], bits) ? ((char) 64) : ((char) 96);
          Serial.print(c);
        }
    }
    Serial.print('\n');
  }
  
}

void loop() {
 
}
