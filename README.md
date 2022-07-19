<div id="top"></div>



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/Logo_Emt.png">
    <img src="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/Logo_Emt.png" alt="Logo" width="140" height="80">
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
* Other basic equipment like 2 LAN cables, some [PiBridge connector](https://revolutionpi.de/shop/en/plug-pibridge), some wires, soldering device, power supply, Oscilloscope (optional).


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

1. First, the revpi modules are linked together via PiBridge. The figure below is the PiBridge connector and its pin assignment. A total of 3 PiBridge connectors are required. In which 2 connectors are uncapped so that they can be soldered to the wire.
<div align="center">
  <a href="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/pin.jpg">
    <img src="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/pin.jpg" alt="pibridge" width="700" height="350">
  </a>
</div>

2. 

<div align="center">
  <a href="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/schema.jpg">
    <img src="https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/schema.jpg" alt="schema" width="700" height="350">
  </a>
</div>

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- Software -->
#### Software

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- Results -->
## Results

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Your Name - [@your_twitter](https://twitter.com/your_username) - email@example.com

Project Link: [https://github.com/your_username/repo_name](https://github.com/your_username/repo_name)

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

Use this space to list resources you find helpful and would like to give credit to. I've included a few of my favorites to kick things off!

* [Choose an Open Source License](https://choosealicense.com)
* [GitHub Emoji Cheat Sheet](https://www.webpagefx.com/tools/emoji-cheat-sheet)
* [Malven's Flexbox Cheatsheet](https://flexbox.malven.co/)
* [Malven's Grid Cheatsheet](https://grid.malven.co/)
* [Img Shields](https://shields.io)
* [GitHub Pages](https://pages.github.com)
* [Font Awesome](https://fontawesome.com)
* [React Icons](https://react-icons.github.io/react-icons/search)

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/othneildrew/Best-README-Template.svg?style=for-the-badge
[contributors-url]: https://github.com/othneildrew/Best-README-Template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/othneildrew/Best-README-Template.svg?style=for-the-badge
[forks-url]: https://github.com/othneildrew/Best-README-Template/network/members
[stars-shield]: https://img.shields.io/github/stars/othneildrew/Best-README-Template.svg?style=for-the-badge
[stars-url]: https://github.com/othneildrew/Best-README-Template/stargazers
[issues-shield]: https://img.shields.io/github/issues/othneildrew/Best-README-Template.svg?style=for-the-badge
[issues-url]: https://github.com/othneildrew/Best-README-Template/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: https://github.com/othneildrew/Best-README-Template/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew
[product-screenshot]: images/screenshot.png
[Next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white
[Next-url]: https://nextjs.org/
[React.js]: https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB
[React-url]: https://reactjs.org/
[Vue.js]: https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D
[Vue-url]: https://vuejs.org/
[Angular.io]: https://img.shields.io/badge/Angular-DD0031?style=for-the-badge&logo=angular&logoColor=white
[Angular-url]: https://angular.io/
[Svelte.dev]: https://img.shields.io/badge/Svelte-4A4A55?style=for-the-badge&logo=svelte&logoColor=FF3E00
[Svelte-url]: https://svelte.dev/
[Laravel.com]: https://img.shields.io/badge/Laravel-FF2D20?style=for-the-badge&logo=laravel&logoColor=white
[Laravel-url]: https://laravel.com
[Bootstrap.com]: https://img.shields.io/badge/Bootstrap-563D7C?style=for-the-badge&logo=bootstrap&logoColor=white
[Bootstrap-url]: https://getbootstrap.com
[JQuery.com]: https://img.shields.io/badge/jQuery-0769AD?style=for-the-badge&logo=jquery&logoColor=white
[JQuery-url]: https://jquery.com 


[vivado]: https://github.com/HSU-EMT/DataDiode_based_on_RevolutionPI/blob/main/others/images/vivado.jpg
[vivado-url]: https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vivado-design-tools/archive.html


