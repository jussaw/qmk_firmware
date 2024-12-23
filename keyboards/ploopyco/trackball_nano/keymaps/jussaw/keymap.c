/* Copyright 2021 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2019 Hiroyuki Okada
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

/* Invert Ploopy Dragscroll*/
#define PLOOPY_DRAGSCROLL_INVERT

// Dummy
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {{{ KC_NO }}};

void suspend_power_down_user(void) {
    // Switch off sensor + LED making trackball unable to wake host
    adns5050_power_down();
}

void suspend_wakeup_init_user(void) {
    adns5050_init();
}

// Add host state drag scroll and dpi cycling
static bool num_lock_state = false;
static bool scroll_lock_state = false;

void keyboard_post_init_user(void) {
    num_lock_state  = host_keyboard_led_state().num_lock;
    scroll_lock_state  = host_keyboard_led_state().scroll_lock;
}

bool led_update_user(led_t led_state) {
    // when scroll lock is pressed, toggle drag scroll state
    if ( scroll_lock_state != led_state.scroll_lock ) {
        toggle_drag_scroll();
        scroll_lock_state = led_state.scroll_lock;
    }

    // when num lock is pressed, cycle dpi
    if ( num_lock_state != led_state.num_lock ) {
        cycle_dpi();
        num_lock_state = led_state.num_lock;
    }

    // when all three are enabled, go to bootloader
    if ( led_state.num_lock && led_state.caps_lock && led_state.scroll_lock ) {
        reset_keyboard();
    }

    return true;
}
