# NXP Application Code Hub
[<img src="https://mcuxpresso.nxp.com/static/icon/nxp-logo-color.svg" width="100"/>](https://www.nxp.com)

## Motion and Temperature Sensing on FRDM-A-S32K344 Using a Serial Terminal to Display the recollected Data.
This demo shows how to configure and use the LPI2C and GPIO drivers from the Real-Time Drivers (RTD / MCAL) package on the FRDM-A-S32K344 Evaluation Board using S32 Design Studio.
The example demonstrates motion and temperature monitoring using I2C sensor communication. The recollected data from the motion sensor is displayed on the RGB LED and the temperature sensor information is then sent via UART to a serial terminal using a baud rate of 115200.

#### Boards: FRDM-A-S32K344 
#### Categories: Communication
#### Peripherals: LPI2C, UART
#### Toolchains: S32 Design Studio IDE

## Table of Contents
1. [Software and Tools](#step1)
2. [Hardware](#step2)
3. [Setup](#step3)
4. [Results](#step4)
5. [Support](#step6)
6. [Release Notes](#step7)

## 1. Software and Tools<a name="step1"></a>
This example was developed using the FRDM Automotive Bundle for S32K3. To download and install the complete software and tools ecosystem, use the following link:
[S32K3 FRDM Automotive Board Installation Package](https://www.nxp.com/app-autopackagemgr/automotive-software-package-manager:AUTO-SW-PACKAGE-MANAGER?currentTab=0&selectedDevices=S32K3&applicationVersionID=156)

## 2. Hardware<a name="step2"></a>
### 2.1 Required Hardware
- Personal Computer
- Type-C USB cable
- [FRDM-A-S32K344](https://www.nxp.com/design/design-center/development-boards-and-designs/S32K344MINI-EVB)[<p align="center"><img src="https://www.nxp.com/assets/images/en/dev-board-image/S32K344-EVB-TOP.png" width="400"/></p>](https://www.nxp.com/assets/images/en/dev-board-image/S32K344-EVB-TOP.png)

### 2.2 Debugger Connection
- Connect the FRDM-A-S32K344 board to your Personal Computer using the Type-C USB cable

## 3. Setup<a name="step3"></a>

### 3.1 Import the Project into S32 Design Studio IDE
1. Open S32 Design Studio IDE, in the Dashboard Panel, choose **Import project from Application Code Hub**.
[<p align="center"><img src="images/import_project_1.png" width="400"/></p>](./images/import_project_1.png)

2. You can find the demo you need by searching for the name directly. Open the project, click the **GitHub link**, S32 Design Studio IDE will automatically retrieve project attributes then click **Next>**.
[<p align="center"><img src="images/import_project_2.png" width="600"/></p>](./images/import_project_2.png) 
[<p align="center"><img src="images/import_project_3.png" width="600"/></p>](./images/import_project_3.png)

3. Select **main** branch and then click **Next>**.
[<p align="center"><img src="images/import_project_4.png" width="600"/></p>](./images/import_project_4.png)

4. Select your local path for the repo in **Destination->Directory** window. The S32 Design Studio IDE will clone the repo into this path, click **Next>**.
[<p align="center"><img src="images/import_project_5.png" width="600"/></p>](./images/import_project_5.png)

5. Select **Import existing Eclipse projects** then click **Next>**.
[<p align="center"><img src="images/import_project_6.png" width="600"/></p>](./images/import_project_6.png)

6. Select the project in this repo (only one project in this repo) then click **Finish**.
[<p align="center"><img src="images/import_project_7.png" width="600"/></p>](./images/import_project_7.png)

### 3.2 Generating, Building and Running the Example
1. In Project Explorer, right-click the project and select **Update Code and Build Project**. This will generate the configuration (Pins, Clocks, Peripherals), update the source code and build the project using the active configuration (e.g. Debug_FLASH). Make sure the build completes successfully and the *.elf file is generated without errors.
[<p align="center"><img src="images/UpdateCodeAndBuildProject.png" width="500"/></p>](./images/UpdateCodeAndBuildProject.png)
Press **Yes** in the **SDK Component Management** pop-up window to continue.

2. Go to **Debug** and select **Debug Configurations**. Select **GDB PEMicro Interface Debugging**:
[<p align="center"><img src="images/DebugConfigurations.png" width="500"/></p>](./images/DebugConfigurations.png)

Use the controls to control the program flow.

## 4. Results<a name="step4"></a>
After flashing and running the application on the FRDM-A-S32K344:
- The system continuously monitors temperature and acceleration data over LPI2C.
- If a shake event is detected:
    - The RGB LED performs a triple-flash animation.
    - A cooldown period prevents repeated triggering.
- Board orientation is displayed using LED color coding:
    - Positive X axis → Green
    - Negative X axis → Cyan
    - Positive Y axis → Red
    - Negative Y axis → Yellow
    - Positive Z axis → Blue
    - Negative Z axis → Magenta
- The Temperature information will be sent via UART to a serial terminal using a baud rate of 115200

## 5. Support<a name="step6"></a>
For general technical questions related to NXP microcontrollers, please use the *[NXP Community Forum]((https://community.nxp.com/))*.
#### Project Metadata

<!----- Boards ----->
[![Board badge](https://img.shields.io/badge/Board-FRDM&ndash;A&ndash;S32K344-blue)]()

<!----- Peripherals ----->
[![Peripheral badge](https://img.shields.io/badge/Peripheral-LPUART-yellow)](https://mcuxpresso.nxp.com/appcodehub?peripheral=i2c)
[![Peripheral badge](https://img.shields.io/badge/Peripheral-LPI2C-yellow)](https://mcuxpresso.nxp.com/appcodehub?peripheral=i2c)

<!----- Toolchains ----->
[![Toolchain badge](https://img.shields.io/badge/Toolchain-S32%20Design%20Studio%20IDE-orange)](https://mcuxpresso.nxp.com/appcodehub?toolchain=s32_design_studio_ide)

Questions regarding the content/correctness of this example can be entered as Issues within this GitHub repository.

>**Warning**: For more general technical questions regarding NXP Microcontrollers and the difference in expected functionality, enter your questions on the [NXP Community Forum](https://community.nxp.com/)

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/NXP_Semiconductors)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/nxp-semiconductors)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/nxpsemi/)
[![Follow us on Twitter](https://img.shields.io/badge/X-Follow%20us%20on%20X-black.svg)](https://x.com/NXP)


## 6. Release Notes<a name="step7"></a>
| Version | Description / Update                           | Date                        |
|:-------:|------------------------------------------------|----------------------------:|
| 1.0     | Initial release on Application Code Hub        |February 24<sup>th</sup> 2026|