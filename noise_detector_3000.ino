const int sampleWindowInMillis = 50; // Sample window width in mS (50 mS = 20Hz)
const int detectionWindowSize = 100;

const double loudThreshold = 2.5 / detectionWindowSize;

unsigned int sample;

const int mic1 = 0;
const int mic2 = 1;
const int mic3 = 2;
const int mic4 = 3;

const int led1 = 7;
const int led2 = 8;
const int led3 = 9;
const int led4 = 10;

const int detectionWindowSizeInMillis = 30 * 1000;

void setup()
{
   Serial.begin(9600);
   pinMode(led1, OUTPUT);
   pinMode(led2, OUTPUT);
   pinMode(led3, OUTPUT);
   pinMode(led4, OUTPUT);
}


void loop()
{
  double mic1Samples [detectionWindowSize];
  double mic2Samples [detectionWindowSize];
  double mic3Samples [detectionWindowSize];
  double mic4Samples [detectionWindowSize];

  int i;

  for(i = 0; i < detectionWindowSize; i++) {
    unsigned long sampleStart = millis();

     unsigned int signalMaxMic1 = 0;
     unsigned int signalMinMic1 = 1024;

     unsigned int signalMaxMic2 = 0;
     unsigned int signalMinMic2 = 1024;

     unsigned int signalMaxMic3 = 0;
     unsigned int signalMinMic3 = 1024;

     unsigned int signalMaxMic4 = 0;
     unsigned int signalMinMic4 = 1024;

     while (millis() - sampleStart < sampleWindowInMillis) {

          double signalMic1 = analogRead(mic1);
          double signalMic2 = analogRead(mic2);
          double signalMic3 = analogRead(mic3);
          double signalMic4 = analogRead(mic4);

          if (signalMic1 < 1024) {
             signalMaxMic1 = (signalMaxMic1 > signalMic1) ? signalMaxMic1 : signalMic1;
             signalMinMic1 = (signalMinMic1 < signalMic1) ? signalMinMic1 : signalMic1;
          }

          if (signalMic2 < 1024) {
            signalMaxMic2 = (signalMaxMic2 > signalMic2) ? signalMaxMic2 : signalMic2;
            signalMinMic2 = (signalMinMic2 < signalMic2) ? signalMinMic2 : signalMic2;
          }

          if (signalMic3 < 1024) {
            signalMaxMic3 = (signalMaxMic3 > signalMic3) ? signalMaxMic3 : signalMic3;
            signalMinMic3 = (signalMinMic3 < signalMic3) ? signalMinMic3 : signalMic3;
          }

          if (signalMic4 < 1024) {
            signalMaxMic4 = (signalMaxMic4 > signalMic4) ? signalMaxMic4 : signalMic4;
            signalMinMic4 = (signalMinMic4 < signalMic4) ? signalMinMic4 : signalMic4;
          }

     }

       double peakToPeakMic1 = signalMaxMic1 - signalMinMic1;
       double peakToPeakMic2 = signalMaxMic2 - signalMinMic2;
       double peakToPeakMic3 = signalMaxMic3 - signalMinMic3;
       double peakToPeakMic4 = signalMaxMic4 - signalMinMic4;

       mic1Samples[i] = (peakToPeakMic1 + 5.0) / 1024;
       mic2Samples[i] = (peakToPeakMic2 + 5.0) / 1024;
       mic3Samples[i] = (peakToPeakMic3 + 5.0) / 1024;
       mic4Samples[i] = (peakToPeakMic4 + 5.0) / 1024;
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

  // Calculate who was the loudest of all
  double loudestMicSampleSum = mic1SamplesSum;
  int loudestMicLed = led1;

  if(loudestMicSampleSum < mic2SamplesSum) {
    loudestMicLed = led2;
    loudestMicSampleSum = mic2SamplesSum;
  } else if (loudestMicSampleSum < mic3SamplesSum) {
    loudestMicLed = led3;
    loudestMicSampleSum = mic3SamplesSum;
  } else if (loudestMicSampleSum < mic4SamplesSum) {
    loudestMicLed = led4;
    loudestMicSampleSum = mic4SamplesSum;
  }

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);

  if(loudestMicSampleSum > (loudThreshold * detectionWindowSize)) {
    digitalWrite(loudestMicLed, HIGH);
  }

}
