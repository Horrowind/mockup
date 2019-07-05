#define BASE_GUI
#include "base/base.h"

#include <string.h>

static  char logbuf[64000];
static   int logbuf_updated = 0;

#if !defined(WASM)
static float bg[3] = { 90, 95, 100 };
#endif

static void write_log(const char *text) {
#if !defined(WASM)
    if (logbuf[0]) { strcat(logbuf, "\n"); }
    strcat(logbuf, text);
    logbuf_updated = 1;
#else
    unused(text);
#endif
}

static void test_window(GuiContext *ctx) {
    static GuiContainer window;

    /* init window manually so we can set its position and size */
    if (!window.inited) {
        mu_init_window(ctx, &window, 0);
        window.rect = recti(40, 40, 300, 450);
    }

    /* limit window to minimum size */
    window.rect.w = max(window.rect.w, 240);
    window.rect.h = max(window.rect.h, 300);


    /* do window */
    if (mu_begin_window(ctx, &window, "Demo Window")) {

        /* window info */
        static int show_info = 0;
        if (mu_header(ctx, &show_info, "Window Info")) {
            char buf[64];
            mu_layout_row(ctx, 2, (int[]) { 54, -1 }, 0);
            mu_label(ctx,"Position:");
            c_string_format_unsafe(buf, "%d, %d", window.rect.x, window.rect.y); mu_label(ctx, buf);
            mu_label(ctx, "Size:");
            c_string_format_unsafe(buf, "%d, %d", window.rect.w, window.rect.h); mu_label(ctx, buf);
        }

        /* labels + buttons */
        static int show_buttons = 1;
        if (mu_header(ctx, &show_buttons, "Test Buttons")) {
            mu_layout_row(ctx, 3, (int[]) { 86, -110, -1 }, 0);
            mu_label(ctx, "Test buttons 1:");
            if (mu_button(ctx, "Button 1")) { write_log("Pressed button 1"); }
            if (mu_button(ctx, "Button 2")) { write_log("Pressed button 2"); }
            mu_label(ctx, "Test buttons 2:");
            if (mu_button(ctx, "Button 3")) { write_log("Pressed button 3"); }
            if (mu_button(ctx, "Button 4")) { write_log("Pressed button 4"); }
        }

        /* tree */
        static int show_tree = 1;
        if (mu_header(ctx, &show_tree, "Tree and Text")) {
            mu_layout_row(ctx, 2, (int[]) { 140, -1 }, 0);
            mu_layout_begin_column(ctx);
            static int states[8];
            if (mu_begin_treenode(ctx, &states[0], "Test 1")) {
                if (mu_begin_treenode(ctx, &states[1], "Test 1a")) {
                    mu_label(ctx, "Hello");
                    mu_label(ctx, "world");
                    mu_end_treenode(ctx);
                }
                if (mu_begin_treenode(ctx, &states[2], "Test 1b")) {
                    if (mu_button(ctx, "Button 1")) { write_log("Pressed button 1"); }
                    if (mu_button(ctx, "Button 2")) { write_log("Pressed button 2"); }
                    mu_end_treenode(ctx);
                }
                mu_end_treenode(ctx);
            }
            if (mu_begin_treenode(ctx, &states[3], "Test 2")) {
                mu_layout_row(ctx, 2, (int[]) { 54, 54 }, 0);
                if (mu_button(ctx, "Button 3")) { write_log("Pressed button 3"); }
                if (mu_button(ctx, "Button 4")) { write_log("Pressed button 4"); }
                if (mu_button(ctx, "Button 5")) { write_log("Pressed button 5"); }
                if (mu_button(ctx, "Button 6")) { write_log("Pressed button 6"); }
                mu_end_treenode(ctx);
            }
            if (mu_begin_treenode(ctx, &states[4], "Test 3")) {
                static int checks[3] = { 1, 0, 1 };
                mu_checkbox(ctx, &checks[0], "Checkbox 1");
                mu_checkbox(ctx, &checks[1], "Checkbox 2");
                mu_checkbox(ctx, &checks[2], "Checkbox 3");
                mu_end_treenode(ctx);
            }
            mu_layout_end_column(ctx);

            mu_layout_begin_column(ctx);
            mu_layout_row(ctx, 1, (int[]) { -1 }, 0);
            mu_text(ctx, "Lorem ipsum dolor sit amet, consectetur adipiscing "
                    "elit. Maecenas lacinia, sem eu lacinia molestie, mi risus faucibus "
                    "ipsum, eu varius magna felis a nulla.");
            mu_layout_end_column(ctx);
        }

#if !defined(WASM)
        /* background color sliders */
        static int show_sliders = 1;
        if (mu_header(ctx, &show_sliders, "Background Color")) {
            mu_layout_row(ctx, 2, (int[]) { -78, -1 }, 76);
            /* sliders */
            mu_layout_begin_column(ctx);
            mu_layout_row(ctx, 2, (int[]) { 46, -1 }, 0);
            mu_label(ctx, "Red:");   mu_slider(ctx, &bg[0], 0, 255);
            mu_label(ctx, "Green:"); mu_slider(ctx, &bg[1], 0, 255);
            mu_label(ctx, "Blue:");  mu_slider(ctx, &bg[2], 0, 255);
            mu_layout_end_column(ctx);
            /* color preview */
            RectI r = mu_layout_next(ctx);
            mu_draw_rect(ctx, r, gui_color(bg[0], bg[1], bg[2], 255));
            mu_draw_box(ctx, r, ctx->style->colors[GUI_COLOR_BORDER]);
            char buf[32];
            c_string_format_unsafe(buf, "#%02X%02X%02X", (int) bg[0], (int) bg[1], (int) bg[2]);
            mu_draw_control_text(ctx, buf, r, GUI_COLOR_TEXT, GUI_OPT_ALIGNCENTER);
        }
#endif

        mu_end_window(ctx);
    }
}

