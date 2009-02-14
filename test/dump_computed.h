#include <libcss/computed.h>
#include <libcss/properties.h>
#include <libcss/types.h>

static size_t dump_css_fixed(css_fixed f, char *ptr, size_t len)
{
#define ABS(x) (uint32_t)((x) < 0 ? -(x) : (x))
	uint32_t uintpart = FIXTOINT(ABS(f));
	/* + 500 to ensure round to nearest (division will truncate) */
	uint32_t fracpart = ((ABS(f) & 0x3ff) * 1000 + 500) / (1 << 10);
#undef ABS
	size_t flen = 0;
	char tmp[20];
	size_t tlen = 0;
	char *buf = ptr;

	if (len == 0)
		return 0;

	if (f < 0) {
		buf[0] = '-';
		buf++;
		len--;
	}

	do {
		tmp[tlen] = "0123456789"[uintpart % 10];
		tlen++;

		uintpart /= 10;
	} while (tlen < 20 && uintpart != 0);

	while (tlen > 0 && len > 0) {
		buf[0] = tmp[--tlen];
		buf++;
		len--;
	}

	if (len > 0) {
		buf[0] = '.';
		buf++;
		len--;
	}

	do {
		tmp[tlen] = "0123456789"[fracpart % 10];
		tlen++;

		fracpart /= 10;
	} while (tlen < 20 && fracpart != 0);

	while (tlen > 0 && len > 0) {
		buf[0] = tmp[--tlen];
		buf++;
		flen++;
		len--;
	}

	while (flen < 3 && len > 0) {
		buf[0] = '0';
		buf++;
		flen++;
		len--;
	}

	if (len > 0)
		buf[0] = '\0';

	return buf - ptr;
}
static size_t dump_css_number(css_fixed val, char *ptr, size_t len)
{
	if (INTTOFIX(FIXTOINT(val)) == val)
		return snprintf(ptr, len, "%d", FIXTOINT(val));
	else
		return dump_css_fixed(val, ptr, len);
}

static size_t dump_css_unit(css_fixed val, css_unit unit, char *ptr, size_t len)
{
	size_t ret = dump_css_number(val, ptr, len);

	switch (unit) {
	case CSS_UNIT_PX:
		ret += snprintf(ptr + ret, len - ret, "px");
		break;
	case CSS_UNIT_EX:
		ret += snprintf(ptr + ret, len - ret, "ex");
		break;
	case CSS_UNIT_EM:
		ret += snprintf(ptr + ret, len - ret, "em");
		break;
	case CSS_UNIT_IN:
		ret += snprintf(ptr + ret, len - ret, "in");
		break;
	case CSS_UNIT_CM:
		ret += snprintf(ptr + ret, len - ret, "cm");
		break;
	case CSS_UNIT_MM:
		ret += snprintf(ptr + ret, len - ret, "mm");
		break;
	case CSS_UNIT_PT:
		ret += snprintf(ptr + ret, len - ret, "pt");
		break;
	case CSS_UNIT_PC:
		ret += snprintf(ptr + ret, len - ret, "pc");
		break;
	case CSS_UNIT_PCT:
		ret += snprintf(ptr + ret, len - ret, "%%");
		break;
	case CSS_UNIT_DEG:
		ret += snprintf(ptr + ret, len - ret, "deg");
		break;
	case CSS_UNIT_GRAD:
		ret += snprintf(ptr + ret, len - ret, "grad");
		break;
	case CSS_UNIT_RAD:
		ret += snprintf(ptr + ret, len - ret, "rad");
		break;
	case CSS_UNIT_MS:
		ret += snprintf(ptr + ret, len - ret, "ms");
		break;
	case CSS_UNIT_S:
		ret += snprintf(ptr + ret, len - ret, "s");
		break;
	case CSS_UNIT_HZ:
		ret += snprintf(ptr + ret, len - ret, "Hz");
		break;
	case CSS_UNIT_KHZ:
		ret += snprintf(ptr + ret, len - ret, "kHz");
		break;
	}

	return ret;
}


