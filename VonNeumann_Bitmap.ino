
uint8_t val_read;
uint8_t val_read1;
uint8_t val_read2;
const int num_reads = 80;
byte analogPin = 3;
int counter = 0;
float delay_val = 0.512;

uint8_t bit_array[10*80] = {0};
// Vector<int> bit_array;

inline uint8_t get_bits(uint8_t data, int y){return ((data>>y) & 1);}

void setup() {
  Serial.begin(19200);
  int k = 0;
  for(int iter1=0; iter1<num_reads; iter1++)
  {
    for(int iter2=0; iter2<num_reads; iter2+=0)
    {
      val_read1=analogRead(analogPin) & 1;
      delay(delay_val);
      val_read2=analogRead(analogPin) & 1;
      if(val_read1!=val_read2)
      {
        val_read = val_read1;
        iter2++;
        bit_array[counter/8] += (val_read<<(counter%8));
        counter++;
      }
      delay(delay_val);
    }
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
