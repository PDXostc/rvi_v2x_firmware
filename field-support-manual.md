# V2X Field Support Documentation

> Work in progress. Some entries may be incomplete or missing.

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
    * [Sim card insertion instructions](#sim-card-instructions)
    * [Bench Power Supply](#bench-power-supply)
* [Startup procedure](#startup-procedure)
    * [Power requirements](#power-requirements)
        * [Battery warning](#battery-warning)
    * [Nominal light states](#nominal-light-states)
    * [Nominal smart cities](#nominal-smart-cities)
* [Shutdown procedure](#shutdown-procedure)
    * [Button power off](#button-power-off)
    * [Command options for power off (see also command set)](#command-power-off)
    * [Button failure or fallback procedure](#button-failure)
    * [When all else fails](#when-fails)
* [V2X Interface and Control](#runtime-operation)
    * [Communication Ports](#communication-ports)
        * [Linux](#communication-ports-linux)
        * [Windows](#communication-ports-windows)
    * [Smart-cities interaction](#smart-cities-interaction)
    * [Cellular](#cellular-operations)
    * [Verification](#verification)
    * [Toubleshooting](#troubleshooting)
* [V2X Command set](#v2x-command-set)
* [Firmware upgrade](#firmware-upgrade)
    * [Tools required](#firmware-tools-required)
        * [Programmer/Debugger](#tools-programmer)
    * [Programmer Connection](#programmer-connection)
        * [Example: AVR Dragon](#programmer-connection-dragon)
    * [Procedure](#firmware-procedure)
        * [Atmel Studio](#firmware-atmel-studio)
        * [AVRDUDE](#firmware-avrdude)

## V2X Board Description <a name="v2x-board-description"></a>

The Vehicle-to-Everything (V2X) board is a telemetry aggregation and delivery
device. It provides access to:

* [ADXL345BCCZ](http://www.mouser.com/ds/2/609/ADXL345-879145.pdf) Accelerometer configuration and dedicated data stream
* CAN/OBD interface via STN110 (mimic of ELM327)
* GPS (requires antenna)
* GSM network connectivity, via SIMCOM chipset (requires 3g antenna, 3g SIM
  card)
* Power management of USB for host computer

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

Cellular network connectivity requires a properly provisioned (activated) SIM card. The V2X has a [full size SIM card socket](http://www.mouser.com/ds/2/276/0475531001_MEMORY_CARD_SOCKET-146294.pdf), allowing use of any other sized SIM card. [Adapters](https://www.amazon.com/dp/B00R25GJJW) are available to adapt up to full size. 


## Requirements for Smart Cities <a name="requirements-smart-cities"></a>

### Host Computer <a name="requirements-host-computer"></a>

#### Raspberry Pi <a name="requirements-raspberry-pi"></a>

#### GENIVI Yocto image <a name="requirements-yocto"></a>

#### Smart Cities application <a name="requirements-smart-cities-application"></a>

## Hardware setup <a name="hardware-setup"></a>

> TODO: links to external hardware required

Needed:

* V2X Board (firmware flashed) mounted in enclosure.
* OBDII to DB-9 cable
* GPS/GSM antenna
* (2) Micro USB to USB cables
* Raspberry Pi and screen assembly

### Cable connection <a name="cable-connection"></a>

1. Connect GPS/GSM antenna cables. There are two possible types (screw type [APAMPS-106](http://www.mouser.com/ds/2/3/APAMPS-106-245254.pdf), tape type [MA-203.A](http://www.mouser.com/ds/2/398/MA.203%20GPS-Penta-band%20Cellular%20Stingray%20110310-2041.pdf)) which have exactly opposite connector colors. The V2X board has markings specifying the connector colors for the APAMPS-106 antenna. The MA-203 has labels attached to the wires adjacent to the connectors. The RF connectors on the V2X board are labeled with GPS and GSM, be mindful when making these connections.
2. Connect a USB cable from the V2X [Type-A](https://upload.wikimedia.org/wikipedia/commons/thumb/5/5d/Types-usb_th1.svg/220px-Types-usb_th1.svg.png) port to the LCD [Micro-B](https://upload.wikimedia.org/wikipedia/commons/thumb/5/5d/Types-usb_th1.svg/220px-Types-usb_th1.svg.png) power port. This cable will supply power to screen and Raspberry Pi.
3. Connect a USB cable from an available Raspberry Pi [Type-A](https://upload.wikimedia.org/wikipedia/commons/thumb/5/5d/Types-usb_th1.svg/220px-Types-usb_th1.svg.png) port to the V2X [Micro-B](https://upload.wikimedia.org/wikipedia/commons/thumb/5/5d/Types-usb_th1.svg/220px-Types-usb_th1.svg.png) labeled (Host DATA). This is the line of communication between the V2X board and the
   host.
4. Connect [OBDII-DB9 cable](http://www.obd2plugs.com/uploadfile/20130213102233930.jpg) to vehicle OBDII port (usually under dash) and V2X-DB-9 port.
5. Secure the wires to the V2X case 'porch' with zip ties.

> TODO: Pictures!

### Sim card insertion instructions <a name="sim-card-instructions"></a>

> TODO: Pictures!

Proper SIM card alignment is critical to cellular network operation.

Please insert SIM card so that:
* Gold contacts face TOWARDS the circuit board
* Cropped corner of card leads into the socket
* Fully inserted card will engage the spring lock

To remove the card, push in until the spring lock releases, and the card will be
ejected partially from the slot.

>TODO: Do we need a diagram for this?

### Bench Power Supply <a name="bench-power-supply"></a>

In lieu of an OBD2 connection, the V2X can be powered by any cable that meets the 
power pinout requirements of the OBD2->DB9 standard.

The V2X requires power through the DB9 connection. For bench testing, we use a
simple spliced cable, terminated DB9 at the V2X board and using either: positive
and ground wires running to a bench supply; barrel jack from wall supply. In
either case please ensure a 12v constant supply.

Our simple cable terminates on the DB9 end with
* *Power* - pin 9
* *GND* - pin 2

Note that the above configuration is just a power supply and will of course not 
support any CAN traffic.

For full pinout, refer to the table below.

**Pinning:**

| Pin Description   | OBDII | DB9  |
| ----------------- | ----- | ---- |
| J1850 BUS+        | 2     | 7    |
| Chassis Ground    | 4     | 2    |
| Signal Ground     | 5     | 1    |
| CAN High J-2284   | 6     | 3    |
| ISO 9141-2 K Line | 7     | 4    |
| J1850 BUS-        | 10    | 6    |
| CAN Low J-2284    | 14    | 5    |
| ISO 9141-2 L Line | 15    | 8    |
| Battery Power     | 16    | 9    |

[source](http://www.obd2plugs.com/uploadfile/20130213102233930.jpg)

## Startup procedure <a name="startup-procedure"></a>

1. Press red button for 1 second. The LED's will momentarily flash and the V2X will boot and start the host. 
2. If the car state check is running (default); 
   1. The first delay before checking for CAN activity is 60 seconds. 
   2. If no CAN activity is found, V2X will change to a low power mode that has no LED indication. 
   3. To use the V2X on the bench without a CAN source, send "VXSDC", to disables the CAN search.
3. When the Cell modem comes online and connects to the V2X, the lights on board should be visible.

### Power requirements <a name="power-requirements"></a>

V2X requires constant 12v power supply to sustain operation. Very informal
testing shows average draw of 0.1-0.2 amps during normal operation, with no host
computer connected. When V2X is supplying power to a host such as a Raspberry Pi
3 with attached touch screen, power draw is ~0.6 amps. Since the car should be running while the system is on, 

#### Battery warning <a name="battery-warning"></a>
it is assumed that if the car is running the alternator is maintaining the battery and the voltage is not checked. If the car state check discovers the car is 'off' the V2X will also check the battery voltage. If the battery has reached 11V, the V2X will power itself completely off in order to protect the battery and hopefully still allow the engine to be started. Once off, the power draw is micro-amps therefore negligible in this application.

In low power mode, the rev 2.1 board draws about 60mA while observing the car. A typical economy car sized battery should allow **Low power operation for about 15 days** without impacting drivability. If the vehicle will be unused for longer, switch the power off at the ODBii cable.

### Nominal light states <a name="nominal-light-states"></a>

Currently, the firmware drives the three on board LEDs respective to GSM modem
status.

* Red: SIM card insertion status.
    - Active: SIM card is inserted (properly)
    - Inactive: SIM card missing or inserted improperly
    - Flash: Car state check performed

* Blue: GSM power status.
    - Active: SIMCOM chipset is powered

* Green: GSM network state.
    - Steady: no network operator registered.
    - 800ms blink: operator selected, no network connection dialed.
    - 200ms blink: network connction dialed.

A steady green light likely indicates that the signal quality is not sufficient
to register and attach to the desired network operator.

> Please note: A bug existed in a previous firmware revision in the driver that can produce a state where the LEDs are not updated with the correct frequency if the host has not actively communicated with the board for some time. Until this issue is resolved, the host should use a scripted means of occasionally contacting the V2X control port. The distro built for Smart Cities contains a work around service for this issue.

If the lights are not visible, the device may be in a low power state. Pressing the button for <5 seconds should boot the V2X into high power for 10-60 seconds.

### Smart Cities Application

In addition, the color of the time and date lines under the GENIVI logo indicate
the status of GPS and RVI, respectively.

When GPS is active, the text displaying the time will be white. If the time is
displayed in pink text, GPS is in an error status. Check the connection or move
the vehicle to a location with a clear view of the sky.

When RVI is active, the date will be displayed in white text. If the date text
is pink, check that the SIM card is properly installed and activated. If the
date text is blue, contact [TBD] to verify that the RVI server is up and
running.

## Shutdown procedure <a name="shutdown-procedure"></a>

Shutdown is currently handled by cutting power to systems; there are multiple
methods available.

### Button power off <a name="button-power-off"></a>

The button on the V2X board can be used to control the power states.

Hold button for N seconds and release.

* \<5 second: Turn on all board components (including Host), including bring up from low power state
* \>5 seconds: Turn off V2X board

> While no LEDs currently dedicated to indicating power state of the board
> itself, powering off the board will deactivate the LEDs.

> 

### Command options for power off (see also command set) <a name="command-power-off"></a>

V2X board supports a command set over a USB-serial port. The V2X chip mounts as 3 sequential ttyACM devices and the GSM modem mounts as 5 ttyUSB devices. Connect a terminal window to the middle ttyACM port. There is no physical manifestation of these serial signals, so the port configuration settings are unimportant. Sending "VXPDA" will perform a complete system power down.  After that the V2X can only be woken by the button.

### Button failure or fallback procedure <a name="button-failure"></a>

> TODO: Improve this section with more details on using command set for power management and troubleshooting.

### When all else fails <a name="when-fails"></a>

Disconnect the board from the OBD port to cut power. Allow a few seconds, then
reconnect. Use the button repower the system.

### Nominal smart cities <a name="nominal-smart-cities"></a>

The Smart Cities application should initialize automatically after host system
start.

## V2X Interface and Control <a name="runtime-operation"></a>

> TODO: com/serial port description

A host computer can communicate with the components on board the device, via the
virtual ports mounted over USB. The exact arrangement and details of these ports
depend on the operating system of the host computer.

### Communication ports <a name="communication-ports"></a> 

> Depending on operating system, arrangement of the virtual ports is at the
> mercy of whatever order they are mounted in. This means that the serial port
> for a particular device may not remain consistent. It is encouraged for the
> host computer to use a rules system (like `udev` in Linux) to map consistent
> port aliases.

#### Linux <a name="communication-ports-linux"></a> 

The common port mounting arrangement observed in Linux:

* `/dev/ttyACM0` -> STN110/ELM327 CAN chip
* `/dev/ttyACM1` -> V2X control port (VX command set)
* `/dev/ttyACM2` -> Accelerometer data stream
* `/dev/ttyUSB0` -> SIMCOM diagnostic port (unused)
* `/dev/ttyUSB1` -> GPS stream
* `/dev/ttyUSB2` -> GSM modem AT command port (PPP dialer should use this for
  internet connection)
* `/dev/ttyUSB3` -> SIMCOM AT control (AT command set)

#### Windows <a name="communication-ports-windows"></a>

On Windows, the arrangement is slightly less intuitive, and the ports require a
driver to be installed. The requisite files, *atmel_devices_cdc.cat* and
*atmel_devices_cdc.inf* are available in the Atmel Software Framework, [on
Atmel's site](http://www.atmel.com/tools/avrsoftwareframework.aspx),
[Microchip](http://www.atmel.com/tools/avrsoftwareframework.aspx), or bundled
with Atmel Studio.

Communication with the SIMCOM chip requires drivers appropriate to the operating
system version. Repository is available
[here](http://simcom.ee/documents/?dir=SIM5320).

Common port mounting arrangement observed in Windows (designated by name, as
COM<x> is determined at runtime:

* `Ports (COM & LPT)`
* ->`Communication Device Class ASF example3, COM1` -> STN110/ELM327 CAN chip
* ->`Communication Device Class ASF example3, COM2` -> V2X control port (VX
  command set)
* ->`Communication Device Class ASF example3, COM3` -> Accelerometer data stream
  (and secret reset back channel)
* ->`Qualcomm HS-USB Diagnostics 9000` -> SIMCOM diagnostic port (unused)
* ->`Qualcomm HS-USB NMEA 9000` -> GPS stream
* ->`Sim Tech HS-USB AT Port 9000` -> SIMCOM AT control (AT command set)
* `Modems`
* ->`Qualcomm HS-USB Modem 9000` -> GSM modem AT command port (PPP dialer should
  use this for

### Verification <a name="verification"></a>
### Toubleshooting <a name="troubleshooting"></a>
## V2X Command set <a name="v2x-command-set"></a>

| Command    | Submodule | Action  | Description                              |
| ---------- | --------- | ------- | ---------------------------------------- |
| VX         |           |         | Display main menu                        |
| VX?        |           | ?       | Display main menu                        |
| VXG        |           | G       | GENIVI Logo                              |
| VXI        |           | I       | V2X Device Information                   |
| VXQ        |           | Q       | Query system status                      |
| VXR        |           | R       | Restart system                           |
| VXV        |           | V       | Verbose communication toggle             |
| VXA?       | A         | ?       | Display Accelerometer menu               |
| VXAD       | A         | D       | Disable the accelerometer                |
| VXAE       | A         | E       | Enable the accelerometer                 |
| VXAF       | A         | F       | Set Fixed/10-bit resolution              |
| VXAG       | A         | G       | Get single Accelerometer sample          |
| VXAI       | A         | I       | Accelerometer device information         |
| VXAQ       | A         | Q       | query Accelerometer state                |
| VXAR       | A         | R       | Restart the Accelerometer                |
| VXAWxx     | A         | Wxx     | Set G-range                              |
| VXASxxxx   | A         | Sxxxx   | Change the sample rate to xxx            |
| VXAXxxxx   | A         | Xxxxx   | Accelerometer X axis offset (zero)       |
| VXAYxxxx   | A         | Yxxxx   | Accelerometer Y axis offset (zero)       |
| VXAZxxxx   | A         | Zxxxx   | Accelerometer Z axis offset (zero)       |
| VXA        | A         |         | Display Accelerometer menu               |
| VXC?       | C         | ?       | Display CAN menu                         |
| VXCA       | C         | A       | Use init string in EE (action)           |
| VXCD       | C         | D       | Disable the CAN interface                |
| VXCE       | C         | E       | Enable the CAN interface                 |
| VXCI       | C         | I       | CANbus device information                |
| VXCQ       | C         | Q       | Query CAN state                          |
| VXCR       | C         | R       | Restart the CAN                          |
| VXCSttt    | C         | Sttt    | Store init string to EE                  |
| VXCW       | C         | W       | Show init string in EE                   |
| VXCXttt    | C         | Xttt    | Command passthrough                      |
| VXC        | C         |         | Display CAN menu                         |
| VXM?       | M         | ?       | Display modem menu                       |
| VXMD       | M         | D       | Disable the Modem                        |
| VXME       | M         | E       | Enable the Modem                         |
| VXMG       | M         | G       | Enable GPS                               |
| VXMI       | M         | I       | Modem device information                 |
| VXMQ       | M         | Q       | Modem state query                        |
| VXMR       | M         | R       | Rerestart the Modem                      |
| VXMS       | M         | S       | Stop GPS lock test                       |
| VXMT       | M         | T       | Test GPS lock time                       |
| VXMXttt    | M         | Xttt    | Command passthrough                      |
| VXM        | M         |         | Display Modem menu                       |
| VXM/       | M         | /       | Logical power down SIM chip              |
| VXP?       | P         | ?       | Display power menu                       |
| VXPD3      | P         | D3      | Disable the 3V power supply              |
| VXPD4      | P         | D4      | Disable the 4V power supply              |
| VXPD5      | P         | D5      | Disable the 5V power supply              |
| VXPDA      | P         | DA      | Disable all power                        |
| VXPDDx     | P         | DDx     | Disable host with delay                  |
| VXPDH      | P         | DH      | Disable the Host power port              |
| VXPE3      | P         | E3      | Enable the 3V power supply               |
| VXPE4      | P         | E4      | Enable the 4V power supply               |
| VXPE5      | P         | E5      | Enable the 5V power supply               |
| VXPEH      | P         | EH      | Enable the Host power port               |
| VXPH       | P         | H       | Go to high powerr state                  |
| VXPL       | P         | L       | Go to low poower mode                    |
| VXPQ       | P         | Q       | Power state query                        |
| VXPR       | P         | R       | Reset to dafaults                        |
| VXPY       | P         | Y       | Wake up event query                      |
| VXP        | P         |         | Display power menu                       |
| VXS?       | S         | ?       |                                          |
| VXSDx      | S         | Dx      | Disable sleep-state checks               |
| VXSEx      | S         | Ex      | Enable sleep-state checks                |
| VXSI       | S         | I       | Info about the submenu                   |
| VXSQ       | S         | Q       | Query the states and timeouts of the sleep-state checks |
| VXSTHxxxxx | S         | THxxxxx | Set the high-power sleep-state check timeout to value, in seconds |
| VXSTLxxxxx | S         | TLxxxxx | Set the low-power sleep-state check timeout to value, in seconds |
| VXS        | S         |         |                                          |
| VXT?       | T         | ?       | display timer menu                       |
| VXTA       | T         | A       | Set an alarm for UET                     |
| VXTD       | T         | D       | disable wakeup timers                    |
| VXTDn      | T         | Dn      | Set daylight saings mode                 |
| VXTG       | T         | G       | get current sytem time                   |
| VXTI       | T         | I       | Timer information                        |
| VXTQ       | T         | Q       | query time status                        |
| VXTR       | T         | R       | Set alarm with relative offset           |
| VXTS       | T         | S       | Set ssytem time with UET                 |
| VXTU       | T         | U       | attemp to update time from GPS           |
| VXTWxxxxx  | T         | Wxxxxx  | Wakeup timer set for xxxxx seconds from now |
| VXTZnnn    | T         | Znnn    | Set clock time zone                      |
| VXT        | T         |         | display timer menu                       |



## Firmware upgrade <a name="firmware-upgrade"></a>

> TODO: section should describe firmware, locations, etc, as well as tools.

The V2X firmware is in ongoing development. Unfortunately there is no easy (or
remote) way to upgrade the firmware; instead development tools are required.

Using a hardware programmer and a software utility, the V2X device can be
flashed with new firmware.

> Recent firmware releases should be available on the repo.

> TODO: Make releases available and link to them here.

If you wish to compile an image from source, firmware image should be compiled
into a *.hex* format. The AVR toolchain is available for
[Linux](http://www.atmel.com/toolsATMELAVRTOOLCHAINFORLINUX.aspx),
[Windows](http://www.atmel.com/tools/atmelavrtoolchainforwindows.aspx), and
comes bundled in [Atmel Studio](http://www.atmel.com/microsite/atmel-studio/).

### Atmel Studio <a name="firmware-atmel-studio"></a>

> A full reiteration of the programming instructions for the board are currently
> out of scope of this document.

Atmel Studio provides an easy programming interface for loading firmware .hex or .elf
files onto the board. 

[Atmel Studio Programming Dialog
documentation](http://www.atmel.com/webdoc/atmelstudio/atmelstudio.AVRStudio.ProgrammingDialog.Introduction.html)

### Dragon Programmer/Debugger <a name="tools-programmer"></a>

The AVR Dragon is the chosen programmer because of it's native integration in Atmel Studio and because it offers many tools and features for developers for a low price.

[AVR Dragon product page](http://www.atmel.com/tools/avrdragon.aspx)

> TODO: AVR Dragon or compatible debugging board

###  Connection to the V2X Board <a name="programmer-connection"></a>
The V2X programming connector is a 6-pin (2x3) 0.1in header near the corner of the board. 

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

### Verify Programmer Connection<a name="verify-connection></a>

1. Open programming dialog from menu `Tools -> Device Programming` or use shortcut `Ctrl+Shift+P`
2. Select the attached Dragon programmer from the `Tool`  dropdown menu
3. Select `ATxmega128A4U` from the `Device` drop down list
4. `PDI` is the only interface option, click `Apply`
5. Press and hold the  button on the  V2X
6. Under `Device Signature` click `Read`
7. If the signature window quickly fills with `0x1E9746`, then communications are established
8. release V2X button

### Firmware Upgrade Procedure <a name="firmware-procedure"></a>

> During the flashing process, power must be maintained to the board. A constant
> 12v power supply is required. In the current hardware revision, the 4v rail
> must be held high by pressing and holding the button prior to and during the
> flashing process.

9. Move to the `Memories` tab on the right side
10. In the  `Flash` tab, click the ellipsis `...` to select the .elf or .hex file to be programmed
11. Check the `Erase` and `Verify` radio buttons
12. Press the V2X button
13. Click `Program` 
14. The erase, program, and verify steps should all report "OK"
15. Release the V2X button

### Alternat programming w/ AVRDUDE <a name="firmware-avrdude"></a>

AVRDude is a cross platform (Linux/Windows) programming software utility.

[AVRDUDE Documentation](http://nongnu.askapache.com/avrdude/avrdude-doc-6.3.pdf)

Please read the above documentation for details and options for using AVRDUDE.

Ultimately, the usage will boil down to something like this:

```
avrdude -p x128a4u -c <programmer-id> -e -U flash:w:<file-name>.hex
```
### Using the GPS test

There exists a test function to repeatedly cold acquire GPS lock and print the results.  This test should be the only automatic control sequence running, "VXSDC" to the V2X control port `ttyACM1` will stop the car state checks. Sending "VXMT" to the V2X control port will start the GPS lock time test.

The control port is spammed with plenty of SIM module GPS query chatter during the test, so the results of the test are sent to the Accelerometer stream `ttyACM2` for consolidated results. Test results are in seconds and appear like this:

`Acquisition time: 44`
`Acquisition time: 46`
`Acquisition time: 38   `

The Accelerometer stream is automatically stopped when the test begins. As the test progresses a "spinner" ( |, \, -, / ) shows there is test activity, the test automatically repeats, and recovers from errors until it is stopped with "VXMS" command. The Accelerometer can then be restarted with VXAE.

## Links and Resources <a name="links-resources"></a>

## FAQ

**Q**: Why does the OBU occasionally take longer to boot or appear to reboot
several times when starting up

**A**: There is an issue with the graphics compositor in the GENIVI Development
Platform and it occasionally does not load properly. Normally, this would cause
a command line to appear instead of the GUI. We have created a workaround to
detect this condition and reload the compositor.  A bug has been logged.


