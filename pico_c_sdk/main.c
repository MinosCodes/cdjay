/**
 * CDJay - Pioneer CDJ-850 USB-MIDI Bridge for Raspberry Pi Pico 2W
 * Written in C using Raspberry Pi Pico C SDK & TinyUSB
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "bsp/board.h"
#include "tusb.h"

// -----------------------------------------------------------------------------
// Pin Definitions (Matching our breadboard prototype)
// -----------------------------------------------------------------------------
#define PIN_LED          PICO_DEFAULT_LED_PIN // Onboard LED
#define PIN_BTN_PLAY     15                   // GP15: Play/Pause Button
#define PIN_BTN_CUE      14                   // GP14: CUE Button
#define PIN_POT_PITCH    26                   // GP26: ADC0 (Pitch/Tempo Fader)
#define ADC_CHANNEL_PITCH 0                   // ADC Channel 0 (GP26)

// -----------------------------------------------------------------------------
// Pioneer CDJ-850 MIDI Specifications
// -----------------------------------------------------------------------------
#define MIDI_CABLE_NUM    0    // USB Virtual Cable 0
#define MIDI_CHANNEL      0    // Channel 1 (0-indexed: 0 = Channel 1)

// Standard MIDI Status Bytes for Channel 1
#define MIDI_STATUS_NOTE_ON    0x90
#define MIDI_STATUS_NOTE_OFF   0x80
#define MIDI_STATUS_CC         0xB0

// Note & Controller Numbers
#define NOTE_PLAY_PAUSE        60   // Note 60 (Middle C)
#define NOTE_CUE               61   // Note 61 (C#)
#define CC_PITCH_FADER         16   // Control Change 16

// -----------------------------------------------------------------------------
// State Tracking Variables
// -----------------------------------------------------------------------------
static bool last_play_pressed = false;
static bool last_cue_pressed  = false;
static uint8_t last_pitch_val = 0xFF; // Start with invalid value to force first read

// -----------------------------------------------------------------------------
// Helper: Send a 3-byte MIDI Packet over TinyUSB
// -----------------------------------------------------------------------------
void send_midi_message(uint8_t status, uint8_t data1, uint8_t data2) {
    uint8_t packet[3] = { status, data1, data2 };
    tud_midi_stream_write(MIDI_CABLE_NUM, packet, 3);
}

// -----------------------------------------------------------------------------
// Hardware Initialization
// -----------------------------------------------------------------------------
void hardware_init(void) {
    board_init();

    // 1. Onboard LED
    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);
    gpio_put(PIN_LED, 0);

    // 2. Play Button (GP15) with Internal Pull-Up Resistor
    gpio_init(PIN_BTN_PLAY);
    gpio_set_dir(PIN_BTN_PLAY, GPIO_IN);
    gpio_pull_up(PIN_BTN_PLAY);

    // 3. CUE Button (GP14) with Internal Pull-Up Resistor
    gpio_init(PIN_BTN_CUE);
    gpio_set_dir(PIN_BTN_CUE, GPIO_IN);
    gpio_pull_up(PIN_BTN_CUE);

    // 4. Pitch Slider (ADC0 / GP26)
    adc_init();
    adc_gpio_init(PIN_POT_PITCH);
    adc_select_input(ADC_CHANNEL_PITCH);
}

// -----------------------------------------------------------------------------
// MIDI Task: Poll Inputs and Transmit Packets
// -----------------------------------------------------------------------------
void midi_task(void) {
    // A. Check PLAY / PAUSE Button (Active LOW: 0 = Pressed, 1 = Released)
    bool play_pressed = !gpio_get(PIN_BTN_PLAY);
    if (play_pressed != last_play_pressed) {
        last_play_pressed = play_pressed;
        if (play_pressed) {
            gpio_put(PIN_LED, 1);
            // Send Note On: 0x90 0x3C 0x7F
            send_midi_message(MIDI_STATUS_NOTE_ON | MIDI_CHANNEL, NOTE_PLAY_PAUSE, 127);
        } else {
            gpio_put(PIN_LED, 0);
            // Send Note Off: 0x80 0x3C 0x00
            send_midi_message(MIDI_STATUS_NOTE_OFF | MIDI_CHANNEL, NOTE_PLAY_PAUSE, 0);
        }
    }

    // B. Check CUE Button (Active LOW)
    bool cue_pressed = !gpio_get(PIN_BTN_CUE);
    if (cue_pressed != last_cue_pressed) {
        last_cue_pressed = cue_pressed;
        if (cue_pressed) {
            gpio_put(PIN_LED, 1);
            // Send Note On: 0x90 0x3D 0x7F
            send_midi_message(MIDI_STATUS_NOTE_ON | MIDI_CHANNEL, NOTE_CUE, 127);
        } else {
            gpio_put(PIN_LED, 0);
            // Send Note Off: 0x80 0x3D 0x00
            send_midi_message(MIDI_STATUS_NOTE_OFF | MIDI_CHANNEL, NOTE_CUE, 0);
        }
    }

    // C. Check Pitch / Tempo Fader (ADC 12-bit: 0 to 4095 -> Scale to 7-bit: 0 to 127)
    uint16_t raw_adc = adc_read();
    uint8_t pitch_midi = (uint8_t)((raw_adc * 127) / 4095);

    // Only transmit when the fader value has changed
    if (pitch_midi != last_pitch_val) {
        last_pitch_val = pitch_midi;
        // Send Control Change: 0xB0 0x10 <value>
        send_midi_message(MIDI_STATUS_CC | MIDI_CHANNEL, CC_PITCH_FADER, pitch_midi);
    }
}

// -----------------------------------------------------------------------------
// Main Application Entry Point
// -----------------------------------------------------------------------------
int main(void) {
    // Initialize hardware peripherals
    hardware_init();

    // Initialize TinyUSB stack (device mode)
    tusb_init();

    while (1) {
        // Handle USB events
        tud_task();

        // Process MIDI inputs if USB is configured and connected
        if (tud_midi_mounted()) {
            midi_task();
        }

        // Small delay (1ms) for contact debouncing
        sleep_ms(1);
    }

    return 0;
}
