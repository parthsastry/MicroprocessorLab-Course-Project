const float waitTime = 0.512;
const int num_reads = 200;
const double num_reads_d = 200;
const byte analogPin = 3;
double double_array[num_reads] = {0};

byte lastByte = 0;
byte leftStack = 0;
byte rightStack = 0;

double byte_to_double(byte byte_convert)
{
  return byte_convert / 256.0;
}

byte rotate(byte b, int r) {
  return (b << r) | (b >> (8 - r));
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

  for (int i = 0; i < 4; i++) {
    delayMicroseconds(waitTime);
    int leftBits = analogRead(analogPin);

    delayMicroseconds(waitTime);
    int rightBits = analogRead(analogPin);

    finalByte ^= rotate(leftBits, i);
    finalByte ^= rotate(rightBits, 7 - i);

    for (int j = 0; j < 8; j++) {
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
double get_c(double arr[])
{
  double sum_duo = arr[0] * arr[num_reads - 1], sum_squares = arr[num_reads - 1] * arr[num_reads - 1], sum = arr[num_reads - 1], c;
  for (int i = 0; i < num_reads - 1; i++)
  {
    sum_duo += arr[i] * arr[i + 1];
    sum_squares += arr[i] * arr[i];
    sum += arr[i];
  }
  c = ((num_reads * sum_duo) - (sum * sum)) / ((num_reads * sum_squares) - (sum * sum));
  return c;
}

void setup() {
  Serial.begin(19200);
  for (int iter = 0; iter < num_reads; iter++)
  {
    double_array[iter] = byte_to_double(getTrueRotateRandomByte());
  }
  double mu = -1/(num_reads_d-1);
  double sigma = (-1*mu)*sqrt((num_reads_d)*(num_reads_d-3)/(num_reads_d+1));
  Serial.print("95% confidence interval is ");
  Serial.print(mu-2*sigma);
  Serial.print(' ');
  Serial.print(mu+2*sigma);
  Serial.print('\n');
  Serial.print("Our Serial Correlation Coefficient is ");
  Serial.println(get_c(double_array));
}

void loop() {}
