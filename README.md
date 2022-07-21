<div id="top"></div>



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/Logo_Emt.png">
    <img src="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/Logo_Emt.png" alt="Logo" width="180" height="80">
  </a>

  <h3 align="center">A New Approach to Secure Industrial Automation Systems Based on Revolution Pi Modules</h3>

  <p align="center">
    A puplication at ETFA2022
    <br />
    <a href="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/doc/README.md"><strong>Explore the paper »</strong></a>
    <br />
  </p>
</div>


<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#required-hardware">Required Hardware</a></li>
        <li><a href="#required-software">Required Software</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li>
      <a href="#instructions">Instructions</a>
      <ul>
        <li><a href="#hardware">Hardware</a></li>
        <li><a href="#software">Software</a></li>
      </ul>
    </li>
    <li><a href="#test">Test</a></li>
    <li><a href="#others">Others</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

There are many data diodes products on the market. Most of them are Ethernet based with high bandwidth and bound to a specific protocol or manufacturer. They are often used to connect 2 complex OT and IT network. In cases where the OT network or the areas that need to be completely isolated within the OT network are small, the use of these data diodes often leads to increased configuration requirements, inefficiency and high costs. Another aspect to consider is the flexibility of data diodes of this type. In most cases, security must be considered from the beginning and maintained throughout all phases of system and network development. This means that these data diode cannot be added to an existing network without changing the network architecture.

