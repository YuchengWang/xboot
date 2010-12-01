#ifndef __TUI_WIDGET_H__
#define __TUI_WIDGET_H__

#include <configs.h>
#include <default.h>
#include <rect.h>
#include <tui/tui.h>


x_bool tui_widget_cell_hline(struct tui_widget * widget, x_u32 cp, enum tcolor fg, enum tcolor bg, x_s32 x0, x_s32 y0, x_s32 x);
x_bool tui_widget_cell_vline(struct tui_widget * widget, x_u32 cp, enum tcolor fg, enum tcolor bg, x_s32 x0, x_s32 y0, x_s32 y);
x_bool tui_widget_cell_clear(struct tui_widget * widget, x_u32 cp, enum tcolor fg, enum tcolor bg, x_s32 x, x_s32 y, x_s32 w, x_s32 h);

#endif /* __TUI_WIDGET_H__ */
