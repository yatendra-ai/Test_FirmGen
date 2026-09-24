1.
 
Objective
 
Monitor
 
interior
 
cabin
 
climate
 
and
 
vehicle
 
ride
 
dynamics
 
using
 
ESP32-C3,
 
and
 
publish
 
the
 
data
 
over
 
MQTT
 
for
 
fleet
 
or
 
personal
 
dashboard
 
use.
 
2.
 
Hardware
 
●
 
MCU:
 
ESP32-C3
 
dev
 
board
 
●
 
Climate
 
sensor:
 
DHT22
 
(temperature,
 
humidity)
 
●
 
IMU:
 
MPU6050
 
(3-axis
 
accelerometer)
 
●
 
Power:
 
5V
 
via
 
micro-USB
 
(from
 
12V
 
automotive
 
adapter)
 
Pin
 
Mapping
 
Signal
 
ESP32-C3-Mini
 
Pin
 
DHT22
 
VCC
 
3.3V
 
DHT22
 
GND
 
GND
 
DHT22
 
OUT
 
GPIO2
 
MPU6050
 
VCC
 
3.3V
 
MPU6050
 
GND
 
GND
 
MPU6050
 
SCL
 
GPIO8
 
MPU6050
 
SDA
 
GPIO9
 
3.
 
Functional
 
Requirements
 
●
 
Read
 
temperature
 
and
 
humidity
 
from
 
DHT22
 
every
 
2
 
seconds
 
●
 
Read
 
X,
 
Y,
 
Z
 
acceleration
 
from
 
MPU6050
 
and
 
convert
 
to
 
G-force
 
●
 
Map
 
axes:
 
X
 
=
 
lateral
 
(cornering),
 
Y
 
=
 
longitudinal
 
(accel/braking),
 
Z
 
=
 
vertical
 
(bumps)
 
●
 
Connect
 
to
 
WiFi
 
automatically
 
;
 
reconnect
 
if
 
the
 
connection
 
drops
 
○
 
SSID
 
:
 
GopalaSutharsan
 
○
 
Password
 
:
 
123987456
 
●
 
Publish
 
climate
 
and
 
motion
 
data
 
as
 
a
 
single
 
JSON
 
payload
 
to
 
MQTT
 
every
 
2000ms
 
●
 
Maintain
 
configurable
 
alert
 
thresholds
 
for
 
temperature,
 
humidity,
 
and
 
G-force
 

●
 
Publish
 
immediately
 
to
 
the
 
alert
 
topic
 
when
 
any
 
threshold
 
is
 
exceeded,
 
independent
 
of
 
the
 
regular
 
2000ms
 
cycle
 
4.
 
MQTT
 
Periodic
 
topic:
 
vehicle/cabin/telemetry
,
 
every
 
2000ms:
 
{
 
  
"temp_c":
 
24.5,
 
  
"humidity_percent":
 
42,
 
  
"lateral_g":
 
0.32,
 
  
"longitudinal_g":
 
-0.18,
 
  
"vertical_g":
 
1.05
 
}
 
Alert
 
topic:
 
vehicle/cabin/alert
,
 
event-driven:
 
{
 
  
"temp_c":
 
47.8,
 
  
"humidity_percent":
 
42,
 
  
"lateral_g":
 
0.85,
 
  
"longitudinal_g":
 
-0.18,
 
  
"vertical_g":
 
2.4,
 
  
"trigger":
 
"vertical_g"
 
}
 
5.
 
Expected
 
Use
 
 
Installed
 
in
 
a
 
vehicle,
 
streaming
 
cabin
 
comfort
 
and
 
ride
 
data
 
to
 
a
 
dashboard
 
for
 
driver
 
behavior
 
review
 
or
 
ride
 
comfort
 
tracking,
 
with
 
immediate
 
alerts
 
on
 
extreme
 
cabin
 
or
 
dynamics
 
events.