static void log_window(GuiContext *ctx) {
    static GuiContainer window;

    /* init window manually so we can set its position and size */
    if (!window.inited) {
        mu_init_window(ctx, &window, 0);
        window.rect = recti(350, 40, 300, 200);
    }

    if (mu_begin_window(ctx, &window, "Log Window")) {

        /* output text panel */
        static GuiContainer panel;
        mu_layout_row(ctx, 1, (int[]) { -1 }, -28);
        mu_begin_panel(ctx, &panel);
        mu_layout_row(ctx, 1, (int[]) { -1 }, -1);
        mu_text(ctx, logbuf);
        mu_end_panel(ctx);
        if (logbuf_updated) {
            panel.scroll.y = panel.content_size.y;
            logbuf_updated = 0;
        }

        /* input textbox + submit button */
        static char buf[128];
        int submitted = 0;
        mu_layout_row(ctx, 2, (int[]) { -70, -1 }, 0);
#if !defined(WASM)
        if (mu_textbox(ctx, buf, sizeof(buf)) & GUI_RES_SUBMIT) {
            mu_set_focus(ctx, ctx->last_id);
            submitted = 1;
        }
#endif
        if (mu_button(ctx, "Submit")) { submitted = 1; }
        if (submitted) {
            write_log(buf);
            buf[0] = '\0';
        }

        mu_end_window(ctx);
    }
}

#if !defined(WASM)
static int uint8_slider(GuiContext *ctx, unsigned char *value, int low, int high) {
    static float tmp;
    mu_push_id(ctx, &value, sizeof(value));
    tmp = *value;
    int res = mu_slider_ex(ctx, &tmp, low, high, 0, "%.0f", GUI_OPT_ALIGNCENTER);
    *value = tmp;
    mu_pop_id(ctx);
    return res;
}

static void style_window(GuiContext *ctx) {
    static GuiContainer window;

    /* init window manually so we can set its position and size */
    if (!window.inited) {
        mu_init_window(ctx, &window, 0);
        window.rect = recti(350, 250, 300, 240);
    }

    static struct { const char *label; int idx; } colors[] = {
        { "text:",         GUI_COLOR_TEXT        },
        { "border:",       GUI_COLOR_BORDER      },
        { "windowbg:",     GUI_COLOR_WINDOWBG    },
        { "titlebg:",      GUI_COLOR_TITLEBG     },
        { "titletext:",    GUI_COLOR_TITLETEXT   },
        { "panelbg:",      GUI_COLOR_PANELBG     },
        { "button:",       GUI_COLOR_BUTTON      },
        { "buttonhover:",  GUI_COLOR_BUTTONHOVER },
        { "buttonfocus:",  GUI_COLOR_BUTTONFOCUS },
        { "base:",         GUI_COLOR_BASE        },
        { "basehover:",    GUI_COLOR_BASEHOVER   },
        { "basefocus:",    GUI_COLOR_BASEFOCUS   },
        { "scrollbase:",   GUI_COLOR_SCROLLBASE  },
        { "scrollthumb:",  GUI_COLOR_SCROLLTHUMB },
        { NULL }
    };

    if (mu_begin_window(ctx, &window, "Style Editor")) {
        int sw = mu_get_container(ctx)->body.w * 0.14;
        mu_layout_row(ctx, 6, (int[]) { 80, sw, sw, sw, sw, -1 }, 0);
        for (int i = 0; colors[i].label; i++) {
            mu_label(ctx, colors[i].label);
            uint8_slider(ctx, &ctx->style->colors[i].r, 0, 255);
            uint8_slider(ctx, &ctx->style->colors[i].g, 0, 255);
            uint8_slider(ctx, &ctx->style->colors[i].b, 0, 255);
            uint8_slider(ctx, &ctx->style->colors[i].a, 0, 255);
            mu_draw_rect(ctx, mu_layout_next(ctx), ctx->style->colors[i]);
        }
        mu_end_window(ctx);
    }
}
#endif

static void process_frame(GuiContext *ctx) {
    test_window(ctx);
    log_window(ctx);
#if !defined(WASM)
    style_window(ctx);
#endif
}

GuiContext global_context = { };
Arena      global_arena;

int init(int argc, char** argv) {
    unused(argc);
    unused(argv);

    GuiContext* context = &global_context;
    Arena* arena = &global_arena;
    *arena = arena_create(MB(10));
    gui_init(context, arena);
    gui_set_window_name(context, L("GuiTest"));
    return 1;
}

int update() {
    GuiContext* context = &global_context;
    gui_begin_frame(context);
    process_frame(context);
    gui_end_frame(context);
    return gui_should_continue(context);
}

int deinit() {
    GuiContext* context = &global_context;
    gui_deinit(context);
    return 0;
}