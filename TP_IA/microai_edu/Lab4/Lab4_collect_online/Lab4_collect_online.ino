extern "C" {
#include "model.h"
}

#include <ICM_20948.h>                      // https://github.com/sparkfun/SparkFun_ICM-20948_ArduinoLibrary

#define SAMPLE_RATE 20

// ICM
ICM_20948_I2C icm;

static long long timer = 0;

#define DURATION 5 * 60
#define ACCEL_SAMPLES DURATION * SAMPLE_RATE
#define SIZE 32

#define MAX_MSG_SIZE 32768


static float accel_buf[ACCEL_SAMPLES][3];
static size_t sample_i = 0;
static size_t sample_index_tab = 0;
static float faccel_buf_win [MODEL_INPUT_SAMPLES*MODEL_INPUT_CHANNELS];
static number_t accel_buf_win[MODEL_INPUT_CHANNELS][MODEL_INPUT_SAMPLES];

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
    icm.setSampleRate((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), {56, 55}); // a = 56 -> 20.09Hz, g = 55 -> 20Hz


    // Notify readyness
    Serial.println("READY");

    timer = millis();
}

void loop() {
    static unsigned int inference_count = 0;
    static number_t outputs[MODEL_OUTPUT_SAMPLES];

    // Try to respect sampling rate
    if (millis() > timer + (1000 / SAMPLE_RATE)) {
        timer = millis();

        if (icm.dataReady()) {
            // Read accelerometer data
            icm.getAGMT(); // The values are only updated when you call 'getAGMT'
        }

        // Blink LED for activity indicator
        //digitalWrite(LS_LED_BLUE, 1 - digitalRead(LS_LED_BLUE));

        printf("0,%f,%f,%f\r\n", icm.accX() / 1000.0f, icm.accY() / 1000.0f, icm.accZ() / 1000.0f);

        // Read accelerometer data
        accel_buf[sample_i][0] = icm.accX() / 1000.0f;
        accel_buf[sample_i][1] = icm.accY() / 1000.0f;
        accel_buf[sample_i][2] = icm.accZ() / 1000.0f;

        sample_i++;

        digitalWrite(LS_LED_BLUE, LOW);


        if(sample_i % SIZE == 0){

            for( int k = 0; k < MODEL_INPUT_SAMPLES * 3; k += 3  ){
                faccel_buf_win[k] = accel_buf[sample_index_tab][0];
                faccel_buf_win[k + 1] = accel_buf[sample_index_tab][1];
                faccel_buf_win[k + 2] = accel_buf[sample_index_tab][2];
                sample_index_tab++;
            }


            //TODO: Convert inputs from floating-point to fixed-point
            for(int i = 0; i < MODEL_INPUT_CHANNELS; i++){
                for(int j = 0; j < MODEL_INPUT_SAMPLES; j++){
                    accel_buf_win[i][j] = clamp_to_number_t(faccel_buf_win[j * MODEL_INPUT_CHANNELS + i] * (1 << FIXED_POINT));
                }
            }


            //digitalWrite(PIN_LED, HIGH);
            // Run inference
            cnn(accel_buf_win, outputs);
            //digitalWrite(PIN_LED, LOW);

            // Get output class
            unsigned int label = 0;
            float max_val = outputs[0];
            for (unsigned int i = 1; i < MODEL_OUTPUT_SAMPLES; i++) {
                if (max_val < outputs[i]) {
                    max_val = outputs[i];
                    label = i;
                }
            }


            digitalWrite(LS_LED_BLUE, HIGH);

/*
          if(label == 1){
            pinMode(LS_LED_BLUE, OUTPUT);
          }else{
            digitalWrite(LS_LED_BLUE, LOW);
          }*/

            inference_count++;

            printf("2,%d,%d\r\n", label, inference_count);
        }

    }
}