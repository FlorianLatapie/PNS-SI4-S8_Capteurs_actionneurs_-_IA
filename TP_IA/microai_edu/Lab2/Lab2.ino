extern "C" {
#include "model.h"
}

#define MAX_MSG_SIZE 32768

void setup() {

  // Initialize serial port
  Serial.begin(115200);

  // Initialize pin for blinking LED
  pinMode(PIN_LED, OUTPUT);

  // Wait for initialization
  while (!Serial && millis() < 5000);

  // Notify readyness
  Serial.println("READY");
}

void loop() {
  static unsigned int inference_count = 0;
  static char buf[MAX_MSG_SIZE];
  static float finputs[MODEL_INPUT_SAMPLES*MODEL_INPUT_CHANNELS];
  static number_t inputs[MODEL_INPUT_CHANNELS][MODEL_INPUT_SAMPLES];
  static number_t outputs[MODEL_OUTPUT_SAMPLES];

  // Read message sent by host
  int msg_len = Serial.readBytesUntil('\n', buf, MAX_MSG_SIZE);
  if (msg_len < 1) {
    // Nothing read, send READY again to make sure we got acknowledged and try again at next iteration
    Serial.println("READY");
    return;
  } else if (msg_len != MAX_MSG_SIZE) {
    // Terminator character is discarded from buffer unless number of bytes read equals max length
    // Artificially increment the message length to account for the missing terminator.
    msg_len++;
  }
  Serial.println(msg_len);

  // Convert received string to floats
  char *pbuf = buf;
  int i = 0;
  while ((pbuf - buf) < msg_len && *pbuf != '\r' && *pbuf != '\n') {
    finputs[i] = strtof(pbuf, &pbuf);
    i++;
    pbuf++; // skip delimiter
  }

  //TODO: Convert inputs from floating-point to fixed-point
    /*
     * With such a fixed representation the real value 2-9 becomes the integer 1, 2-8 the integer 2, 2-7 the
integer 4...
So, for each input, your conversion code has to multiply the real data by 29. The floating point result of
this multiplication has then to be converted in integer in the type long_number_t. Then this long integer
value is clamped in our working representation (16 bits) with the clamp_to_number_t() function already
used previously.
     * Warning: the data is received in channels_last format (TensorFlow/Keras convention), while the C
inference code expects the data in channels_first format (PyTorch convention). Therefore, the finputs
array has dimensions [MODEL_INPUT_SAMPLES][MODEL_INPUT_CHANNELS] and the inputs array has
dimensions [MODEL_INPUT_CHANNELS][MODEL_INPUT_SAMPLES]. In your code you must make sure
you convert the value in the correct order.
     */

    for (int i = 0; i < MODEL_INPUT_SAMPLES * MODEL_INPUT_CHANNELS; i++) {
        int col = i / MODEL_INPUT_CHANNELS;
        int row = j % MODEL_INPUT_CHANNELS;
        inputs[col][rol] = clamp_to_number_t(finputs[i] * (1 << FIXED_POINT));
    }

  digitalWrite(PIN_LED, HIGH);
  // Run inference
  cnn(inputs, outputs);
  digitalWrite(PIN_LED, LOW); 

  // Get output class
  unsigned int label = 0;
  float max_val = outputs[0];
  for (unsigned int i = 1; i < MODEL_OUTPUT_SAMPLES; i++) {
    if (max_val < outputs[i]) {
      max_val = outputs[i];
      label = i;
    }
  }

  inference_count++;

  char msg[64];
  snprintf(msg, sizeof(msg), "%d,%d,%f", inference_count, label, (double)max_val); // force double cast to workaround -Werror=double-promotion since printf uses variadic arguments so promotes to double automatically
  Serial.println(msg);

}
