/*
  counter8 with SystemC Co-Emulation
*/

#define PIN_CLK     22
#define PIN_nCLR_L  23
#define PIN_nCLR_H  24
#define PIN_nLOAD_L 25
#define PIN_nLOAD_H 26
#define PIN_ENP     27
#define PIN_ENT     28
#define PIN_Din_0   29
#define PIN_Din_1   30
#define PIN_Din_2   31
#define PIN_Din_3   32
#define PIN_Din_4   33
#define PIN_Din_5   34
#define PIN_Din_6   35
#define PIN_Din_7   36
#define PIN_Dout_0  37
#define PIN_Dout_1  38
#define PIN_Dout_2  39
#define PIN_Dout_3  40
#define PIN_Dout_4  41
#define PIN_Dout_5  42
#define PIN_Dout_6  43
#define PIN_Dout_7  44
#define PIN_RCO     45

//--------------------------------------------------------------------
void establishContact()
{
  while (Serial.available() <= 0)
  {
    Serial.print('A');  // send a capital A
    delay(300);
    if (Serial.read()==(int)'A')
      break;
  }
}
//------------------------------------------------------------------------------
void setup()
{
  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial)
  {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  establishContact();  // send a byte to establish contact until receiver responds

  // Set digital pins to output connecting DUT's INPUT
  pinMode(PIN_CLK     , OUTPUT);  digitalWrite(PIN_CLK     , LOW);
  pinMode(PIN_nCLR_L  , OUTPUT);  digitalWrite(PIN_nCLR_L  , LOW);
  pinMode(PIN_nCLR_H  , OUTPUT);  digitalWrite(PIN_nCLR_H  , LOW);
  pinMode(PIN_nLOAD_L , OUTPUT);  digitalWrite(PIN_nLOAD_L , LOW);
  pinMode(PIN_nLOAD_H , OUTPUT);  digitalWrite(PIN_nLOAD_H , LOW);
  pinMode(PIN_ENP     , OUTPUT);  digitalWrite(PIN_ENP     , LOW);
  pinMode(PIN_ENT     , OUTPUT);  digitalWrite(PIN_ENT     , LOW);
  pinMode(PIN_Din_0   , OUTPUT);  digitalWrite(PIN_Din_0   , LOW);
  pinMode(PIN_Din_1   , OUTPUT);  digitalWrite(PIN_Din_1   , LOW);
  pinMode(PIN_Din_2   , OUTPUT);  digitalWrite(PIN_Din_2   , LOW);
  pinMode(PIN_Din_3   , OUTPUT);  digitalWrite(PIN_Din_3   , LOW);
  pinMode(PIN_Din_4   , OUTPUT);  digitalWrite(PIN_Din_4   , LOW);
  pinMode(PIN_Din_5   , OUTPUT);  digitalWrite(PIN_Din_5   , LOW);
  pinMode(PIN_Din_6   , OUTPUT);  digitalWrite(PIN_Din_6   , LOW);
  pinMode(PIN_Din_7   , OUTPUT);  digitalWrite(PIN_Din_7   , LOW);

  // Set digital pins to input connecting DUT's OUTPUT
  pinMode(PIN_Dout_0  , INPUT);
  pinMode(PIN_Dout_1  , INPUT);
  pinMode(PIN_Dout_2  , INPUT);
  pinMode(PIN_Dout_3  , INPUT);
  pinMode(PIN_Dout_4  , INPUT);
  pinMode(PIN_Dout_5  , INPUT);
  pinMode(PIN_Dout_6  , INPUT);
  pinMode(PIN_Dout_7  , INPUT);
  pinMode(PIN_RCO     , INPUT);

  // Monitoring LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

#define N_RX  2 // [0]={-|CLK|nCLR_L|nCLR_H|nLOAD_L|nLOAD_H|ENP|ENT}
                // [1]=Din;
#define N_TX  2 // [0]=Dout
                // [1]={-------|RCO}

uint8_t rxBuf[N_RX], txBuf[N_TX];

void RxPacket()
{
  int rxByte;

  while(true)
  {
    if (Serial.available() >= N_RX)
    {
      for(int i=0; i<N_RX; i++)
      {
        rxByte = Serial.read();
        rxBuf[i] = (uint8_t)rxByte;
      }
      //rxBuf[0]={-|CLK|nCLR_L|nCLR_H|nLOAD_L|nLOAD_H|ENP|ENT}
      digitalWrite(PIN_nCLR_L,  rxBuf[0] & 0x20);
      digitalWrite(PIN_nCLR_H,  rxBuf[0] & 0x10);
      digitalWrite(PIN_nLOAD_L, rxBuf[0] & 0x08);
      digitalWrite(PIN_nLOAD_H, rxBuf[0] & 0x04);
      digitalWrite(PIN_ENP,     rxBuf[0] & 0x02);
      digitalWrite(PIN_ENT,     rxBuf[0] & 0x01);
      // rxBuf[1] = Din
      digitalWrite(PIN_Din_0,   rxBuf[1] & 0x01);
      digitalWrite(PIN_Din_1,   rxBuf[1] & 0x02);
      digitalWrite(PIN_Din_2,   rxBuf[1] & 0x04);
      digitalWrite(PIN_Din_3,   rxBuf[1] & 0x08);
      digitalWrite(PIN_Din_4,   rxBuf[1] & 0x10);
      digitalWrite(PIN_Din_5,   rxBuf[1] & 0x20);
      digitalWrite(PIN_Din_6,   rxBuf[1] & 0x40);
      digitalWrite(PIN_Din_7,   rxBuf[1] & 0x80);
      // Clocking to DUT
      if (rxBuf[0] & 0x40) 
        digitalWrite(PIN_CLK, HIGH);
      else
        digitalWrite(PIN_CLK, LOW);
      return;
    }
  }
}

void TxPacket()
{
  int txByte;

  while(1)
  {
    if (Serial.availableForWrite() >= N_TX)
    {
      txBuf[0] = ((digitalRead(PIN_Dout_0)? 0x01:0x00) |
                  (digitalRead(PIN_Dout_1)? 0x02:0x00) |
                  (digitalRead(PIN_Dout_2)? 0x04:0x00) |
                  (digitalRead(PIN_Dout_3)? 0x08:0x00) |
                  (digitalRead(PIN_Dout_4)? 0x10:0x00) |
                  (digitalRead(PIN_Dout_5)? 0x20:0x00) |
                  (digitalRead(PIN_Dout_6)? 0x40:0x00) |
                  (digitalRead(PIN_Dout_7)? 0x80:0x00));  // Dout
      txBuf[1] = ((digitalRead(PIN_RCO)? 0x01:0x00)); // RCO

      for(int i=0; i<N_TX; i++)
      {
        txByte = (int)txBuf[i];
        Serial.write(txByte);
      }

      return;
    }
  }
}

uint8_t counter;

void loop()
{
  counter += 1;
  digitalWrite(LED_BUILTIN, (counter & 0x10)? HIGH:LOW);

  RxPacket();
  TxPacket();
}
