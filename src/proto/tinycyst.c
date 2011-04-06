/**
 * Copyright (c) 2011 Kevin Nelson <kevin@soundcyst.com>
 *
 * Based on work that is Copyright (c) 2010 William Light <wrl@illest.net>
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdlib.h>
#include <stdint.h>

#include <monome.h>
#include "internal.h"
#include "platform.h"
#include "rotation.h"

#include "tinycyst.h"

/**
 * private
 */

static int monome_write(monome_t *monome, const uint8_t *buf,
						ssize_t bufsize) {
    if( monome_platform_write(monome, buf, bufsize) == bufsize )
		return 0;

	return -1;
}


/**
 * public
 */

static int proto_tinycyst_led_all(monome_t *monome, uint_t status) {
	uint8_t buf = PROTO_TINYCYST_CLEAR | (status & 0x01);
	return monome_write(monome, &buf, sizeof(buf));
}

static int proto_tinycyst_mode(monome_t *monome, monome_mode_t mode) {
	uint8_t buf = PROTO_TINYCYST_MODE | ((mode & PROTO_TINYCYST_MODE_TEST) | (mode & PROTO_TINYCYST_MODE_SHUTDOWN));
	return monome_write(monome, &buf, sizeof(buf));
}


static int proto_tinycyst_led_set(monome_t *monome, uint_t x, uint_t y, uint_t on) {
	uint8_t buf[2];

	ROTATE_COORDS(monome, x, y);

	buf[0] = PROTO_TINYCYST_LED_ON + (!on << 4);
	buf[1] = (x << 4) | y;

	return monome_write(monome, buf, sizeof(buf));
}

/* tinycyst only listens to small row messages, since it's only one row.

iiii.... ....aaaa

i: message id (4)
.: don't care
a: row data (4 bits)
*/
static int proto_tinycyst_led_row(monome_t *monome, uint_t x_off, uint_t y,
                                size_t count, const uint8_t *data) {
	uint8_t buf[2];

	if (y != 0)
	{
		return -1;
	}

	buf[0] = PROTO_TINYCYST_LED_ROW;

	buf[1] = &data & 0x0F;

	return monome_write(monome, buf, sizeof(buf));

}

/* soundcyst 4+2 */
static int proto_tinycyst_led_color(monome_t *monome, uint_t x, uint_t y,
								    uint_t r, uint_t g, uint_t b)
{

	uint8_t buf[5];

	buf[0] = PROTO_TINYCYST_LED_COLOR;
	buf[1] = (x << 4) | y;

	buf[2] = r;
	buf[3] = g;
	buf[4] = b;
	
	return monome_write(monome, buf, sizeof(buf));
}


static int proto_tinycyst_next_event(monome_t *monome, monome_event_t *e) {
	uint8_t buf[2] = {0, 0};

	if( monome_platform_read(monome, buf, sizeof(buf)) < sizeof(buf) )
		return 0;

	switch( buf[0] ) {
	case PROTO_TINYCYST_BUTTON_DOWN:
	case PROTO_TINYCYST_BUTTON_UP:
		e->event_type = (buf[0] == PROTO_TINYCYST_BUTTON_DOWN) ? MONOME_BUTTON_DOWN : MONOME_BUTTON_UP;
		e->grid.x = buf[1] >> 4;
		e->grid.y = buf[1] & 0x0F;

		UNROTATE_COORDS(monome, e->grid.x, e->grid.y);
		return 1;

	case PROTO_TINYCYST_KNOB:
		e->event_type = MONOME_ADC_CHANGE;
		e->adc.number = buf[0] & 0x0F;
		e->adc.value  = buf[1];
		return 1;
	}

	return 0;
}

static int proto_tinycyst_open(monome_t *monome, const char *dev,
							 const char *serial, const monome_devmap_t *m,
							 va_list args) {
	monome->rows   = m->dimensions.rows;
	monome->cols   = m->dimensions.cols;
	monome->serial = serial;

	return monome_platform_open(monome, m, dev);
}

static int proto_tinycyst_close(monome_t *monome) {
	return monome_platform_close(monome);
}

static void proto_tinycyst_free(monome_t *monome) {
	m_free(monome);
}

monome_t *monome_protocol_new() {
	monome_t *monome = m_calloc(1, sizeof(monome_t));

	if( !monome )
		return NULL;

	monome->open       = proto_tinycyst_open;
	monome->close      = proto_tinycyst_close;
	monome->free       = proto_tinycyst_free;

	monome->next_event = proto_tinycyst_next_event;

	monome->mode       = proto_tinycyst_mode;

	monome->led.set    = proto_tinycyst_led_set;
	monome->led.all    = proto_tinycyst_led_all;
	monome->led.map    = NULL;
	monome->led.row    = proto_tinycyst_led_row;
	monome->led.col    = NULL;
	monome->led.intensity = NULL;

	monome->led.color  = proto_tinycyst_led_color;

	return monome;
}
