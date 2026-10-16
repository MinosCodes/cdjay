# CDjay

## Introduction
The success of Pioneer's CDJ-Players is based on their usability and performance-oriented controls. And while the technology has evolved from CDs over SD-Cards to USB-Keys or direct interaction with a laptop, the control interface itself hasn't changed much. 
Modern CDJs, such as the CDJ-850, CDJ-900, CDJ-2000, and newer offer the possibility to directly connect to a host software, such as Algoriddim DJay, Native Instruments' Traktor, Serato Scratch, in order to be used as controller. 
The goal of this project is to bring this capability to older CDJ-Models in order to keep their control interface. 

## Related Work
A number of other projects have looked at the same idea. Some turning the CDJ into a MIDI-based controller, others integrating a RaspberryPi and running Mixxx, and open source DJ-Software on it, essentially turning them into full-blown, standalone, media-players. 

## Hardware
The goal is to provide drop-in replacement mainboards that communicate with the original hardware and bridge to a USB connection. All boards start from a Raspberry Pi Pico 2, but depending on the model, more or less additional hardware is required. See the individual files in the docs folder. 

## Interface
Since we want to focus on interfacing the old CDJs with the modern world (USB-C, Software, etc.), we use iPad mini running Algoriddim's djay in single deck mode, which gives us all the benefits such as touch interaction, integration to streaming services, etc.. Any iPad being able to run djay is fine, although interfacing is easier with the usb-c models. 

### Installation
Install djay from the App-Store on your iPad. Technically, it is possible to run djay from an iPhone, however, it doesn't support single-deck mode on smaller devices. 
	
	
### Connecting
If you have an iPad with a lightning-connector, you need a lightning to usb-converter that supports USB OTG. This will make the iPad the host of your USB connection, and the CDJay the client. 
For the USB-Models, just connect the USB-C connection to your iPad. 


## Acknowledgements

This project was supported by...

## Repositories
This work is part of a University project with its main focus is on teaching. 
The repository under https://git.fh-aachen.de/applab/projects/cdjay is the internal repo. 
The repository under https://github.com/applab-fhaachen/cdjay is a mirror to facilitate development with a larger community.

## Authors: 
Original contributors are 
- Florian H.
- Julius P.
- Konstantin P.
- Mike B.
- Nuno C.

## License
MIT License

All trademarks are property of their respective owners. This project is part of the education at FH Aachen University of Applied Sciences and not affiliated to any of the brand names. 
