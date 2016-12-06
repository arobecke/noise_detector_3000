#define LOG_OUT 1 // use the log output function
#define LIN_OUT8 1 // use the linear byte output function


const int detectionWindowSize = 4;
#define AmpMax (1024 / 2)
#define MicSamples (1024*2)
#define VolumeGainFactorBits 0

const double loudThreshold = 1 / detectionWindowSize;

unsigned int sample;

const int mic1 = 0;
const int mic2 = 1;
const int mic3 = 2;
const int mic4 = 3;


const int mic1Switch = 2;
const int mic2Switch = 3;
const int mic3Switch = 4;
const int mic4Switch = 5;

const int led1 = 7;
const int led2 = 8;
const int led3 = 9;
const int led4 = 10;



const int detectionWindowSizeInMillis = 30 * 1000;

void setup()
{

   pinMode(led1, OUTPUT);
   pinMode(led2, OUTPUT);
   pinMode(led3, OUTPUT);
   pinMode(led4, OUTPUT);

   pinMode(mic1Switch, INPUT);
   pinMode(mic2Switch, INPUT);
   pinMode(mic3Switch, INPUT);
   pinMode(mic4Switch, INPUT);


   ADMUX |= 0x40; // Use Vcc for analog reference.
   DIDR0 = 0x01; // turn off the digital input for adc0
   analogReference(EXTERNAL); // 3.3V to AREF


     // serial
     Serial.begin(9600);
     while (!Serial); // Wait untilSerial is ready - Leonardo
     Serial.println("Starting noise detector 3000...");
}


