# V2X Field Support Documentation

This documentation is intended to cover common V2X board use cases in the field.

Please note that the documentation will include some information specific to the
GENIVI Smart-Cities project.

> ## Vehicle Safety Warning
> * Do not attempt maintenance, operation, troubleshooting, or anything in this
>   manual while the vehicle is in motion or operation.
> * Use of the V2X board and any related hardware or software is entirely at
>   your own risk. The authors and maintainers assume no responsibility or
>   liability whatsoever for any injury or damage to property anywhere, ever.
> * As a powered device, the V2X could drain a vehicle battery. Turn off the
>   device when turning off the vehicle. Disconnect the device when leaving the
>   vehicle unattaned.

## Contents
* [V2X board description](#v2x-board-description)
* [Requiremments for Nominal V2X Operation](#requirements-v2x)
    * [Power / OBD](#requirements-power)
    * [GPS / network antenna](#requirements-gps)
    * [SIM card](#requirements-sim)
* [Requirements for smart-cities](#requirements-smart-cities)
    * [Host computer](#requirements-host-computer)
        * [Raspberry Pi](#requirements-raspberry-pi)
        * [GENIVI Yocto image](#requirements-yocto)
        * [Smart Cities application](#requirements-smart-cities-application)
* [Hardware setup](#hardware-setup)
    * [Cable connection](#cable-connection)
    * [Case enclosure and mounting](#case-enclosure-and-mounting)
    * [Sim card insertion instructions](#sim-card-instructions)
* [Startup procedure](#startup-procedure)
    * [Power requirements](#power-requirements)
        * [Battery warning](#battery-warning)
    * [Nominal light states](#nominal-light-states)
    * [Nominal smart cities](#nominal-smart-cities)
* [Shutdown procedure](#shutdown-procedure)
    * [Button power off](#button-power-off)
        * [Button failure or fallback procedure](#button-failure)
    * [When all else fails](#when-fails)
    * [Command options for power off (see also command set)](#command-power-off)
* [Runtime operation](#runtime-operation)
    * [Vehicle safety notice](#vehicle-safety-notice)
    * [Smart-cities interaction](#smart-cities-interaction)
    * [Cellular](#cellular-operations)
    * [Verification](#verification)
    * [Toubleshooting](#troubleshooting)
* [V2X Command set](#v2x-command-set)
* [Firmware upgrade](#firmware-upgrade)
    - [Tools required](#firmware-tools-required)
    - [Procedure](#firmware-procedure)

## V2X Board Description <a name="v2x-board-description"></a>

The Vehicle-to-Everything (V2X) board is a telemetry aggregation and delivery
device. It provides access to:

* Accelerometer
* CAN/OBD interface via STN110/ELM327
* GPS (requires antenna)
* GSM network connectivity, via SIMCOM chipset (requires 3g antenna, 3g SIM
  card)
* Power over USB for host computer

Configuration and operation can be handled manually, but the device is intended
to be complemented by a host computer for automated operation.

## Requirements for Nominal V2X Operation <a name="requirements-v2x"></a>

### Power / OBD <a name="requirements-power"></a>

At minimum, the V2X requires a 12v power supply. Power requirements are met by a
standard OBDII-to-serial connection.

> Please note: The board will draw power while in operation even if the vehicle
> engine is not engaged. Low power / suspend mode is not yet supported in
> firmware. It is recommended to power off the V2X board while vehicle is not in
> operation.

###  GPS / network antenna <a name="requirements-gps"></a>

GPS and cellular network connections require a combination GPS/GSM 3g antenna.

### SIM card <a name="requirements-sim"></a>

Celllar network connectivity requires a properly provisioned SIM card.

## Hardware setup <a name="hardware-setup"></a>

> TODO: links to external hardware required

> Please note: SIM card should be inserted before board is powered on, and it is
> likely easier to do so before connecting cables.

Need:

* V2X Board
* OBDII to serial cable
* GPS/GSM antenna
* (2) Micro USB to USB cables

### Cable connection <a name="cable-connection"></a>

1. Connect OBDII-serial cable into vehicle OBDII port (under dash) and the V2X
   serial port.
2. Connect GPS/GSM antenna cables.
    > Typical configuration requires attaching
    >* "gold" head to port marked "**GPS Antenna**"
    >* "silver" head to port marked "**3g Cellula Network Antenna**"

    > Incorrectly reversing this connection will inhibit GPS signal, and should
      be one of the first things to check if no GPS signal is observed.

3. Connect USB cable from port on board marked (Host PWR) to host computer micro
   usb power supply port. This will supply power to host computer (Raspberry Pi
   or other).
4. Connect micro USB from port on board marked (Host DATA) to free USB port on
   host computer. This is the line of communication between the board and the
   host.

> TODO: Pictures!

### Case enclosure and mounting <a name="case-enclosure-and-mounting"></a>

> TODO: Pictures!

### Sim card insertion instructions <a name="sim-card-instructions"></a>

> TODO: Pictures!

Proper SIM card alignment is critical to cellular network operation.

Please insert SIM card so that
* Gold contacts face TOWARDS the board
* Chipped edge of card leads into the socket
* Fully inserted card will engage the spring lock

To remove the card, push in until the spring lock releases, and the card will be
ejected partially from the slot.

>TODO: Do we need a diagram for this?

## Startup procedure <a name="startup-procedure"></a>

1. Press red button. The driver is configured to bring all components to full
   power. Please allow some seconds for host computer start and application
   initialization.

When the host comes on line and connects to the V2X, the lights (red, blue,
green) on board should be visible.

### Power requirements <a name="power-requirements"></a>

V2X requires constant 12v power supply to sustain operation. Very informal
testing shows average draw of ~0.15 amps during normal operation, with no host
computer connected. When V2X is supplying power to a host such as a Raspberry Pi
3 with attached touch screen, power draw is ~0.6 amps.

> #### Battery warning <a name="battery-warning"></a>
> As noted above, the V2X firmware does not yet support low power / standby
state. If left powered when vehicle is off, the vehicle battery could be drained
significantly. Please power off the V2X (and host computer) when disengaging the
vehicle.

### Nominal light states <a name="nominal-light-states"></a>

Currently, the firmware drives the three on board LEDs respective to GSM modem
status.

* Red: SIM card insertion status.
    - Active: SIM card is inserted (properly)
    - Inactive: SIM card missing or instered improperly

* Blue: GSM power status.
    - Active: SIMCOM chipset is powered

* Green: GSM network state.
    - Steady: no network operator registered.
    - 800ms blink: operator selected, no network connection dialed.
    - 200ms blink: network connction dialed.

A steady green light likely indicates that the signal quality is not sufficient
to register and attach to the desired network operator.

> Please note: A bug currently exists in the driver that can produce a state
where the LEDs are not updated with the correct frequency if the host has not
actively communicated with the host for some time. Until this issue is resolved,
the host should use a scripted means of occaisionally contacting the V2X control
port. The distro built for Smart Cities contains a work around service for this
issue.

If the lights are not visible, this should indicate the device is not powered.

## Shutdown procedure <a name="shutdown-procedure"></a>

Shutdown is currently handled by cutting power to systems; there are multiple
methods available.

### Button power off <a name="button-power-off"></a>

The button on the V2X board can be used to control the power states.

Hold button for longer than N seconds and release.

* \> 1 second: Turn on all board components (including Host)
* \> 3 seconds: Turn off all board components (including Host), only Atmel CPU
  will retain power
* \> 5 seconds: Turn off V2X board

> While no LEDs currently dedicated to indicating power state of the board
> itself, powering off the board will deactivate the LEDs.

### Button failure or fallback procedure <a name="button-failure"></a>

V2X board supports a command set over a USB-serial port.

> TODO: Later section with details on using command set for power management and
troubleshooting.

### When all else fails <a name="when-fails"></a>

Disconnect the board from the OBD port to cut power. Allow a few seconds, then
reconnect. Use the button repower the system.

### Command options for power off (see also command set) <a name="command-power-off"></a>

> TODO: Describe contacting the V2X control port, and commands that can be used
for power state manipulation.

### Nominal smart cities <a name="nominal-smart-cities"></a>

The Smart Cities application should initialize automatically after host system
start.

## V2X Interface and Control<a name="runtime-operation"></a>

> TODO: com/serial port description

A host computer can communicate with the components on board the device, via the
virtual ports mounted over USB. The exact arrangement and details of these ports
depend on the operating system of the host computer.

### Micro controller / command port


### Communication ports 

> Depending on operating system, arrangement of the virtual ports is at the
> mercy of whatever order they are mounted in. This means that the serial port
> for a particular device may not remain consistent. It is encouraged for the
> host computer to use a rules system (like `udev` in Linux) to map consistent
> port aliases.

The common port mounting arrangement observed in Linux:

* `/dev/ttyACM0` -> STN110/ELM327 CAN chip
* `/dev/ttyACM1` -> V2X control port (VX command set)
* `/dev/ttyACM2` -> Accelerometer data stream (and secret reset back channel)
* `/dev/ttyUSB0` -> SIMCOM diagnostic port (unused)
* `/dev/ttyUSB1` -> GPS stream
* `/dev/ttyUSB2` -> GSM modem AT command port (PPP dialer should use this for
  internet connection)
* `/dev/ttyUSB3` -> SIMCOM AT control (AT command set)

### Verification <a name="verification"></a>
### Toubleshooting <a name="troubleshooting"></a>
## V2X Command set <a name="v2x-command-set"></a>

> TODO: Include V2X command set

|  Command     | Submodule | Action | Description                    |
|--------------|-----------|--------|--------------------------------|
| VXAD         | A         |  D     | Disable the accelerometer   
| VXAE         | A         |  E     | Enable the accelerometer    
| VXAI         | A         |  I     | Accelerometer device information
| VXAQ         | A         |  Q     | Accelerometer state query   
| VXAR         | A         |  R     | Restart the Accelerometer   
| VXASxxx      | A         | Sxxxx  | Change the sample rate to xxx   1,3,6,12,25,50,100,200,400,800,1600,3200
| VXAXxxx      | A         | Xxxxx  | Accelerometer X axis offset (zero)  -127 to 127 Number confirmed    
| VXAYxxx      | A         | Yxxxx  | Accelerometer Y axis offset (zero)  -127 to 127 Number confirmed    
| VXAZxxx      | A         | Zxxxx  | Accelerometer Z axis offset (zero)  -127 to 127 Number confirmed    
| VXAG         | A         |  G     | Get single Accelerometer sample 
| VXARxx       | A         | Rxx    | Set G-range 
| VXCD         | C         |  D     | Disable the CAN interface   
| VXCE         | C         |  E     | Enable the CAN interface    
| VXCI         | C         |  I     | CANbus device information   
| VXCQ         | C         |  Q     | CAN state information 
| VXCR         | C         |  R     | Restart the CAN 
| VXI          | I         |        | V2X   Device Information 
| VXMD         | M         |  D     | Disable the Modem   
| VXME         | M         |  E     | Enable the Modem    
| VXMI         | M         |  I     | Modem device information  
| VXMQ         | M         |  Q     | Modem state query   
| VXMR         | M         |  R     | Rerestart the Modem 
| VXPD3        | P         | D3     | Disable the 3V power supply
| VXPD4        | P         | D4     | Disable the 4V power supply
| VXPD5        | P         | D5     | Disable the 5V power supply
| VXPDH        | P         | DH     | Disable the Host power port
| VXPDDx       | P         | DDx    | Disable host with delay x: seconds
| VXPE3        | P         | E3     | Enable the 3V power supply 
| VXPE4        | P         | E4     | Enable the 4V power supply 
| VXPE5        | P         | E5     | Enable the 5V power supply 
| VXPEH        | P         | EH     | Enable the Host power port 
| VXPQ         | P         | Q      | Power state query       "3V3=x, 4V1=x, 5V0=x, HOST=x" 1=on 0=off    
| VXW          | W         |        | Wake   up event query         
| VXQ          | Q         |        | Whole   system status query     All other query results rolled into one    
| VXTD         | T         |    D   | disable wakeup timers         
| VXTWxxxx     | x         |        | Wxxxxx      Wakeup timer set for xxxxx seconds from now         
| VXR          | R         |        | V2X   board reset         
| VXS          | S         |        | SIMCARD   check     "SIMCARD=x" 1=in 0=out    

## Firmware upgrade <a name="firmware-upgrade"></a>

> TODO: section should describe firmware, locations, etc, as well as tools.

The V2X firmware is in ongoing development. Unfortunately there is no easy (or remote) way
to upgrade the firmware; instead development tools are required.

Using a hardware programmer and a software utility, the V2X device can be
flashed with new firmware.

> Recent firmware releases should be available on the repo.

> TODO: Make releases available and link to them here.

If you wish to compile an image from source, firmware image should be compiled into a *.hex* format. 
The AVR toolchain is available for
[Linux](http://www.atmel.com/toolsATMELAVRTOOLCHAINFORLINUX.aspx),
[Windows](http://www.atmel.com/tools/atmelavrtoolchainforwindows.aspx),
and comes bundled in 
[Atmel Studio](http://www.atmel.com/microsite/atmel-studio/). 

### Tools required <a name="firmware-tools-required"></a>

#### Programmer/Debugger

A plethora of hardware programming/debugger solutions are available. The AVR
Dragon is one such device.

[AVR Dragon product page](http://www.atmel.com/tools/avrdragon.aspx)

> TODO: AVR Dragon or compatible debugging board

### Programmer Connection to the V2X Board

#### Example: AVR Dragon

* Connect one end of the programming ribbon cable to the Dragon board on the set
  of pins marked *1 ISP 5*. Ensure that the red stripe on the ribbon is aligned
  with the *1* on the pin port.
* Connect the other end of the cable to the set of pins marked *PDI* on top of
  the V2X board. Align the connector so that the red stripe is closest to the
  bottom corner nearest the *PDI* marking.

> TODO: image of proper connection

To check if the cable has been correctly connected and aligned, use programming
software to read the voltage of the device while the board is powered. The
voltage should read **3.2v**.

### Firmware Upgrade Procedure <a name="firmware-procedure"></a>

#### Atmel Studio

> A full reiteration of the programming instructions for the board are currently
> out of scope of this document.

Atmel Studio supports an easy programming interface for loading firmware .hex
files onto the board.

[Atmel Studio Programming Dialog
documentation](http://www.atmel.com/webdoc/atmelstudio/atmelstudio.AVRStudio.ProgrammingDialog.Introduction.html)

#### AVRDUDE

AVRDude is a cross platform (Linux/Windows) programming software utility.


[AVRDUDE Documentation](http://nongnu.askapache.com/avrdude/avrdude-doc-6.3.pdf)

Please read the above documentation for details and options for using AVRDUDE.

Ultimately, the usage will boil down to something like this:

```
avrdude -p x128a4u -c <programmer-id> -e -U flash:w:<file-name>.hex
```
## Links and Resources



[v2x-board-connected-top]: [v2x-board-connected-under]:
[v2x-connections-facing]: [v2x-dragon-v2x-connected]:
[v2x-dragon-v2x-connected-detail]: [v2x-lights-on]:
[v2x-sim-card-alignment-under]: [v2x-sim-card-ejected]: [v2x-obd-connection]:
[v2x-mounted-enclosure]:
