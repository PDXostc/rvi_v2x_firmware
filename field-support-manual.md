# V2X Field support documentation

This documentation is intended to cover common V2X board use cases in the field.

Please note that the documentation will include some information specific to the
GENIVI Smart-Cities project.

## Contents
* V2X board description
* [Requiremments for Nominal V2X Operation](#requirements-v2x)
    * [Power / OBD](#requirements-power)
    * [GPS / network antenna](#requirements-gps)
    * [SIM card](#requirements-sim)
* Requirements for smart-cities
    * Host computer
        * Raspberry Pi
        * GENIVI Yocto image
        * Smart Cities application
* Hardware setup
    * Cable connection
    * Case enclosure and mounting
    * Sim card insertion instructions
* Startup procedure
    * Power requirements
        * Battery warning
    * Nominal light states
    * Nominal smart cities
* Shutdown procedure
    * Button power off
        * Button failure or fallback procedure
    * Command options for power off (see also command set)
* Runtime operation
    * Vehicle safety notice
    * Smart-cities interaction
    * Verification
    * Toubleshooting
* V2X Command set
* Firmware upgrade
    - Tools required
    - Procedure

## Requiremments for Nominal V2X Operation <a name="requirements-v2x"></a>

### Power / OBD <a name="requirements-power"></a>

At minimum, the V2X requires a 12v power supply. Power requirements are met by a
standard OBDII connection.

> Please note: The board will draw power while in operation even if the vehicle
> engine is not engaged. Low power / suspend mode is not yet supported in
> firmware. It is recommended to power off the V2X board while vehicle is not in
> operation.

###  GPS / network antenna <a name="requirements-gps"></a>

GPS and cellular network connections require a combination GPS/GSM antenna.

### SIM card <a name="requirements-sim"></a>

Celllar network connectivity requires a properly provisioned SIM card.
  
## Hardware setup

> TODO: links to external hardware required

> Please note: SIM card must be inserted before board is powered on, and it is
> likely easier to do so before connecting cables.

Need:
* V2X Board
* OBDII to serial cable
* GPS/GSM antenna
* (2) Micro USB to USB cables

### Cable connection

1. Connect OBDII-serial cable into vehicle OBDII port (under dash) and the V2X
   serial port.
2. Connect GPS/GSM antenna cables.
    * Typical configuration requires attaching "gold" head to port marked "GPS
      Antenna" on board, and "silver" to port marked "3g Cellula Network
      Antenna". Incorrectly reversing this connection will inhibit GPS signal,
      and should be one of the first things to check if no GPS signal is
      observed.
3. Connect USB cable from port on board marked (Host PWR) to host computer micro
   usb power supply port. This will supply power to host computer (Raspberry Pi
   or other).
4. Connect micro USB from port on board marked (Host DATA) to free USB port on
   host computer. This is the line of communication between the board and the
   host.

### Case enclosure and mounting

### Sim card insertion instructions

Proper SIM card alignment is critical to cellular network operation.

Please insert SIM card so that
* Gold contacts face TOWARDS the board
* Chipped edge of card leads into the socket

>TODO: Do we need a diagram for this?

## Startup procedure

1. Press red button. The driver is configured to bring all components to full
   power. Please allow some seconds for host computer start and application
   initialization.

### Power requirements

V2X requires constant 12v power supply to sustain operation. Very informal testing shows average draw of ~0.15 amps during normal operation, with no host computer connected. When V2X is supplying power to a host such as a Raspberry Pi 3 with attached touch screen, power draw is ~0.6 amps.

#### Battery warning

>As noted above, the V2X firmware does not yet support low power / standby state. If left powered when vehicle is off, the vehicle battery could be drained significantly. Please power off the V2X (and host computer) when disengaging the vehicle.

### Nominal light states

### Nominal smart cities

## Shutdown procedure

### Button power off

### Button failure or fallback procedure
### Command options for power off (see also command set)
## Runtime operation
### Vehicle safety notice
### Smart-cities interaction
### Verification
### Toubleshooting
## V2X Command set
## Firmware upgrade
### Tools required
### Procedure