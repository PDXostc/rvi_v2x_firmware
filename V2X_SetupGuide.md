# V2X Project Setup Guide
## Brief: 
How to get the V2X project setup and ready to edit on your computer.
## Install Atmel Studio
1. Go to http://www.atmel.com/tools/ATMELSTUDIO.aspx
   Download the offline installer as a Guest if involvement is short. An account is not complicated if you will work on the project for some time. You will have to provide your email to get the installer (a link is sent).
2. Install Atmel Studio 7 and the USB drivers. This will require running the installer with “run as administrator” privileges. Be sure to use “Advanced view” when the question is asked.
## Clone GIT repo
3. GIT Clone the repo to a local project folder. [https://github.com/PDXostc/rvi_v2x_firmware](https://github.com/PDXostc/rvi_v2x_firmware "GIT repo")
```
git clone https://github.com/PDXostc/rvi_v2x_firmware
```
4. Switch to Develop branch of git repo.
## Load Generic ASF files
5. Navigate into the repo \rvi_v2x_firmware\V2X_Firmware and open the V2X.cproj file. Atmel studio should launch
6. Once the program launches, the ASF wizard should be displayed, select “V2X” from the project pull down.
7. Click the Version tab. Uncheck the "Trigger Upgrade of modules" checkbox. A dialog will launch for confirmation. click "Yes"
8. Downgrade the project by one ASF version to 3.33.0
9. Check the "Trigger Upgrade of modules" checkbox then return the project to the current version, 3.34.1. Click "Upgrade"
10. Follow the wizard, 
  1. Click “Next” 
  2. Select “No”
  3. Click "Finish" 
  4. After the upgrade, there is no need to check the log. Click OK.
## Setup ASF
11. Navigate back to the "Extension" tab. Verify these modules are contained in the project:
  If not present, select corresponding module from the “Available modules” field in the left pane
    1. Generic board Suport (driver)
    2. Calendar Functionality (service)
    3. Delay routines (service)
    4. DMA ­ Direct Memory Access (driver)
    5. GPIO ­ General purpose Input/Output (service)
    6. Interrupt management (Common API) (driver)
    7. IOPORT ­ General purpose I/O Service (service)
    8. IOPORT ­ Input/Output Port Controller (driver)
    9. Memory Control Access Interface (service)
    10. NVM ­ Non Volatile Memory (driver)
    11. NVM ­ Non volatile memory access (CommonAPI) (Driver)
      * No_extmem
    12. PMIC ­ Programmable Multi­level Interrupt Controller (driver)
    13. PWM service using timer/counter (service)
    14. RTC ­ Real Time Counter (driver)
    15. Sleep Controller driver (driver)
    16. Sleep manager (service)
    17. SPI ­ Serial Peripheral Interface (driver)
    18. SPI ­ Serial Periferal Interface Master (Common API) (service)
       * Standard_spi
    19. Standard serial I/O (stdio) (driver)
    20. System Clock Control (sevice)
    21. TC ­ Timer Counter (driver)
    22. USART ­ Serial interface (service)
    23. USART ­ Universal Synchronous/Asynchronous Reciever/Transmitter (driver)  
    24. USB Device (service)
      * cdc
12. Click “Apply”, then “Ok” when a confirmation dialog appears.
13. Save the project then Exit Atmel Studio
14. A popup dialog will ask to save the project. Click Yes. Click Save. the application will
exit.
## Move config files into place
15. Navigate to \rvi_v2x_firmware\V2X_Firmware\src\V2X\OtherFiles.
16. Follow the Readme.md (you will copy and overwrite some files)
## Build the project
17. Launch the the V2X.atsln project file in \rvi_v2x_firmware\V2X_Firmware. Atmel Studio will start. The ASF wizard tab can be closed. The project should successfully build (F7).
## Setup Dragon Programmer
18. Connect the V2X hardware to dragon, power and USB
19. Plug in the AVR Dragon programmer by USB (drivers should auto install)
20. Use the menu Debug ­> V2X Properties… (bottom)
21. Select the "Tool" Tab. If the dragon installed properly it should appear in the list. Select it, click save from the buttons along the top. Close the tab.
## Test Programmer connectivity with V2X
22. Verify the V2X hardware is powered on. If the V2X has never been installed before you will need to hold the button down while any programming operations are performed.
23. Select Device programming from the Tools menu.
24. Select the Dragon as the device, select the ATxmega128A4U from the drop down, PDI interface, then click "Apply”
25. Hold the V2X button down and click "device signature". If the chip is connected properly and operating the ID value 0x1E9746 will populate the Device Signature field and the Target Voltage field will also populate. If this test passes, close the window and move on. Failure discussion is beyond the scope of this document. 
## Program the V2X
26. Install the firmware to the V2X hardware using menu Debug -> Start Without Debugging. Again, you may need to hold the V2X button down to install the firmware.