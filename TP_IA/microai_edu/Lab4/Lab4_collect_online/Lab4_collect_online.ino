extern "C" {
#include "model.h"
}

#include <ICM_20948.h>                      // https://github.com/sparkfun/SparkFun_ICM-20948_ArduinoLibrary

#define SAMPLE_RATE 20

// ICM
ICM_20948_I2C icm;

static long long timer = 0;

void setup() {
    // 10-14-21: Mandatory on new board revision otherwise I2C does not work
    pinMode(SD_ON_OFF, OUTPUT);
    digitalWrite(SD_ON_OFF, HIGH);

    // Initialize pin for blinking LED
    pinMode(LS_LED_BLUE, OUTPUT);

    // Initialize serial port
    Serial.begin(115200);

    // Wait for initialization
    while (!Serial && millis() < 5000);

    // Initialize I2C used by IMU
    Wire.begin();

    // Initialize IMU
    icm.begin(Wire, 0);

    // Set sample rate to ~20Hz
    icm.setSampleRate((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr),
                      {56, 55}); // a = 56 -> 20.09Hz, g = 55 -> 20Hz

    // Notify readyness
    Serial.println("READY");

    timer = millis();
}

void loop() {
    // Try to respect sampling rate
    if (millis() > timer + (1000 / SAMPLE_RATE)) {
        static unsigned int inference_count = 0;

        timer = millis();

        if (icm.dataReady()) {
            // Read accelerometer data
            icm.getAGMT(); // The values are only updated when you call 'getAGMT'
        }

        for (int i = 0; i < MODEL_INPUT_CHANNELS; i++)
        {
            for (int j = 0; j < MODEL_INPUT_SAMPLES; j++)
            {
                float temp = finputs[j * MODEL_INPUT_CHANNELS + i];
                long_number_t long_temp = temp * (1 << FIXED_POINT);
                inputs[i][j] = clamp_to_number_t(long_temp);
            }
        }

        // Blink LED for activity indicator
        digitalWrite(LS_LED_BLUE, 1 - digitalRead(LS_LED_BLUE));

        // Run inference
        cnn(inputs, outputs);

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

        // Format message with accelerometer data
        char msg[128];
        //snprintf(msg, sizeof(msg), "0,%f,%f,%f\r\n", icm.accX() / 1000.0f, icm.accY() / 1000.0f, icm.accZ() / 1000.0f);
        snprintf(msg, sizeof(msg), "%d,%d,%f", inference_count, label, (double)max_val); // force double cast to workaround -Werror=double-promotion since printf uses variadic arguments so promotes to double automatically

        // Send message over serial port
        Serial.print(msg);
    }
}
