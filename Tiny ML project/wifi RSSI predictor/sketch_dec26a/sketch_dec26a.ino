#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include <WiFi.h>
#include "wifi_rssi_model.h"
#include "driver/periph_ctrl.h" // Include this header file

const char* ssid = "Nish_DRouter";
const char* password = "Nishan622";

const int SEQUENCE_LENGTH = 10;
const float NORMALIZATION_CONSTANT = 90.0;
const float SCALE_CONSTANT = 40.0;
float rssi_sequence[SEQUENCE_LENGTH];
int sequence_index = 0;

namespace {
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* input = nullptr;
  TfLiteTensor* output = nullptr;
  constexpr int kTensorArenaSize = 4 * 1024;
  uint8_t tensor_arena[kTensorArenaSize];
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  model = tflite::GetModel(wifi_rssi_model);
  static tflite::AllOpsResolver resolver;
  
  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, kTensorArenaSize, nullptr);
  interpreter = &static_interpreter;

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    Serial.println("AllocateTensors() failed");
    return;
  }

  input = interpreter->input(0);
  output = interpreter->output(0);

  // Initialize sequence with current RSSI
  float initial_rssi = WiFi.RSSI();
  for(int i = 0; i < SEQUENCE_LENGTH; i++) {
    rssi_sequence[i] = initial_rssi;
  }
}

void loop() {
  if(WiFi.status() == WL_CONNECTED) {
    float current_rssi = WiFi.RSSI();
    rssi_sequence[sequence_index] = current_rssi;
    sequence_index = (sequence_index + 1) % SEQUENCE_LENGTH;

    if(sequence_index == 0) {
      // Copy sequence to input tensor
      for(int i = 0; i < SEQUENCE_LENGTH; i++) {
        input->data.f[i] = (rssi_sequence[i] + NORMALIZATION_CONSTANT) / SCALE_CONSTANT; // Normalize
      }

      if (interpreter->Invoke() != kTfLiteOk) {
        Serial.println("Invoke() failed");
        return;
      }

      float predicted_rssi = (output->data.f[0] * SCALE_CONSTANT) - NORMALIZATION_CONSTANT; // Denormalize

      Serial.print("Current RSSI: ");
      Serial.print(current_rssi);
      Serial.print(" | Predicted RSSI: ");
      Serial.println(predicted_rssi);
    }
  } else {
    Serial.println("WiFi disconnected");
  }
  delay(1000);
}