The concept proposed here can solve the above problems of existing data diodes. It employs this open source and with comparably low resources customized data diode using the backplane bus of the [Revolution Pi](https://github.com/RevolutionPi) (RevPi) automation system to exchange data.

<p align="right">(<a href="#top">back to top</a>)</p>







<!-- GETTING STARTED -->
## Getting Started

#### Required Hardware

* [Zynq-7000 ARM/FPGA SoC Development Board](https://digilent.com/shop/zybo-z7-zynq-7000-arm-fpga-soc-development-board/): We use one Zybo Z7-20 board to build the data diode. Other board series may be suitable but not guaranteed. Also, if you use another board serie, you have to change the pins setting manually in [constrain](https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/project_1.srcs/constrs_1/new/constrain.xdc) file.
* [Revolution Pi Modules](https://revolutionpi.de/revolution-pi-serie/): Two [RevPi Connect](https://revolutionpi.de/revpi-connect/) and one [DIO module](https://revolutionpi.de/io-modul/) are used in this project.
* [RS-485 Pmod](https://digilent.com/reference/pmod/pmodrs485/start): Two RS-485 Pmod modules are required to build the RS-485 communication channel for the data diode.
* Other basic equipment like two LAN cables, three [PiBridge connector](https://revolutionpi.de/shop/en/plug-pibridge), some wires, soldering device, power supply, Oscilloscope (optional).


#### Required Software

* [Vivado Design Suite](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vivado-design-tools/archive.html): We use Vivado HLx Editions 2019.1. Other versions may be suitable but not guaranteed.
* [Xilinx Software Development Kit (SDK)](https://www.xilinx.com/support/documentation-navigation/design-hubs/dh0015-sdk-hub.html): We use SDK 2019.1 integrated with Vivado 2019.1. Recommend to take a look [here](https://digilent.com/reference/programmable-logic/guides/installing-vivado-and-sdk) to install all required softwares.


<p align="right">(<a href="#top">back to top</a>)</p>


<!-- USAGE -->
## Usage

For more details, please refer to the [Documentation](https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/doc/README.md)

<p align="right">(<a href="#top">back to top</a>)</p>


## Instructions

<!-- Hardware -->
#### Hardware

1. First, the RevPi modules are connected together via [PiBridge](https://revolutionpi.com/pibridge/). The figure below is the PiBridge connector and its pin assignment. A total of 3 PiBridge connectors are required. In which 2 connectors are uncapped so that they can be soldered to wires.
<div align="center">
  <a href="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/pin.jpg">
    <img src="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/pin.jpg" alt="pin" width="700" height="350">
  </a>
</div>

2. The diagram below shows how hardware devices are connected to each other. The author recommends readers to go through manual reference of the Zybo Z7-20 board [here](https://digilent.com/reference/programmable-logic/zybo-z7/reference-manual?redirect=1) and RS-485 Pmod [here](https://digilent.com/reference/pmod/pmodrs485/reference-manual) so that the diagram below can be easily understood. JF, JD or JC are the Pmod ports on the board. Where 0, 9, 10 and 13 are the numbers of the used pins. The Pibridge pins are connected to Revpi modules or soldered to wires. The other end of these wires is connected to the pins of the Pmod ports or the pins of the RS-485 Pmods. Two 100kOhm resistors are used to create the pull-down connections. Two RevPi Connect have been selected as controller modules, denoted as Controller-OT and Controller OT Mirror. A RevPi DIO module is connected to the Controller-OT on the left side.

<div align="center">
  <a href="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/schema.jpg">
    <img src="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/schema.jpg" alt="schema" width="700" height="350">
  </a>
</div>

<div align="center">
  <a href="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/hardwares.jpg">
    <img src="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/hardwares.jpg" alt="hardwares" width="700" height="400">
  </a>
</div>

3. [Power supply](https://revolutionpi.com/tutorials/uebersicht-revpi-connect/spannungsversorgung-anschliessen-connect/) for circuit boards and RevPi devices. Connect 2 controller modules via LAN cable to your laptop to be able to configure RevPi modules in PiCtory. See [here](https://revolutionpi.com/tutorials/uebersicht-revpi-connect/) and [here](https://revolutionpi.com/tutorials/was-ist-pictory-2/) for more details.

4. In PiCtory, two DIO modules and one RevPi Connect are configured for each Controller-OT and Controller-OT Mirror. The DIO module next to the RevPi Connect is set to default. The leftmost DIO module activates 6 counter channels as shown below.

<div align="center">
  <a href="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/pictory.jpg">
    <img src="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/pictory.jpg" alt="pictory" width="900" height="230">
  </a>
</div>


<p align="right">(<a href="#top">back to top</a>)</p>

<!-- Software -->
#### Software

1. Download or clone this repository.
2. Open `project_1.xpr` with Vivado. All IP cores and pin assignments have been designed. Click the button ``Generate Bitstream``. After bitstream has been created. Click on ``File`` -> ``Export`` -> ``Export Hardware``. Navigate to the folder ``applications`` and check on ``Include Bitstream``. Then click ``OK``.
3. Click on ``File`` -> ``Launch SDK``. Navigate to the folder ``applications`` for ``Exported location`` and ``Workspace``. Then click ``OK``. The SDK application will be opened.
4. In SDK, click on ``File`` -> ``Open Projects From File System`` and add folder ``core0``, ``core0_bsp``, ``core1``, ``core1_bsp``in the folder ``applications`` into workspace.
5. Right click on ``core1_bsp``. Select ``Board Support Package Setting``. There, add ``-DUSE_AMP`` for ``extra_compiler_flag`` of ``ps7_cortexa9_1``. This allows the use of interrupt controllers for multicore processors in Zynq, like [here](https://support.xilinx.com/s/article/62557?language=en_US).
6. Run two applications ``main_core1.c`` on processor core 1 and ``main_core0.c`` on processor core 0. If you are not familiar with it, we recommend that you check it out [here](https://www.youtube.com/watch?v=n0hbwp36hBs&t=412s).

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- Test -->
## Test


To test the setup, we connect 2 controllers to the laptop via SSH, e.g. with the [``Putty``](https://www.putty.org/) app, and use the ``piTest`` program like [here](https://www.youtube.com/watch?v=ug8WJmfFjYY) or [here](https://revolutionpi.com/tutorials/software/pitest-verwenden/). Change the value of any output or input variable of the DIO module next to Controller-OT module in ``piTest`` running on Controller-OT and read this change in ``piTest`` running on Controller-OT Mirror. If the change is successfully transferred, it means that your setup was successful.

<div align="center">
  <a href="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/pitest.jpg">
    <img src="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/pitest.jpg" alt="pitest" width="900" height="300">
  </a>
</div>

The output value of the real I/O modules connected to the Controller-OT is stored on response packet of the data diode (Fig. 5 in [Documentation](https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/doc/README.md)), namely on the bytes of the counter channel of the data diode (leftmost DIO module in PiCtory). Therefore, when read by ``piTest``, its name has the form Counter_N_... by default, where N is the number of the counter channel, which corresponds to the number of the real I/O module (counted from the Controller-OT to outside). You can change the names of these variables in ``PiCtory`` as you wish.

For more details, please refer to the [Documentation](https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/doc/README.md).


## Others

1. You can also connect more I/O modules to Controller-OT module. The process data of these I/O modules will be automatically transmitted to Controller-OT Mirror. For more details, please refer to the [Documentation](https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/doc/README.md).
2. The size of the response packet of the data diode can be changed by setting the number of active counter channels for the DIO module emulated by the data diode in PiCtory (leftmost DIO module in PiCtory). Each counter channel corresponds to four bytes and up to six counter channels are possibly active at the same time, corresponds to 24 free bytes to store output value of the real I/O modules. Because of the limited number of these bytes, we had to limit some features of real I/O modules connected to Controller-OT such as: Do not use PWM features for DO,DIO modules; do not use MIO modules. We will try to remove these limitations in the next data diode version.
3. In the PiCtory, users must set the feature of the I/O modules connected to Controller-OT Mirror and the corresponding I/O modules connected to Controller-OT identically to minimize the difference in cycle time between the two networks during the cyclic data exchange phase.
4. We are still improving the code and developing the project. Please report bugs if you find them.


Project Link: [Github](https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI)

<p align="right">(<a href="#top">back to top</a>)</p>