void loop()
{
  double mic1Samples [detectionWindowSize];
  double mic2Samples [detectionWindowSize];
  double mic3Samples [detectionWindowSize];
  double mic4Samples [detectionWindowSize];

  int i;

  boolean Mic1SwitchedOn = (digitalRead(mic1Switch) == HIGH) ? true : false;
  boolean Mic2SwitchedOn = (digitalRead(mic2Switch) == HIGH) ? true : false;
  boolean Mic3SwitchedOn = (digitalRead(mic3Switch) == HIGH) ? true : false;
  boolean Mic4SwitchedOn = (digitalRead(mic4Switch) == HIGH) ? true : false;

  for(i = 0; i < detectionWindowSize; i++) {
     unsigned int signalMaxMic1 = 0;
     unsigned int signalMinMic1 = 1024;

     unsigned int signalMaxMic2 = 0;
     unsigned int signalMinMic2 = 1024;

     unsigned int signalMaxMic3 = 0;
     unsigned int signalMinMic3 = 1024;

     unsigned int signalMaxMic4 = 0;
     unsigned int signalMinMic4 = 1024;



     long soundVolAvg1 = 0, soundVolMax1 = 0, soundVolRMS1 = 0;
     long soundVolAvg2 = 0, soundVolMax2 = 0, soundVolRMS2 = 0;
     long soundVolAvg3 = 0, soundVolMax3 = 0, soundVolRMS3 = 0;
     long soundVolAvg4 = 0, soundVolMax4 = 0, soundVolRMS4 = 0;

      for (int x = 0; x < MicSamples; x++) {

        int signalMic1 = analogRead(mic1);
        int signalMic2 = analogRead(mic2);
        int signalMic3 = analogRead(mic3);
        int signalMic4 = analogRead(mic4);


        int signalMic1Amplitude = abs(signalMic1 - AmpMax);
        int signalMic2Amplitude = abs(signalMic2 - AmpMax);
        int signalMic3Amplitude = abs(signalMic3 - AmpMax);
        int signalMic4Amplitude = abs(signalMic4 - AmpMax);


         soundVolMax1 = max(soundVolMax1, signalMic1Amplitude);
         soundVolMax2 = max(soundVolMax2, signalMic2Amplitude);
         soundVolMax3 = max(soundVolMax3, signalMic3Amplitude);
         soundVolMax4 = max(soundVolMax4, signalMic4Amplitude);

         soundVolAvg1 += signalMic1Amplitude;
         soundVolAvg2 += signalMic2Amplitude;
         soundVolAvg3 += signalMic3Amplitude;
         soundVolAvg4 += signalMic4Amplitude;

         soundVolRMS1 += ((long)signalMic1Amplitude*signalMic1Amplitude);
         soundVolRMS2 += ((long)signalMic2Amplitude*signalMic2Amplitude);
         soundVolRMS3 += ((long)signalMic3Amplitude*signalMic3Amplitude);
         soundVolRMS4 += ((long)signalMic4Amplitude*signalMic4Amplitude);

      }

      soundVolAvg1 /= MicSamples;
      soundVolAvg2 /= MicSamples;
      soundVolAvg3 /= MicSamples;
      soundVolAvg4 /= MicSamples;

      soundVolRMS1 /= MicSamples;
      soundVolRMS2 /= MicSamples;
      soundVolRMS3 /= MicSamples;
      soundVolRMS4 /= MicSamples;

      float soundVolRMS1flt = sqrt(soundVolRMS1);
      float soundVolRMS2flt = sqrt(soundVolRMS2);
      float soundVolRMS3flt = sqrt(soundVolRMS3);
      float soundVolRMS4flt = sqrt(soundVolRMS4);

      soundVolAvg1 = 100 * soundVolAvg1 / AmpMax;
      soundVolAvg2 = 100 * soundVolAvg2 / AmpMax;
      soundVolAvg3 = 100 * soundVolAvg3 / AmpMax;
      soundVolAvg4 = 100 * soundVolAvg4 / AmpMax;

      soundVolMax1 = 100 * soundVolMax1 / AmpMax;
      soundVolMax2 = 100 * soundVolMax2 / AmpMax;
      soundVolMax3 = 100 * soundVolMax3 / AmpMax;
      soundVolMax4 = 100 * soundVolMax4 / AmpMax;

      soundVolRMS1flt = 100 * soundVolRMS1flt / AmpMax;
      soundVolRMS2flt = 100 * soundVolRMS2flt / AmpMax;
      soundVolRMS3flt = 100 * soundVolRMS3flt / AmpMax;
      soundVolRMS4flt = 100 * soundVolRMS4flt / AmpMax;

      soundVolRMS1 = 10 * soundVolRMS1flt / 7; // RMS to estimate peak (RMS is 0.7 of the peak in sin)
      soundVolRMS2 = 10 * soundVolRMS2flt / 7; // RMS to estimate peak (RMS is 0.7 of the peak in sin)
      soundVolRMS3 = 10 * soundVolRMS3flt / 7; // RMS to estimate peak (RMS is 0.7 of the peak in sin)
      soundVolRMS4 = 10 * soundVolRMS4flt / 7; // RMS to estimate peak (RMS is 0.7 of the peak in sin)



//       mic1Samples[i] = Mic1SwitchedOn ? soundVolMax1 : 0;
//       mic2Samples[i] = Mic2SwitchedOn ? soundVolMax2 : 0;
//       mic3Samples[i] = Mic3SwitchedOn ? soundVolMax3 : 0;
//       mic4Samples[i] = Mic4SwitchedOn ? soundVolMax4 : 0;

       mic1Samples[i] = Mic1SwitchedOn ? soundVolRMS1 : 0;
       mic2Samples[i] = Mic2SwitchedOn ? soundVolRMS2 : 0;
       mic3Samples[i] = Mic3SwitchedOn ? soundVolRMS3 : 0;
       mic4Samples[i] = Mic4SwitchedOn ? soundVolRMS4 : 0;
  }

  int j;

  double mic1SamplesSum = 0;
  double mic2SamplesSum = 0;
  double mic3SamplesSum = 0;
  double mic4SamplesSum = 0;

  for(j = 0; j < detectionWindowSize; j++) {
    mic1SamplesSum = mic1SamplesSum + mic1Samples[j];
    mic2SamplesSum = mic2SamplesSum + mic2Samples[j];
    mic3SamplesSum = mic3SamplesSum + mic3Samples[j];
    mic4SamplesSum = mic4SamplesSum + mic4Samples[j];
  }

  mic1SamplesSum /= detectionWindowSize;
  mic2SamplesSum /= detectionWindowSize;
  mic3SamplesSum /= detectionWindowSize;
  mic4SamplesSum /= detectionWindowSize;

  // Calculate who was the loudest of all
  double loudestMicSampleSum = mic1SamplesSum;
  int loudestMicLed = led1;

  if(loudestMicSampleSum < mic2SamplesSum) {
    loudestMicLed = led2;
    loudestMicSampleSum = mic2SamplesSum;
  }

  if (loudestMicSampleSum < mic3SamplesSum) {
    loudestMicLed = led3;
    loudestMicSampleSum = mic3SamplesSum;
  }

  if (loudestMicSampleSum < mic4SamplesSum) {
    loudestMicLed = led4;
    loudestMicSampleSum = mic4SamplesSum;
  }

  Serial.println("Emanuel:");
  Serial.println(mic1SamplesSum);

  Serial.println("Martin:");
  Serial.println(mic2SamplesSum);

  Serial.println("Andreas:");
  Serial.println(mic3SamplesSum);

  Serial.println("Dominik:");
  Serial.println(mic4SamplesSum);

  Serial.println("loudestMicSampleSum:");
  Serial.println(loudestMicSampleSum);

  int loudest = loudestMicLed - 6;

  if(loudest == 1) {
    Serial.println("Emanuel is the loudest!");
  } else if (loudest == 2) {
    Serial.println("Martin is the loudest!");
  } else if (loudest == 3) {
    Serial.println("Andy is the loudest");
  } else if (loudest == 4) {
    Serial.println("Domi is the loudest, again!");
  }

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);

  if(loudestMicSampleSum > 1) {
    digitalWrite(loudestMicLed, HIGH);

    if(loudest == 1) {
        Serial.println("Emanuel was too loud!");
      } else if (loudest == 2) {
        Serial.println("Martin was too loud!");
      } else if (loudest == 3) {
        Serial.println("Andy was too loud!");
      } else if (loudest == 4) {
        Serial.println("Domi was too loud, again!");
      }
  }

}






































