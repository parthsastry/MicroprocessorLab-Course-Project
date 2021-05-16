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

byte rotate(byte b, int r) {return (b << r) | (b >> (8-r));}

void pushLeftStack(byte bitToPush) {leftStack = (leftStack << 1) ^ bitToPush ^ leftStack;}
void pushRightStackRight(byte bitToPush) {rightStack = (rightStack >> 1) ^ (bitToPush << 7) ^ rightStack;}

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

void swap(uint8_t* a, uint8_t* b)  
{  
    uint8_t t = *a;  
    *a = *b;  
    *b = t;  
}  
int partition (uint8_t arr[], int low, int high)  
{  
    uint8_t pivot = arr[high]; // pivot  
    int i = (low - 1); // Index of smaller element  
  
    for (int j = low; j <= high - 1; j++)  
    {
        if (arr[j] < pivot)  
        {  
            i++;  
            swap(&arr[i], &arr[j]);  
        }  
    }  
    swap(&arr[i + 1], &arr[high]);  
    return (i + 1);  
}  

void quickSort(uint8_t arr[], int low, int high)  
{  
    if (low < high)  
    {  
        int pi = partition(arr, low, high);  
        quickSort(arr, low, pi - 1);  
        quickSort(arr, pi + 1, high);  
    }  
} 

float getZstatistic(byte arr[]) {
  byte temp_arr[num_reads*num_reads/8] = {0};
  for (int k = 0; k < num_reads*num_reads/8; k++) {temp_arr[k] = arr[k];}
  
  quickSort(arr, 0, num_reads*num_reads/8 - 1);
  uint8_t median = (arr[num_reads*num_reads/16] + arr[num_reads*num_reads/16 - 1])/2;

  int runs = 0;
  double n1 = 0, n2 = 0;

  for (int i = 1; i<num_reads*num_reads/8; i++) {
      if ((temp_arr[i] >= median && temp_arr[i-1] < median) || (temp_arr[i] < median && temp_arr[i-1] >= median)) {runs++;}
      if (temp_arr[i] >= median) {n1++;}
      else {n2++;}
  }

  double runs_exp = (2*n1*n2/(n1 + n2))+1;
  
  double stan_dev = sqrt( ( 2*n1*n2* (2*n1*n2-n1-n2) )/((n1+n2)*(n1+n2)*(n1+n2-1)));
  
  double z = (runs-runs_exp)/stan_dev; 
  
  return z;
}

void setup() {
  Serial.begin(19200);
  for(int iter1 = 0; iter1<num_reads; iter1++)
  {
    for(int iter2 = 0; iter2<num_reads/8; iter2++){bit_array[iter2*num_reads + iter1] = getTrueRotateRandomByte();}
  }
  Serial.println("Zcritical = 1.96");
  float Z = getZstatistic(bit_array);
  Serial.print("Z value of our RNG is - ");
  Serial.println(Z);
}

void loop() {}
