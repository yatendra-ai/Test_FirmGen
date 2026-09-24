Product Requirements Document
 GBM2804H-100T Basic Motor Rotation
Overview
Develop firmware for the NUCLEO-G431RB with the X-NUCLEO-IHM16M1 to demonstrate basic rotation of the
GBM2804H-100T three-phase gimbal motor. The purpose of this project is only to verify the MCU, motor-driver
board, PWM outputs and motor connections. Closed-loop FOC and motor characterization are intentionally excluded.
Target Hardware
Item
Specification
MCU Board
NUCLEO-G431RB
Motor Driver
X-NUCLEO-IHM16M1 / STSPIN830
Motor
GBM2804H-100T, 7 pole pairs
Supply
12 V DC
Motor Outputs
U, V, W three-phase output
Functional Requirements
1. Configure TIM1 to generate three-phase PWM.
2. Use the factory-default X-NUCLEO-IHM16M1 three-input configuration.
3. Generate a rotating three-phase electrical field using open-loop sinusoidal PWM.
4. Start with low PWM amplitude and gradually ramp the electrical frequency.
5. Motor shall remain stopped after system initialization.
6. Support clockwise and counter-clockwise rotation.
7. MOTOR START shall begin the open-loop startup ramp.
8. MOTOR STOP shall immediately disable PWM.
9. An active STSPIN830 EN/FAULT condition shall immediately disable PWM.
10. STBY shall be controlled correctly to enable and disable the motor driver.
Initial Operating Parameters
Parameter
Initial Value
PWM switching frequency
20 kHz
Initial PWM amplitude
10%
Maximum PWM amplitude for this test
20%
Initial electrical frequency
1 Hz
Electrical-frequency ramp target
10 Hz
Motor pole pairs
7
No closed-loop RPM regulation is required. Electrical frequency is only used to create a rotating field; actual motor
RPM is not measured or guaranteed.
UART Interface
Command
Action


MOTOR START
Start the open-loop rotation ramp
MOTOR STOP
Disable PWM and stop the motor
DIR CW / DIR CCW
Set rotation direction
FREQ <Hz>
Set electrical frequency
DUTY <%>
Set PWM amplitude within the configured safe limit
STATUS
Report motor state, direction, electrical frequency, PWM duty and driver fault status
Validation Criteria
 Firmware builds and flashes successfully.
 
 PWM remains disabled after boot.
 
 MOTOR START produces visible motor movement or rotation.
 
 MOTOR STOP disables PWM and stops motor drive.
 
 CW and CCW commands reverse the generated phase sequence.
 
 Driver fault disables PWM.
 
Explicitly Out of Scope
Sensorless FOC, Motor resistance, inductance or flux characterization, Rotor observer, Clarke/Park and inverse
transforms, Current PI or speed PI controller, Closed-loop RPM control, Phase-current ADC control, VBUS ADC
control, ADC-triggered DMA, Current-offset calibration, Oscilloscope integration, Automatic motor-parameter
identification.
Implementation constraint: Do not block MOTOR START because ADC measurements, motor electrical
parameters, characterization, FOC, observer, current sensing, or closed-loop control are unavailable. These are
explicitly outside the scope of this project. MOTOR START shall depend only on successful PWM/GPIO
initialization and the absence of an active STSPIN830 hardware fault.