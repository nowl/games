#ifndef RENDER_H__
#define RENDER_H__

void render_callback_add(RenderCallback callback_func, unsigned int layer);
void render_callback_remove(RenderCallback callback_func, unsigned int layer);

void render(double interpolation);

#endif  /* RENDER_H__ */
