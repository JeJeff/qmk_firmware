/* Copyright 2021 JeJeff
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _FN
};

// Some of the standard Zoom keyboard shortcuts you could use
#define KY_VIDEO    LALT(KC_V) // Zoom Alt+V: Start/stop video
#define KY_MICMUTE  LALT(KC_A) // Zoom Alt+A: Mute/unmute audio
#define KY_MUTEALL  LALT(KC_M) // Zoom Alt+M: Mute/unmute audio for everyone except host (Note: For the meeting host only)
#define KY_SHPAUSE  LALT(KC_T) // Zoom Alt+T: Pause or resume screen share (Note: Will only work when meeting control toolbar has focus)
#define KY_RECLOCAL LALT(KC_R) // Zoom Alt+R: Start/stop local recording
#define KY_RECCLOUD LALT(KC_C) // Zoom Alt+C: Start/stop cloud recording
#define KY_RECPAUSE LALT(KC_P) // Zoom Alt+P: Pause or resume recording
#define KY_CAMERA   LALT(KC_N) // Zoom Alt+N: Switch camera
#define KY_FULLSCR  LALT(KC_F) // Zoom Alt+F: Enter or exit full screen
#define KY_CHAT     LALT(KC_H) // Zoom Alt+H: Display/hide in-meeting chat panel
#define KY_PARTICI  LALT(KC_U) // Zoom Alt+U: Display/hide participants panel
#define KY_INVITE   LALT(KC_I) // Zoom Alt+I: Open invite window
#define KY_RAISEHND LALT(KC_Y) // Zoom Alt+Y: Raise/lower hand
#define KY_ENDMEET  LALT(KC_Q) // Zoom Alt+Q: End meeting


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        MO(_FN),    KY_VIDEO,   KY_MICMUTE, KY_RAISEHND, KC_MUTE
    ),
    [_FN] = LAYOUT(
        _______,    KY_CAMERA,  KY_MUTEALL, KY_ENDMEET,  RESET
    )
};

// Setup hint strings to display on the oled screen
// THese are to help the user know what the buttons do
// Oled screen is 21 characters across
// NOTE:
// Keep your strings exactly this long or the line wrapping will go wrong
//                                           123456789-123456789-12
const char oled_hint_text_base[] PROGMEM =  "^ | Vid | Mic  | Rais";
const char oled_hint_text_base2[] PROGMEM = "^ |  eo | mute | hand";
const char oled_hint_text_fn[] PROGMEM =    "v | Cam | Mute | End ";
const char oled_hint_text_fn2[] PROGMEM =   "v | era | all  | call";

PGM_P const oled_key_hint_text[2] = {
    [_BASE] = oled_hint_text_base,
    [_FN] = oled_hint_text_fn
};
PGM_P const oled_key_hint_text2[2] = {
    [_BASE] = oled_hint_text_base2,
    [_FN] = oled_hint_text_fn2
};


/*
const char* oled_key_press_text[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = {
        "Shift", "Video", "Mute Mic", "Raise Hand", "Volume"
    },
    [_FN] = {
        "Shift", "Switch Camera", "Mute Others", "End Meeting", "RESET"
    }
};
*/

/*
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QMKBEST:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                SEND_STRING("QMK is the best thing ever!");
            } else {
                // when keycode QMKBEST is released
            }
            break;
        case QMKURL:
            if (record->event.pressed) {
                // when keycode QMKURL is pressed
                SEND_STRING("https://qmk.fm/\n");
            } else {
                // when keycode QMKURL is released
            }
            break;
    }
    return true;
}
*/


#ifdef ENCODER_ENABLE
// Useful examples of using encoders can be found at 
// https://docs.splitkb.com/hc/en-us/articles/360010513760-How-can-I-use-a-rotary-encoder-
bool encoder_update_user(uint8_t index, bool clockwise) {
  // index 1 == minion side
  if (index == 0) {
    if (clockwise) {
      tap_code(KC_VOLD);
    } else {
      tap_code(KC_VOLU);
    }
  }
    return true;
}
#endif

#ifdef OLED_ENABLE
// Useful examples of OLED usage can be found at 
// https://docs.splitkb.com/hc/en-us/articles/360010533820-What-can-you-use-an-OLED-display-for-on-a-keyboard-
// https://docs.splitkb.com/hc/en-us/articles/360013811280-How-do-I-convert-an-image-for-use-on-an-OLED-display-

// flip the screen
oled_rotation_t oled_init_user(oled_rotation_t rotation) { 
    return OLED_ROTATION_180; 
}

void oled_task_user(void) {
    oled_write_ln_P(PSTR("Piccolo Zoom control\n"), false);
    // Show button hints
    int layer_index = get_highest_layer(layer_state);
    switch (layer_index) {
        case _BASE:
        case _FN:
            // don't use oled_write_ln_P() because we already spill onto the next line due to string length
            oled_write_P(oled_key_hint_text[layer_index], false);
            oled_write_P(oled_key_hint_text2[layer_index], false);
            break;

        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }

/*
    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
    */
    
}
#endif

// I hate the red LEDs showing on the pro micro, do you?
void matrix_init_kb(void) {

    // // green led on
    // DDRD |= (1<<5);
    // PORTD &= ~(1<<5);

    // // orange led on
    // DDRB |= (1<<0);
    // PORTB &= ~(1<<0);
	
	//turn off LEDs on ProMicro
   DDRD &= ~(1<<5);
   PORTD &= ~(1<<5);

   DDRB &= ~(1<<0);
   PORTB &= ~(1<<0);

	matrix_init_user();
};