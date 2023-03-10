//#include <LibLacuna.h>
#include <I2S.h>

#include "full_model.h"

#define I2S_SAMPLE_RATE 16000  // [16000, 48000] supported by the microphone
#define I2S_BITS_PER_SAMPLE 32 // Data is sent in 32-bit packets over I2S but only 18 bits are used by the microphone, remaining least significant bits are set to 0

static number_t inputs[MODEL_INPUT_CHANNELS][MODEL_INPUT_SAMPLES]; // 1-channel, 16000 samples for 16kHz over 1s
static volatile size_t sample_i = 0;
static number_t outputs[MODEL_OUTPUT_SAMPLES];
static volatile boolean ready_for_inference = false;

void onI2SReceive() {
  //TODO: fill DMA interrupt callback function
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  pinMode(LS_GPS_ENABLE, OUTPUT);
  digitalWrite(LS_GPS_ENABLE, LOW);
  pinMode(LS_GPS_V_BCKP, OUTPUT);
  digitalWrite(LS_GPS_V_BCKP, LOW);
  pinMode(SD_ON_OFF, OUTPUT);
  digitalWrite(SD_ON_OFF, HIGH);
  
  // start I2S
  if (!I2S.begin(I2S_PHILIPS_MODE, I2S_SAMPLE_RATE, I2S_BITS_PER_SAMPLE, false)) {
    Serial.println("Failed to initialize I2S!");
    while (1); // do nothing
  }

  I2S.onReceive(onI2SReceive);

  // Trigger a read to start DMA
  I2S.peek();
  
  while (!Serial && millis() < 5000);
}

void loop() {
  if (ready_for_inference) {
    // Input buffer full, perform inference

    // Turn LED on during preprocessing/prediction
    digitalWrite(PIN_LED, HIGH);

    // Start timer
    long long t_start = millis();


    // Compute DC offset
    int32_t dc_offset = 0;

    for (size_t i = 0; i < sample_i; i++) { // Accumulate samples
      dc_offset += inputs[0][i];
      
    }

    dc_offset = dc_offset / (int32_t)sample_i; // Compute average over samples
    
    // Filtering
    for (size_t i = 0; i < sample_i; i++) { 
      // Remove DC offset
      inputs[0][i] -= dc_offset;
      
      // Amplify
      inputs[0][i] = inputs[0][i] * 4;
    }

    // Send signed 16-bit PCM little endian 1 channel
    //Serial.write((uint8_t*)inputs[0], MODEL_INPUT_SAMPLES*2);


    // Normalize
    /*max_sample = inputs[0][0];
    for (size_t i = 1; i < MODEL_INPUT_SAMPLES; i++) {
      if (max_sample < inputs[0][i])
        max_sample = inputs[0][i];
    }
    for (size_t i = 0; i < MODEL_INPUT_SAMPLES; i++) {
      inputs[0][i] = ((int32_t)inputs[0][i] << FIXED_POINT) / max_sample;
    }*/

    // Predict
    cnn(inputs, outputs);

    // Get output class
    unsigned int label = 0;
    number_t max_val = outputs[0];
    for (unsigned int i = 1; i < MODEL_OUTPUT_SAMPLES; i++) {
      if (max_val < outputs[i]) {
        max_val = outputs[i];
        label = i;
      }
    }

    static char msg[128];
    snprintf(msg, sizeof(msg), "%d,%d, %d", label, max_val, (int)(millis() - t_start));
    Serial.println(msg);

    // Turn LED off after prediction has been sent
    digitalWrite(PIN_LED, LOW);
    
    ready_for_inference = false;
    sample_i = 0;

  }
}
