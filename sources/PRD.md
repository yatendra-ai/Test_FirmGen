Product Requirements Document
 GBM2804H-100T Automatic Basic Motor Rotation
Overview
Develop firmware for the NUCLEO-G431RB with the X-NUCLEO-IHM16M1 to automatically demonstrate basic
rotation of the GBM2804H-100T three-phase gimbal motor after power-up. The project is intended only to verify the
MCU, motor-driver board, PWM outputs and motor connections. No UART command is required to start or stop the
motor.
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
4. Keep PWM disabled and the driver in a safe state during the initial boot delay.
5. Automatically start the motor sequence after the configured boot delay; no UART command is required.
6. Ramp PWM amplitude and electrical frequency gradually to avoid an abrupt startup.
7. Run the motor for the configured demonstration duration.
8. Ramp down the electrical frequency, disable PWM, and return the driver to standby automatically.
9. An active STSPIN830 EN/FAULT condition shall immediately disable PWM at any time.
10. After the automatic test completes, the motor shall remain stopped until the board is reset or power-cycled.
Automatic Test Sequence
 Time from boot
Action
0-3 s
Initialization. Keep PWM disabled and motor driver in standby.
3-8 s
Enable driver and ramp electrical frequency from 1 Hz to 10 Hz. Ramp PWM amplitude from 10% to 20%.
8-18 s
Hold at 10 Hz electrical frequency and maximum 20% PWM amplitude.
18-20 s
Ramp electrical frequency and PWM amplitude down to zero.
After 20 s
Disable PWM, place driver in standby, and remain stopped until reset/power cycle.
Initial Operating Parameters
 Parameter
Value
PWM switching frequency
20 kHz
Boot delay
3 s


Startup ramp duration
5 s
Initial PWM amplitude
10%
Maximum PWM amplitude
20%
Electrical frequency
1 Hz ramping to 10 Hz
Hold duration
10 s
Shutdown ramp
2 s
Motor pole pairs
7
The electrical frequency creates the rotating field only. Actual motor RPM is not measured or regulated in this test.
Validation Criteria
  Firmware builds and flashes successfully.
  PWM remains disabled for the first 3 seconds after boot.
  The motor sequence starts automatically without any UART input.
  Visible motor movement or rotation occurs during the automatic run window.
  PWM is disabled automatically after the run completes.
  Driver fault immediately disables PWM.
  The motor remains stopped after the sequence until reset or power-cycle.
 
Explicitly Out of Scope
UART-based motor start/stop, Sensorless FOC, Motor resistance/inductance/flux characterization, Rotor observer,
Clarke/Park transforms, Current PI or speed PI controller, Closed-loop RPM control, Phase-current ADC control, VBUS
ADC control, ADC-triggered DMA, Current-offset calibration, Oscilloscope integration, Automatic motor-parameter
identification.
Implementation constraint: Do not block the automatic motor sequence because ADC measurements, motor
electrical parameters, characterization, FOC, observer, current sensing, UART commands, or closed-loop
control are unavailable. The automatic sequence shall depend only on successful PWM/GPIO initialization,
completion of the boot delay, and absence of an active STSPIN830 hardware fault.