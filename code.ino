// Define pin numbers
#include <Servo.h>
const int PUMP_PIN = 9; // Example pin for controlling the pump
const int SERVO_PIN = 10;
Servo servo;
// Function to run the pump for a specified duration
void runPump(float volume_ml, float pump_speed_ml_per_min) {
  // Calculate the time needed to dispense the volume
  float duration_sec = (volume_ml / pump_speed_ml_per_min) * 60.00;
  // Turn on the pump
  digitalWrite(PUMP_PIN, HIGH);
  // Wait for the calculated duration
  delay(duration_sec * 1000); // Convert seconds to milliseconds
  // Turn off the pump
  digitalWrite(PUMP_PIN, LOW);
}

void dispenseLiquid(int container) {
  switch(container) {
    case 1:
      servo.write(0); // Move the servo to position for container 1
      delay(1000); // Delay for servo movement
      break;
    case 2:
      servo.write(90); // Move the servo to position for container 2
      delay(1000); // Delay for servo movement
      break;
    case 3:
      servo.write(180); // Move the servo to position for container 3
      delay(1000); // Delay for servo movement
      break;
    default:
      break;
  }
}

void setup() {
  // Set up pin mode
  pinMode(PUMP_PIN, OUTPUT);
  servo.attach(SERVO_PIN);
  Serial.begin(9600); // Start serial communication
}

void loop() {
  while (true) {
    Serial.println("Enter container number");
    while (!Serial.available()) {}
    float container_no = Serial.parseFloat();
    if (container_no <= 0) {
      Serial.println("Volume must be a positive number.");
      continue;
    }
    if (container_no < 1 || container_no > 3) {
      Serial.println("Invalid container selection. Please select 1, 2, or 3.");
      // Clear the serial buffer
      while (Serial.available()) {
        Serial.read();
      }
      continue;
    }
    if(container_no == 3) {
      dispenseLiquid(1);
    } else if (container_no == 2) {
      dispenseLiquid(2);
    } else if(container_no ==1) {
      dispenseLiquid(3);
    }
    // Prompt user for volume input
    Serial.println("Enter volume to be obtained (in ml): ");
    delay(5000);
    while (!Serial.available()) {} // Wait for user input
    float volume_ml = Serial.parseFloat(); // Read the input as float
    if (volume_ml <= 0) {
      Serial.println("Volume must be a positive number.");
      continue;
    }
    float pump_speed_ml_per_min = 20.4; // caliberate the value throught the trial and error method or if the speed is spesified on pump just update it.
    runPump(volume_ml, pump_speed_ml_per_min);
    Serial.print("Pump finished dispensing ");
    Serial.print(volume_ml);
    Serial.println(" ml.");
  }
}
