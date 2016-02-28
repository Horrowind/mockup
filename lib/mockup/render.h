#ifndef MOCKUP_RENDER_H
#define MOCKUP_RENDER_H

void    level_render_line_fg(level_t* l, u8* line, int linenum);
void    level_render_line_bg(level_t* l, u8* line, int linenum);
u8 level_render_pixel(level_t* l, int x, int y);



#endif //MOCKUP_RENDER_H

