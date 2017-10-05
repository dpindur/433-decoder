#define SYNC_LENGTH   15000 // in microseconds (default was 11000) (now 15000 - 15500)
#define BIT_LENGTH    300   // in microseconds (default was 375)
#define SIZE          256
#define RF_PIN        2
#define INTERRUPT_PIN 0

unsigned long timings[SIZE];
static unsigned long lastTime = 0;
unsigned int syncIndex = 0;

bool printing = false;
bool synced = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("Started.");
  delay(500);

  lastTime = micros();
  pinMode(RF_PIN, INPUT);
  attachInterrupt(INTERRUPT_PIN, handler, CHANGE);
}

void loop()
{
  if(printing == true)
  {
    detachInterrupt(INTERRUPT_PIN);

    Serial.println("Print-Start");

    unsigned int i = syncIndex;
    unsigned char lastBit = 0;
    unsigned int count = 0;
    while(i != ((syncIndex + (SIZE-1)) % SIZE))
    {
      if(timings[i] > SYNC_LENGTH)
      {
        Serial.println();
        Serial.print("SYNC: ");
        Serial.println(timings[i]);
        lastBit = 1;
      }
      else
      {
        count = timings[i] / BIT_LENGTH;
        for(int j = 0; j < count; j++)
        {
          Serial.print(lastBit);
        }
//        Serial.print(": ");
//        Serial.println(timings[i]);
        lastBit = lastBit == 0 ? 1 : 0;
      }
      i = (i + 1) % SIZE;
    }

    Serial.println("Print-End");
    delay(1000);

    printing = false;
    lastTime = micros();
    attachInterrupt(INTERRUPT_PIN, handler, CHANGE);
  }
}

void handler()
{
  static unsigned long duration = 0;
  static unsigned int ringIndex = 0;

  if (printing == true)
  {
    return;
  }

  // Get timing information
  long time = micros();
  duration = time - lastTime;
  lastTime = time;

  // Put time into buffer
  timings[ringIndex] = duration;

  if(!synced && duration > SYNC_LENGTH && duration < SYNC_LENGTH + 500)
  {
    syncIndex = ringIndex;
    synced = true;
  }

  if(synced && ringIndex == (syncIndex + (SIZE-1)) % SIZE)
  {
    synced = false;
    printing = true;
  }

  ringIndex = (ringIndex + 1) % SIZE;
}















