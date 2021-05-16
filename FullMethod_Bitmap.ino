const float waitTime = 0.512;
const int num_reads = 80;
const byte analogPin = 3;
uint8_t bit_array[num_reads*num_reads/8] = {0};

byte lastByte = 0;
byte leftStack = 0;
byte rightStack = 0;

inline uint8_t get_bits(uint8_t data, int y){return ((data>>y) & 1);}

float byte_to_float(byte byte_convert)
{
  float float_convert = 0;
  for(int i = 0; i<8; i++)
  {
    float_convert += ((byte_convert >> i) & 1)/((float)pow(2, 8-i));
  }
  return float_convert;
}

byte rotate(byte b, int r) {
  return (b << r) | (b >> (8-r));
}

void pushLeftStack(byte bitToPush) {
  leftStack = (leftStack << 1) ^ bitToPush ^ leftStack;
}
void pushRightStackRight(byte bitToPush) {
  rightStack = (rightStack >> 1) ^ (bitToPush << 7) ^ rightStack;
}

// contrary to previous code, the random byte contains oldest value at MSB and newest value at LSB
byte getTrueRotateRandomByte() {
  byte finalByte = 0;
  
  byte lastStack = leftStack ^ rightStack;
  
  for (int i=0; i<4; i++) {
    delayMicroseconds(waitTime);
    int leftBits = analogRead(analogPin);
    
    delayMicroseconds(waitTime);
    int rightBits = analogRead(analogPin);
    
    finalByte ^= rotate(leftBits, i);
    finalByte ^= rotate(rightBits, 7-i);
    
    for (int j=0; j<8; j++) {
      byte leftBit = (leftBits >> j) & 1;
      byte rightBit = (rightBits >> j) & 1;
  
      if (leftBit != rightBit) {
        if (lastStack % 2 == 0) {
          pushLeftStack(leftBit);
        } else {
          pushRightStackRight(leftBit);
        }
      }
    }
    
  }
  lastByte ^= (lastByte >> 3) ^ (lastByte << 5) ^ (lastByte >> 4);
  lastByte ^= finalByte;
  
  return lastByte ^ leftStack ^ rightStack;
}

void setup() {
  Serial.begin(19200);
  for(int iter1 = 0; iter1<num_reads; iter1++)
  {
    for(int iter2 = 0; iter2<num_reads/8; iter2++)
    {
      bit_array[iter2*num_reads + iter1] = getTrueRotateRandomByte();
      for (int bits = 0; bits<8; bits++)
      {
          char c = get_bits(bit_array[iter2*num_reads + iter1], bits) ? ((char) 64) : ((char) 96);
          Serial.print(c);
      }
    }
    Serial.print('\n');
  }
}

void loop() {

}
