int num_reads = 80; 

uint16_t x = 0xb5ad;
uint8_t compare = UINT8_MAX /   2;

inline static uint8_t ms() {
    x *= x; 
    return x = (x>>8) | (x<<8);
}

void setup() {
  Serial.begin(19200);
  for (int iter1 = 0; iter1 < num_reads; iter1++) {
    for(int iter2 = 0; iter2 < num_reads; iter2++) {
      char c = (ms() > compare) ? ((char) 64) : ((char) 32);
      Serial.print(c);
    }
    Serial.print('\n');
  }
}

void loop() {}
