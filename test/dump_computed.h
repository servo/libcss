#include <libcss/computed.h>
#include <libcss/properties.h>

static void dump_computed_style(const css_computed_style *style, char *buf, 
		size_t *len)
{
	char *ptr = buf;
	size_t wrote = 0;
	uint8_t val;
	css_color color;

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

	val = css_computed_color(style, &color);
	if (val == CSS_COLOR_COLOR) {
		wrote = snprintf(ptr, *len, "color: #%08x\n", color);
		ptr += wrote;
		*len -= wrote;
	}

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

