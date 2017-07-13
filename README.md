# rvi_v2x_firmware

## Description

This repo contains the firmware source and  design files for the JLR OSTC V2X
project, an open source hardware device equipped with
* CAN/OBD2 interface
* 2G/3G/GPS/GSM modem
* Accelerometer

This project, and its associated firmware and hardware, are under ongoing
development through the joint efforts of
* [Jaguar Land Rover Open Software Technology Center
(https://github.com/PDXostc)](https://github.com/PDXostc) and the
* [GENIVI Alliance (https://github.com/GENIVI)](https://github.com/GENIVI)

Hardware repo can be found at
[https://github.com/PDXostc/rvi_v2x_hardware](https://github.com/PDXostc/rvi_v2x_hardware).

> At the time of writing, current [firmware version 0.2](insert-link-here)
> intended to be used with the [hardware 2.1 release](https://github.com/PDXostc/rvi_v2x_hardware/commit/71e0d74038f05f68dc1f46546d6cc1ff5eac84f3).

Usage profile suggests pairing with a RaspberryPi host with 
[RVI node](https://github.com/GENIVI/rvi_lib) 
installed, for telemetry preprocessing and automated control. Suggested host
software is located with the 
[GENIVI Smart Cities pilot project yocto layer](https://github.com/GENIVI/meta-smart-cities-pilot)
and 
[GENIVI Smart Cities application](https://github.com/GENIVI/meta-smart-cities-pilot). 
Use of this software is not required for operation, but can provide a certain 
reference implementation.

## Installation

Please see the [*V2X-project-setup.pdf*](V2X-project-setup.pdf) for instructions
on correctly setting up the project in the Atmel environment and with the 
[Atmel Software Framework](http://www.atmel.com/tools/avrsoftwareframework.aspx).

Tips for use in the field can be found in the [Support Manual](field-support-manual.md), 
located in this repo.

Tools in use:
* [Atmel Studio 7 - Primary software
  IDE](http://www.atmel.com/tools/ATMELSTUDIO.aspx)
* [AVR Dragon - in-circuit programmer and
  debugger](http://www.atmel.com/tools/avrdragon.aspx)
* [FOXIT - PDF printer and
  viewer](https://www.foxitsoftware.com/products/pdf-reader/)
* [V2X development hardware](https://github.com/PDXostc/rvi_v2x_hardware), OSTC
  prototype device

## Contributors

Primary contact: [rstreif@partner.jaguarlandrover.com](rstreif@partner.jaguarlandrover.com)

Primary Development by Jesse Banks - jese@jopeldesigns.com.

Other Contributions from UCSD ECE191 Students Jaguar Land Rover OSTC - Portland,
Oregon 2015-2016.

Additional development and 0.2.0 release by Steve Miller - [stevemillercode@gmail.com](stevemillercode@gmail.com)

__Contributions and feedback are most welcome!__
Please use the Github [issue tracking system](https://github.com/PDXostc/rvi_v2x_firmware/issues) for bugs and suggestions. Please feel free to fork this repo and send a merge request if you'd like to make an improvement or fix.
