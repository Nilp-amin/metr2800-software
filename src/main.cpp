#include <Arduino.h>
#include <Ultrasonic.h>
#include <AccelStepper.h>

#include "../lib/navigation/navigation.h" 
#include "../lib/motor/motor.h"
#include "../lib/turret/turret.h"

#define START_BUTTON      PIN_PD5

Laser laser(LASER_PIN, 2000L);
IR irSensors(IR1_PIN, IR2_PIN, IR3_PIN, IR4_PIN,
             IR5_PIN, IR6_PIN, IR7_PIN, IR8_PIN, 
             100, 5);

Ultrasonic frontUltrasonic(F_TRIGGER, F_ECHO);
Ultrasonic rearUltrasonic(R_TRIGGER, R_ECHO);

// Have to flip pin 2 and 3 in decleration due to library
// Have to enable this class bfr running and disable when not running
AccelStepper leftStepper(AccelStepper::FULL4WIRE, L_STEPPER_PIN_1, 
                         L_STEPPER_PIN_3, L_STEPPER_PIN_2, L_STEPPER_PIN_4, false);
AccelStepper rightStepper(AccelStepper::FULL4WIRE, R_STEPPER_PIN_1, 
                          R_STEPPER_PIN_3, R_STEPPER_PIN_2, R_STEPPER_PIN_4, false);
// Use half step for speed and low current usage
AccelStepper turretStepper(AccelStepper::HALF4WIRE, TURRET_PIN_1, 
                           TURRET_PIN_3, TURRET_PIN_2, TURRET_PIN_4, false);

void setup() {  
  Serial.begin(9600);
  leftStepper.setMaxSpeed(MAX_SPEED_FULLSTEP);
  leftStepper.setAcceleration(DRIVE_ACCEL);

  rightStepper.setMaxSpeed(MAX_SPEED_FULLSTEP);
  rightStepper.setAcceleration(DRIVE_ACCEL);

  turretStepper.setMaxSpeed(MAX_SPEED_HALFSTEP);
  turretStepper.setAcceleration(200);
  turretStepper.setMaxSpeed(MAX_SPEED_HALFSTEP);
  turretStepper.setCurrentPosition(0);
  pinMode(START_BUTTON, OUTPUT);
}

void loop() {

    if (digitalRead(START_BUTTON)) {
        //laser.shootLaser();
        locate(frontUltrasonic, rearUltrasonic, leftStepper, rightStepper);
        irSensors.targetSearch(leftStepper, rightStepper, turretStepper, laser);
        //irSensors.lateralSearch(turretStepper, laser);
     /*
        laser.constantOn();
       while(1) {
       Serial.println(irSensors.totalSensorAvg(100));
       }
       */
    }
}