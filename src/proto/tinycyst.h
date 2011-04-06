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

#include "monome.h"
#include "internal.h"

typedef enum {
	/* input (from device) */

	PROTO_TINYCYST_BUTTON_DOWN         = 0x00,
	PROTO_TINYCYST_BUTTON_UP           = 0x10,
	PROTO_TINYCYST_KNOB                = 0xE0,

	/* output (to device) */

	PROTO_TINYCYST_LED_ON              = 0x20,
	PROTO_TINYCYST_LED_OFF             = 0x30,
	PROTO_TINYCYST_LED_ROW             = 0x40,
	PROTO_TINYCYST_CLEAR               = 0x90,
	PROTO_TINYCYST_MODE                = 0xB0,
	
	PROTO_TINYCYST_COLOR               = 0xF0,
} proto_tinycyst_message_t;

/* modes (argument to the PROTO_TINYCYST_MODE output command) */

typedef enum {
	PROTO_TINYCYST_MODE_NORMAL         = 0x00,
	PROTO_TINYCYST_MODE_TEST           = 0x01,
	PROTO_TINYCYST_MODE_SHUTDOWN       = 0x02
} proto_tinycyst_mode_t;
