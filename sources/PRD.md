Product Requirements Document
 GBM2804H-100T Gimbal Motor Control
Overview
Develop firmware for the NUCLEO-G431RB with the X-NUCLEO-IHM16M1 motor-driver board to operate the GBM2804H-100T
three-phase gimbal motor using sensorless Field-Oriented Control (FOC).
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
Motor Connection
U, V, W three-phase output
Current Sensing
Three-shunt current sensing
Functional Requirements
1. Implement sensorless FOC for three-phase motor control.
2. Motor shall remain stopped after system initialization.
3. Support START and STOP commands through UART.
4. Support configurable speed command from 300 to 1000 RPM.
5. Support clockwise and counter-clockwise rotation.
6. Use hardware PWM generation for the three-phase inverter.
7. Measure motor phase current using the IHM16M1 current-sensing circuitry.
8. Limit commanded motor current to <= 1.5 A RMS or a lower validated project limit.
9. Detect motor-control faults and immediately disable PWM output.
UART Interface
UART: 115200 baud, 8-N-1. Required commands:
MOTOR START
Start motor
MOTOR STOP
Stop motor
SPEED <RPM>
Set target speed
DIR CW / DIR CCW
Set direction
STATUS
Report state, target/estimated speed, direction and fault status
Validation Criteria
- Reliable motor startup from standstill
- Stable rotation at commanded speed
- Speed change while running
- CW and CCW operation
- Controlled motor stop
- Immediate PWM shutdown on motor-driver fault
- No operation outside configured voltage/current limits
Implementation Note: The G431RB + IHM16M1 + GBM2804H-100T combination is intended for three-phase BLDC/PMSM motor-control
evaluation. Final speed, startup and current limits shall be tuned and validated on the actual motor and supply before extended operation.