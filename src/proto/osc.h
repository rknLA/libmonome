/**
 * Copyright (c) 2010 William Light <wrl@illest.net>
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

#include <lo/lo.h>

#include "monome.h"
#include "internal.h"

typedef struct monome_osc monome_osc_t;

struct monome_osc {
	monome_t parent;

	lo_server server;
	lo_address outgoing;
	char *prefix;

	int have_event;
	monome_event_t *e_ptr;

	char *set_str;
	char *all_str;
	char *map_str;
	char *col_str;
	char *row_str;
	char *intensity_str;
	
	/* soundcyst 4+2 */
	char *color_str;
};
