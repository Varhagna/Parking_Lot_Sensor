#include <RH_ASK.h>
#include <SPI.h>
#define CHECK_VAL 3

uint8_t echoPin = 7;
uint8_t trigPin = 8;
uint8_t id = 1;

uint8_t distance;
uint8_t avg_dist = 0;
RH_ASK driver;
enum FSM {ACQUIRE, TRANSMIT} state;
uint8_t values[CHECK_VAL];
uint8_t num_vals = 0;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    if(!driver.init()) {
        Serial.println("Failed!");
    }
  state = ACQUIRE;
}

uint8_t* constructMessage() {
    uint8_t* message = new uint8_t[2];
    message[0] = avg_dist;
    message[1] = id;
    return message;
}

void tick() {
    switch(state) {
        case ACQUIRE:
                Serial.print("Acquiring Distance: ");
                digitalWrite(trigPin, HIGH);
                delayMicroseconds(10);
                digitalWrite(trigPin, LOW);
                distance = int(.017 * pulseIn(echoPin, HIGH));
        Serial.println(distance);
        values[num_vals % CHECK_VAL] = distance;
        num_vals = num_vals + 1;
                if(num_vals % CHECK_VAL == 0) {
          state = TRANSMIT;
        }
                break;
    case TRANSMIT:
        uint16_t new_avg = 0;
        for(uint8_t i = 0; i < CHECK_VAL; i++) {
            new_avg += values[i];
        }

        new_avg /= CHECK_VAL;
        Serial.print("Prev Average:");
        Serial.println(avg_dist);
        Serial.print("New Average:");
        Serial.println(new_avg);
        if(abs(avg_dist - new_avg) >= 5 || avg_dist == 0) {
          avg_dist = new_avg;
          Serial.print("Attempting Transmission..");
          driver.send(constructMessage(), 2);
          driver.waitPacketSent();
          Serial.print("Sent from ID: ");
          Serial.println(id);
        }
        state = ACQUIRE;
                break;
        default:
                break;
    }
}

void loop() {
  delay(2000);
  tick();
}