static void dump_computed_style(const css_computed_style *style, char *buf,
		size_t *len)
{
	char *ptr = buf;
	size_t wrote = 0;
	uint8_t val;
	css_color color;
	const css_string *url;
	css_fixed len1, len2;
	css_unit unit1, unit2;

	/* background-attachment */
	val = css_computed_background_attachment(style);
	switch (val) {
	case CSS_BACKGROUND_ATTACHMENT_FIXED:
		wrote = snprintf(ptr, *len, "background-attachment: fixed\n");
		break;
	case CSS_BACKGROUND_ATTACHMENT_SCROLL:
		wrote = snprintf(ptr, *len, "background-attachment: scroll\n");
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* background-color */
	val = css_computed_background_color(style, &color);
	switch (val) {
	case CSS_BACKGROUND_COLOR_TRANSPARENT:
		wrote = snprintf(ptr, *len, "background-color: transparent\n");
		break;
	case CSS_BACKGROUND_COLOR_COLOR:
		wrote = snprintf(ptr, *len, "background-color: #%08x\n", color);
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* background-image */
	val = css_computed_background_image(style, &url);
	if (val == CSS_BACKGROUND_IMAGE_IMAGE && url->data != NULL) {
		wrote = snprintf(ptr, *len, "background-image: url('%.*s')\n",
				(int) url->len, url->data);
	} else if (val == CSS_BACKGROUND_IMAGE_NONE) {
		wrote = snprintf(ptr, *len, "background-image: none\n");
	} else {
		wrote = 0;
	}
	ptr += wrote;
	*len -= wrote;

	/* background-position */
	val = css_computed_background_position(style, &len1, &unit1,
			&len2, &unit2);
	if (val == CSS_BACKGROUND_POSITION_SET) {
		wrote = snprintf(ptr, *len, "background-position: ");
		ptr += wrote;
		*len -= wrote;

		wrote = dump_css_unit(len1, unit1, ptr, *len);
		ptr += wrote;
		*len -= wrote;

		wrote = snprintf(ptr, *len, " ");
		ptr += wrote;
		*len -= wrote;

		wrote = dump_css_unit(len2, unit2, ptr, *len);
		ptr += wrote;
		*len -= wrote;

		wrote = snprintf(ptr, *len, "\n");
		ptr += wrote;
		*len -= wrote;
	}

	/* background-repeat */
	val = css_computed_background_repeat(style);
	switch (val) {
	case CSS_BACKGROUND_REPEAT_REPEAT_X:
		wrote = snprintf(ptr, *len, "background-repeat: repeat-x\n");
		break;
	case CSS_BACKGROUND_REPEAT_REPEAT_Y:
		wrote = snprintf(ptr, *len, "background-repeat: repeat-y\n");
		break;
	case CSS_BACKGROUND_REPEAT_REPEAT:
		wrote = snprintf(ptr, *len, "background-repeat: repeat\n");
		break;
	case CSS_BACKGROUND_REPEAT_NO_REPEAT:
		wrote = snprintf(ptr, *len, "background-repeat: no-repeat\n");
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* border-collapse */
	val = css_computed_border_collapse(style);
	switch (val) {
	case CSS_BORDER_COLLAPSE_SEPARATE:
		wrote = snprintf(ptr, *len, "border-collapse: separate\n");
		break;
	case CSS_BORDER_COLLAPSE_COLLAPSE:
		wrote = snprintf(ptr, *len, "border-collapse: collapse\n");
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* border-spacing */
	if (style->uncommon != NULL) {
		val = css_computed_border_spacing(style, &len1, &unit1,
				&len2, &unit2);
		if (val == CSS_BORDER_SPACING_SET) {
			wrote = snprintf(ptr, *len, "border-spacing: ");
			ptr += wrote;
			*len -= wrote;

			wrote = dump_css_unit(len1, unit1, ptr, *len);
			ptr += wrote;
			*len -= wrote;

			wrote = snprintf(ptr, *len, " ");
			ptr += wrote;
			*len -= wrote;

			wrote = dump_css_unit(len2, unit2, ptr, *len);
			ptr += wrote;
			*len -= wrote;

			wrote = snprintf(ptr, *len, "\n");
			ptr += wrote;
			*len -= wrote;
		}
	}

	/* border-top-color */
	val = css_computed_border_top_color(style, &color);
	switch (val) {
	case CSS_BORDER_COLOR_TRANSPARENT:
		wrote = snprintf(ptr, *len, "border-top-color: transparent\n");
		break;
	case CSS_BORDER_COLOR_COLOR:
		wrote = snprintf(ptr, *len, "border-top-color: #%08x\n", color);
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* border-right-color */
	val = css_computed_border_right_color(style, &color);
	switch (val) {
	case CSS_BORDER_COLOR_TRANSPARENT:
		wrote = snprintf(ptr, *len,
				"border-right-color: transparent\n");
		break;
	case CSS_BORDER_COLOR_COLOR:
		wrote = snprintf(ptr, *len,
				"border-right-color: #%08x\n", color);
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* border-bottom-color */
	val = css_computed_border_bottom_color(style, &color);
	switch (val) {
	case CSS_BORDER_COLOR_TRANSPARENT:
		wrote = snprintf(ptr, *len,
				"border-bottom-color: transparent\n");
		break;
	case CSS_BORDER_COLOR_COLOR:
		wrote = snprintf(ptr, *len,
				"border-bottom-color: #%08x\n", color);
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* border-left-color */
	val = css_computed_border_left_color(style, &color);
	switch (val) {
	case CSS_BORDER_COLOR_TRANSPARENT:
		wrote = snprintf(ptr, *len, "border-left-color: transparent\n");
		break;
	case CSS_BORDER_COLOR_COLOR:
		wrote = snprintf(ptr, *len,
				"border-left-color: #%08x\n", color);
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* border-top-style */
	val = css_computed_border_top_style(style);
	switch (val) {
	case CSS_BORDER_STYLE_NONE:
		wrote = snprintf(ptr, *len, "border-top-style: none\n");
		break;
	case CSS_BORDER_STYLE_HIDDEN:
		wrote = snprintf(ptr, *len, "border-top-style: hidden\n");
		break;
	case CSS_BORDER_STYLE_DOTTED:
		wrote = snprintf(ptr, *len, "border-top-style: dotted\n");
		break;
	case CSS_BORDER_STYLE_DASHED:
		wrote = snprintf(ptr, *len, "border-top-style: dashed\n");
		break;
	case CSS_BORDER_STYLE_SOLID:
		wrote = snprintf(ptr, *len, "border-top-style: solid\n");
		break;
	case CSS_BORDER_STYLE_DOUBLE:
		wrote = snprintf(ptr, *len, "border-top-style: double\n");
		break;
	case CSS_BORDER_STYLE_GROOVE:
		wrote = snprintf(ptr, *len, "border-top-style: groove\n");
		break;
	case CSS_BORDER_STYLE_RIDGE:
		wrote = snprintf(ptr, *len, "border-top-style: ridge\n");
		break;
	case CSS_BORDER_STYLE_INSET:
		wrote = snprintf(ptr, *len, "border-top-style: inset\n");
		break;
	case CSS_BORDER_STYLE_OUTSET:
		wrote = snprintf(ptr, *len, "border-top-style: outset\n");
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* border-right-style */
	val = css_computed_border_right_style(style);
	switch (val) {
	case CSS_BORDER_STYLE_NONE:
		wrote = snprintf(ptr, *len, "border-right-style: none\n");
		break;
	case CSS_BORDER_STYLE_HIDDEN:
		wrote = snprintf(ptr, *len, "border-right-style: hidden\n");
		break;
	case CSS_BORDER_STYLE_DOTTED:
		wrote = snprintf(ptr, *len, "border-right-style: dotted\n");
		break;
	case CSS_BORDER_STYLE_DASHED:
		wrote = snprintf(ptr, *len, "border-right-style: dashed\n");
		break;
	case CSS_BORDER_STYLE_SOLID:
		wrote = snprintf(ptr, *len, "border-right-style: solid\n");
		break;
	case CSS_BORDER_STYLE_DOUBLE:
		wrote = snprintf(ptr, *len, "border-right-style: double\n");
		break;
	case CSS_BORDER_STYLE_GROOVE:
		wrote = snprintf(ptr, *len, "border-right-style: groove\n");
		break;
	case CSS_BORDER_STYLE_RIDGE:
		wrote = snprintf(ptr, *len, "border-right-style: ridge\n");
		break;
	case CSS_BORDER_STYLE_INSET:
		wrote = snprintf(ptr, *len, "border-right-style: inset\n");
		break;
	case CSS_BORDER_STYLE_OUTSET:
		wrote = snprintf(ptr, *len, "border-right-style: outset\n");
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* border-bottom-style */
	val = css_computed_border_bottom_style(style);
	switch (val) {
	case CSS_BORDER_STYLE_NONE:
		wrote = snprintf(ptr, *len, "border-bottom-style: none\n");
		break;
	case CSS_BORDER_STYLE_HIDDEN:
		wrote = snprintf(ptr, *len, "border-bottom-style: hidden\n");
		break;
	case CSS_BORDER_STYLE_DOTTED:
		wrote = snprintf(ptr, *len, "border-bottom-style: dotted\n");
		break;
	case CSS_BORDER_STYLE_DASHED:
		wrote = snprintf(ptr, *len, "border-bottom-style: dashed\n");
		break;
	case CSS_BORDER_STYLE_SOLID:
		wrote = snprintf(ptr, *len, "border-bottom-style: solid\n");
		break;
	case CSS_BORDER_STYLE_DOUBLE:
		wrote = snprintf(ptr, *len, "border-bottom-style: double\n");
		break;
	case CSS_BORDER_STYLE_GROOVE:
		wrote = snprintf(ptr, *len, "border-bottom-style: groove\n");
		break;
	case CSS_BORDER_STYLE_RIDGE:
		wrote = snprintf(ptr, *len, "border-bottom-style: ridge\n");
		break;
	case CSS_BORDER_STYLE_INSET:
		wrote = snprintf(ptr, *len, "border-bottom-style: inset\n");
		break;
	case CSS_BORDER_STYLE_OUTSET:
		wrote = snprintf(ptr, *len, "border-bottom-style: outset\n");
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* border-left-style */
	val = css_computed_border_left_style(style);
	switch (val) {
	case CSS_BORDER_STYLE_NONE:
		wrote = snprintf(ptr, *len, "border-left-style: none\n");
		break;
	case CSS_BORDER_STYLE_HIDDEN:
		wrote = snprintf(ptr, *len, "border-left-style: hidden\n");
		break;
	case CSS_BORDER_STYLE_DOTTED:
		wrote = snprintf(ptr, *len, "border-left-style: dotted\n");
		break;
	case CSS_BORDER_STYLE_DASHED:
		wrote = snprintf(ptr, *len, "border-left-style: dashed\n");
		break;
	case CSS_BORDER_STYLE_SOLID:
		wrote = snprintf(ptr, *len, "border-left-style: solid\n");
		break;
	case CSS_BORDER_STYLE_DOUBLE:
		wrote = snprintf(ptr, *len, "border-left-style: double\n");
		break;
	case CSS_BORDER_STYLE_GROOVE:
		wrote = snprintf(ptr, *len, "border-left-style: groove\n");
		break;
	case CSS_BORDER_STYLE_RIDGE:
		wrote = snprintf(ptr, *len, "border-left-style: ridge\n");
		break;
	case CSS_BORDER_STYLE_INSET:
		wrote = snprintf(ptr, *len, "border-left-style: inset\n");
		break;
	case CSS_BORDER_STYLE_OUTSET:
		wrote = snprintf(ptr, *len, "border-left-style: outset\n");
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* border-top-width */
	val = css_computed_border_top_width(style, &len1, &unit1);
	switch (val) {
	case CSS_BORDER_WIDTH_THIN:
		wrote = snprintf(ptr, *len, "border-top-width: thin\n");
		break;
	case CSS_BORDER_WIDTH_MEDIUM:
		wrote = snprintf(ptr, *len, "border-top-width: medium\n");
		break;
	case CSS_BORDER_WIDTH_THICK:
		wrote = snprintf(ptr, *len, "border-top-width: thick\n");
		break;
	case CSS_BORDER_WIDTH_WIDTH:
		wrote = snprintf(ptr, *len, "border-top-width: \n");

		wrote += dump_css_unit(len1, unit1, ptr, *len);
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* border-right-width */
	val = css_computed_border_right_width(style, &len1, &unit1);
	switch (val) {
	case CSS_BORDER_WIDTH_THIN:
		wrote = snprintf(ptr, *len, "border-right-width: thin\n");
		break;
	case CSS_BORDER_WIDTH_MEDIUM:
		wrote = snprintf(ptr, *len, "border-right-width: medium\n");
		break;
	case CSS_BORDER_WIDTH_THICK:
		wrote = snprintf(ptr, *len, "border-right-width: thick\n");
		break;
	case CSS_BORDER_WIDTH_WIDTH:
		wrote = snprintf(ptr, *len, "border-right-width: \n");

		wrote += dump_css_unit(len1, unit1, ptr, *len);
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* border-bottom-width */
	val = css_computed_border_bottom_width(style, &len1, &unit1);
	switch (val) {
	case CSS_BORDER_WIDTH_THIN:
		wrote = snprintf(ptr, *len, "border-bottom-width: thin\n");
		break;
	case CSS_BORDER_WIDTH_MEDIUM:
		wrote = snprintf(ptr, *len, "border-bottom-width: medium\n");
		break;
	case CSS_BORDER_WIDTH_THICK:
		wrote = snprintf(ptr, *len, "border-bottom-width: thick\n");
		break;
	case CSS_BORDER_WIDTH_WIDTH:
		wrote = snprintf(ptr, *len, "border-bottom-width: \n");

		wrote += dump_css_unit(len1, unit1, ptr, *len);
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* border-left-width */
	val = css_computed_border_left_width(style, &len1, &unit1);
	switch (val) {
	case CSS_BORDER_WIDTH_THIN:
		wrote = snprintf(ptr, *len, "border-left-width: thin\n");
		break;
	case CSS_BORDER_WIDTH_MEDIUM:
		wrote = snprintf(ptr, *len, "border-left-width: medium\n");
		break;
	case CSS_BORDER_WIDTH_THICK:
		wrote = snprintf(ptr, *len, "border-left-width: thick\n");
		break;
	case CSS_BORDER_WIDTH_WIDTH:
		wrote = snprintf(ptr, *len, "border-left-width: \n");

		wrote += dump_css_unit(len1, unit1, ptr, *len);
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* bottom */
	val = css_computed_bottom(style, &len1, &unit1);
	switch (val) {
	case CSS_BOTTOM_AUTO:
		wrote = snprintf(ptr, *len, "bottom: auto\n");
		break;
	case CSS_BOTTOM_SET:
		wrote = snprintf(ptr, *len, "bottom: \n");

		wrote += dump_css_unit(len1, unit1, ptr, *len);
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* caption-side */
	val = css_computed_caption_side(style);
	switch (val) {
	case CSS_CAPTION_SIDE_TOP:
		wrote = snprintf(ptr, *len, "caption_side: top\n");
		break;
	case CSS_CAPTION_SIDE_BOTTOM:
		wrote = snprintf(ptr, *len, "caption_side: bottom\n");
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* clear */
	val = css_computed_clear(style);
	switch (val) {
	case CSS_CLEAR_NONE:
		wrote = snprintf(ptr, *len, "clear: none\n");
		break;
	case CSS_CLEAR_LEFT:
		wrote = snprintf(ptr, *len, "clear: left\n");
		break;
	case CSS_CLEAR_RIGHT:
		wrote = snprintf(ptr, *len, "clear: right\n");
		break;
	case CSS_CLEAR_BOTH:
		wrote = snprintf(ptr, *len, "clear: both\n");
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;

	/* clip */
	if (style->uncommon != NULL) {
#if 0
		val = css_computed_clip(style);
		switch (val) {
		case CSS_CLIP_AUTO:
			wrote = snprintf(ptr, *len, "clip: auto\n");
			break;
		case CSS_CLIP_RECT:
			wrote = snprintf(ptr, *len, "clip: rect\n");
			break;
		default:
			wrote = 0;
			break;
		}
		ptr += wrote;
		*len -= wrote;
#endif
	}

	/* color */
	val = css_computed_color(style, &color);
	if (val == CSS_COLOR_COLOR) {
		wrote = snprintf(ptr, *len, "color: #%08x\n", color);
		ptr += wrote;
		*len -= wrote;
	}

	/* display */
	val = css_computed_display(style);
	switch (val) {
	case CSS_DISPLAY_INLINE:
		wrote = snprintf(ptr, *len, "display: inline\n");
		break;
	case CSS_DISPLAY_BLOCK:
		wrote = snprintf(ptr, *len, "display: block\n");
		break;
	case CSS_DISPLAY_LIST_ITEM:
		wrote = snprintf(ptr, *len, "display: list-item\n");
		break;
	case CSS_DISPLAY_RUN_IN:
		wrote = snprintf(ptr, *len, "display: run-in\n");
		break;
	case CSS_DISPLAY_INLINE_BLOCK:
		wrote = snprintf(ptr, *len, "display: inline-block\n");
		break;
	case CSS_DISPLAY_TABLE:
		wrote = snprintf(ptr, *len, "display: table\n");
		break;
	case CSS_DISPLAY_INLINE_TABLE:
		wrote = snprintf(ptr, *len, "display: inline-table\n");
		break;
	case CSS_DISPLAY_TABLE_ROW_GROUP:
		wrote = snprintf(ptr, *len, "display: table-row-group\n");
		break;
	case CSS_DISPLAY_TABLE_HEADER_GROUP:
		wrote = snprintf(ptr, *len, "display: table-header-group\n");
		break;
	case CSS_DISPLAY_TABLE_FOOTER_GROUP:
		wrote = snprintf(ptr, *len, "display: table-footer-group\n");
		break;
	case CSS_DISPLAY_TABLE_ROW:
		wrote = snprintf(ptr, *len, "display: table-row\n");
		break;
	case CSS_DISPLAY_TABLE_COLUMN_GROUP:
		wrote = snprintf(ptr, *len, "display: table-column-group\n");
		break;
	case CSS_DISPLAY_TABLE_COLUMN:
		wrote = snprintf(ptr, *len, "display: table-column\n");
		break;
	case CSS_DISPLAY_TABLE_CELL:
		wrote = snprintf(ptr, *len, "display: table-cell\n");
		break;
	case CSS_DISPLAY_TABLE_CAPTION:
		wrote = snprintf(ptr, *len, "display: table-caption\n");
		break;
	case CSS_DISPLAY_NONE:
		wrote = snprintf(ptr, *len, "display: none\n");
		break;
	default:
		wrote = 0;
		break;
	}
	ptr += wrote;
	*len -= wrote;
}

