# TinyUSB Configuration for CDJ-850 USB-MIDI Device

#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
 extern "C" {
#endif

// Set TinyUSB to Device Mode
#define CFG_TUD_ENABLED       1

// RHPort0 is used for Pico's USB
#define BOARD_DEVICE_RHPORT_NUM 0
#define CFG_TUSB_RHPORT0_MODE   OPT_MODE_DEVICE

// Enable MIDI Class Driver
#define CFG_TUD_MIDI          1

// MIDI FIFO buffer sizes
#define CFG_TUD_MIDI_RX_BUFSIZE   64
#define CFG_TUD_MIDI_TX_BUFSIZE   64

#ifdef __cplusplus
 }
#endif

#endif /* _TUSB_CONFIG_H_ */
