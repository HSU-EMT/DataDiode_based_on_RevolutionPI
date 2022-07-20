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
    <li><a href="#results">Results</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
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

For more examples, please refer to the [Documentation](https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/doc/README.md)

<p align="right">(<a href="#top">back to top</a>)</p>


## Instructions

<!-- Hardware -->
#### Hardware

1. First, the RevPi modules are connected together via PiBridge. The figure below is the PiBridge connector and its pin assignment. A total of 3 PiBridge connectors are required. In which 2 connectors are uncapped so that they can be soldered to the wire.
<div align="center">
  <a href="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/pin.jpg">
    <img src="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/pin.jpg" alt="pibridge" width="700" height="350">
  </a>
</div>

2. The diagram below shows how hardware devices are connected to each other. The author recommends readers to go through the Zybo Z7-20 board manual reference [here](https://digilent.com/reference/programmable-logic/zybo-z7/reference-manual?redirect=1) so that the diagram below can be easily understood. JF, JD or JC are the Pmod ports on the board. Where 0, 9, 10 and 13 are the numbers of the used pins. The Pibridge pins are connected to Revpi modules or soldered to wires. The other end of these wires is connected to the pins of the Pmod ports or the pins of the RS-485 Pmods. Two 100kOhm resistors are used to create the pull-down connections. Two RevPi Connect have been selected as controller modules, denoted as Controller-OT and Controller OT Mirror. A RevPi DIO module is connected to the Controller-OT on the left side.

<div align="center">
  <a href="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/schema.jpg">
    <img src="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/schema.jpg" alt="schema" width="700" height="350">
  </a>
</div>

<div align="center">
  <a href="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/hardwares.jpg">
    <img src="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/hardwares.jpg" alt="schema" width="700" height="400">
  </a>
</div>

3. 
<p align="right">(<a href="#top">back to top</a>)</p>

(NOT DONE YET)

<!-- Software -->
#### Software


<p align="right">(<a href="#top">back to top</a>)</p>


<!-- Results -->
## Results


<p align="right">(<a href="#top">back to top</a>)</p>



<!-- LICENSE -->
## License


<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Song Ha: song.ha@hsu-hh.de

Project Link: [Github](https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI)

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

<p align="right">(<a href="#top">back to top</a>)</p>



