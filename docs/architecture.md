# Software Architecture

## Status Quo
### The USB-Device
A CDJ-850 registers itself as a USB-Device with 5 interfaces. 
- 0 Audio/Control - PIONEER CDJ-850
- 1 Audio/Streaming: The actual audio endpoint for interface 0
- 2 Audio/Control - PIONEER CDJ-850 MIDI
- 3 Audio/Streaming: The actual MIDI interface: USB MIDI Interface2
- 4 USB/HID: PIONEER CDJ-850 HID

#### Audio
The CDJs all have an analog audio output, and some have an additional digital audio ouput. Our device should therefore register as a USB-Audio device.

#### MIDI
The MIDI interface is designed as output only, meaning it can only send commands to the host, not receive information. This however, is a limitation of the original implementation. It doesn't mean that our implementation has to follow the same path. 

#### USB-HID
This is the most complex interface, since it allows bi-directional communication between the CDJ and the host-software. Output is parallel to the MIDI interface, meaning that both interfaces send out the same commands. On iOS, where custom HID-drivers are not available, the host-software has to revert to the MIDI interface. 

As communication is bi-directional, the host software can send information to the CDJ to be displayed on the integrated display or change the configuration of the user interface (turn on/off LEDs etc.).

## Concept

Our software will register as a CDJ-850 with all its interfaces. This part of the software is independent of the actual model we run on. 
Below this USB-layer, we will have an abstraction layer that bridges between the USB-devices and the actual hardware. There will be a number of function pointers that can be set (or left NULL), depending on whether the hardware supports that. 



