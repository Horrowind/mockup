#ifdef WASM
  #define SHADER_VERSION "#version 100\n"
#else
  #define SHADER_VERSION "#version 300 es\n"
#endif

void device_upload_atlas(OpenGLDevice* dev, void *image, int width, int height) {
    gl_GenTextures(1, &dev->font_tex);
    gl_BindTexture(GL_TEXTURE_2D, dev->font_tex);
    gl_TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl_TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl_TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                  GL_RGBA, GL_UNSIGNED_BYTE, image);
}

void device_create(OpenGLDevice *dev, Arena* arena) {
    int status;
    static const char* vertex_shader =
        SHADER_VERSION
#if defined(WASM)
        "attribute vec2 Position;\n"
        "attribute vec2 TexCoord;\n"
        "attribute vec4 Color;\n"
        "varying vec2 Frag_UV;\n"
        "varying vec4 Frag_Color;\n"
#else
        "in vec2 Position;\n"
        "in vec2 TexCoord;\n"
        "in vec4 Color;\n"
        "out vec2 Frag_UV;\n"
        "out vec4 Frag_Color;\n"
#endif
        "uniform mat4 ProjMtx;\n"
        "void main() {\n"
        "   Frag_UV = TexCoord;\n"
        "   Frag_Color = Color;\n"
        "   gl_Position = ProjMtx * vec4(Position.xy, 0, 1);\n"
        "}\n";
    static const char *fragment_shader =
        SHADER_VERSION
        "precision mediump float;\n"
        "uniform sampler2D Texture;\n"
#if defined(WASM)
        "varying vec2 Frag_UV;\n"
        "varying vec4 Frag_Color;\n"
#else
        "in vec2 Frag_UV;\n"
        "in vec4 Frag_Color;\n"
        "out vec4 Out_Color;\n"
#endif
        "void main(){\n"
#if defined(WASM)
        "   gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV);\n"
#else
        "   Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
#endif
        "}\n";

    u8* cmd_memory = arena_alloc_array(arena, MB(16), u8);
    nk_buffer_init_fixed(&dev->cmds, cmd_memory, MB(16));
    //nk_buffer_init_default(&dev->cmds);
    dev->prog = gl_CreateProgram();
    dev->vert_shdr = gl_CreateShader(GL_VERTEX_SHADER);
    dev->frag_shdr = gl_CreateShader(GL_FRAGMENT_SHADER);
    gl_ShaderSource(dev->vert_shdr, 1, &vertex_shader, 0);
    gl_ShaderSource(dev->frag_shdr, 1, &fragment_shader, 0);
    gl_CompileShader(dev->vert_shdr);
    gl_CompileShader(dev->frag_shdr);
#if defined(WASM)
    status = gl_GetShaderParameter(dev->vert_shdr, GL_COMPILE_STATUS);
#else
    gl_GetShaderiv(dev->vert_shdr, GL_COMPILE_STATUS, &status);
#endif
    assert(status);
    gl_AttachShader(dev->prog, dev->vert_shdr);
    gl_AttachShader(dev->prog, dev->frag_shdr);
    gl_LinkProgram(dev->prog);
#if defined(WASM)
    status = gl_GetProgramParameter(dev->prog, GL_LINK_STATUS);
#else
    gl_GetProgramiv(dev->prog, GL_LINK_STATUS, &status);
#endif
    assert(status);
    dev->uniform_tex = gl_GetUniformLocation(dev->prog, "Texture");
    dev->uniform_proj = gl_GetUniformLocation(dev->prog, "ProjMtx");
    dev->attrib_pos = gl_GetAttribLocation(dev->prog, "Position");
    dev->attrib_uv = gl_GetAttribLocation(dev->prog, "TexCoord");
    dev->attrib_col = gl_GetAttribLocation(dev->prog, "Color");

    {
        /* buffer setup */
        int vs = sizeof(Vertex);
        iptr vp = offsetof(Vertex, position);
        iptr vt = offsetof(Vertex, uv);
        iptr vc = offsetof(Vertex, col);

        gl_GenBuffers(1, &dev->vbo);
        gl_GenBuffers(1, &dev->ebo);
        gl_GenVertexArrays(1, &dev->vao);

        gl_BindVertexArray(dev->vao);
        gl_BindBuffer(GL_ARRAY_BUFFER, dev->vbo);
        gl_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

        gl_EnableVertexAttribArray((uint)dev->attrib_pos);
        gl_EnableVertexAttribArray((uint)dev->attrib_uv);
        gl_EnableVertexAttribArray((uint)dev->attrib_col);

        gl_VertexAttribPointer((uint)dev->attrib_pos, 2, GL_FLOAT, false, vs, (void*)vp);
        gl_VertexAttribPointer((uint)dev->attrib_uv, 2, GL_FLOAT, false, vs, (void*)vt);
        gl_VertexAttribPointer((uint)dev->attrib_col, 4, GL_UNSIGNED_BYTE, true, vs, (void*)vc);
    }

    gl_BindTexture(GL_TEXTURE_2D, 0);
    gl_BindBuffer(GL_ARRAY_BUFFER, 0);
    gl_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    gl_BindVertexArray(0);
}

void device_destroy(OpenGLDevice *dev) {
    gl_DetachShader(dev->prog, dev->vert_shdr);
    gl_DetachShader(dev->prog, dev->frag_shdr);
    gl_DeleteShader(dev->vert_shdr);
    gl_DeleteShader(dev->frag_shdr);
    gl_DeleteProgram(dev->prog);
    gl_DeleteTextures(1, &dev->font_tex);
    gl_DeleteBuffers(1, &dev->vbo);
    gl_DeleteBuffers(1, &dev->ebo);
    nk_buffer_free(&dev->cmds);
}

void render(RenderInfo info)
{
    OpenGLDevice *dev = info.ogl;
    struct nk_buffer vbuf, ebuf;
    float ortho[4][4] = {
        {2.0f, 0.0f, 0.0f, 0.0f},
        {0.0f,-2.0f, 0.0f, 0.0f},
        {0.0f, 0.0f,-1.0f, 0.0f},
        {-1.0f,1.0f, 0.0f, 1.0f},
    };
    ortho[0][0] /= (float)info.width;
    ortho[1][1] /= (float)info.height;

    /* setup global state */
    gl_Enable(GL_BLEND);
    gl_BlendEquation(GL_FUNC_ADD);
    gl_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl_Disable(GL_CULL_FACE);
    gl_Disable(GL_DEPTH_TEST);
    gl_Enable(GL_SCISSOR_TEST);
    gl_ActiveTexture(GL_TEXTURE0);

    /* setup program */
    gl_UseProgram(dev->prog);
    gl_Uniform1i(dev->uniform_tex, 0);
    gl_UniformMatrix4fv(dev->uniform_proj, 1, false, &ortho[0][0]);
    gl_Viewport(0,0,(int)info.display_width,(int)info.display_height);
    {
        /* convert from command queue into draw list and draw to screen */
        const struct nk_draw_command* cmd;
        void *vertices, *elements;
        nk_draw_index* offset = NULL;

        /* allocate vertex and element buffer */
        gl_BindVertexArray(dev->vao);
        gl_BindBuffer(GL_ARRAY_BUFFER, dev->vbo);
        gl_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

        gl_BufferData(GL_ARRAY_BUFFER, info.max_vertex_buffer, NULL, GL_STREAM_DRAW);
        gl_BufferData(GL_ELEMENT_ARRAY_BUFFER, info.max_element_buffer, NULL, GL_STREAM_DRAW);

        /* load draw vertices & elements directly into vertex + element buffer */
#if defined(WASM)
        Temp temp = temp_begin(info.arena);
        vertices = arena_alloc_array(info.arena, info.max_vertex_buffer, u8);
        elements = arena_alloc_array(info.arena, info.max_element_buffer, u8);
#else
        vertices = gl_MapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        elements = gl_MapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
#endif
        {
            /* fill convert configuration */
            struct nk_convert_config config;
            static struct nk_draw_vertex_layout_element vertex_layout[] = {
                {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(Vertex, position)},
                {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(Vertex, uv)},
                {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(Vertex, col)},
                {NK_VERTEX_LAYOUT_END}
            };
            config = (struct nk_convert_config) {};
            config.vertex_layout = vertex_layout;
            config.vertex_size = sizeof(Vertex);
            config.vertex_alignment = NK_ALIGNOF(Vertex);
            config.null = dev->null;
            config.circle_segment_count = 22;
            config.curve_segment_count = 22;
            config.arc_segment_count = 22;
            config.global_alpha = 1.0f;
            config.shape_AA = info.AA;
            config.line_AA = info.AA;

            /* setup buffers to load vertices and elements */
            nk_buffer_init_fixed(&vbuf, vertices, (uint)info.max_vertex_buffer);
            nk_buffer_init_fixed(&ebuf, elements, (uint)info.max_element_buffer);
            nk_convert(info.ctx, &dev->cmds, &vbuf, &ebuf, &config);
        }
#if defined(WASM)
        gl_BufferSubData(GL_ARRAY_BUFFER, 0, info.max_vertex_buffer, vertices);
        gl_BufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, info.max_element_buffer, elements);
        temp_end(temp);
#else
        gl_UnmapBuffer(GL_ARRAY_BUFFER);
        gl_UnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
#endif
        /* iterate over and execute each draw command */
        nk_draw_foreach(cmd, info.ctx, &dev->cmds)
        {
            if (!cmd->elem_count) continue;
            gl_BindTexture(GL_TEXTURE_2D, (uint)cmd->texture.id);
            gl_Scissor(
                (int)(cmd->clip_rect.x * info.fb_scale.x),
                (int)((info.height - (int)(cmd->clip_rect.y + cmd->clip_rect.h)) * info.fb_scale.y),
                (int)(cmd->clip_rect.w * info.fb_scale.x),
                (int)(cmd->clip_rect.h * info.fb_scale.y));
            gl_DrawElements(GL_TRIANGLES, (int)cmd->elem_count, GL_UNSIGNED_INT, offset);
            offset += 2 * cmd->elem_count;
        }
        nk_clear(info.ctx);
    }

    /* default OpenGL state */
    gl_UseProgram(0);
    gl_BindBuffer(GL_ARRAY_BUFFER, 0);
    gl_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    gl_BindVertexArray(0);
    gl_Disable(GL_BLEND);
    gl_Disable(GL_SCISSOR_TEST);
}

void* nk_custom_alloc(nk_handle handle, void* old, nk_size size) {
    unused(old);
    Arena* arena = (Arena*)handle.ptr;
    return arena_alloc(arena, size, 16);
}

void nk_custom_free(nk_handle handle, void* old) {
    unused(handle);
    unused(old);
}

NK_API void font_stash_begin(Mockup* mockup, Arena* persistent_arena, Arena* transient_arena) {
    struct nk_allocator* persistent = arena_alloc_type(transient_arena, struct nk_allocator);
    struct nk_allocator* transient  = arena_alloc_type(transient_arena, struct nk_allocator);
    *persistent = (struct nk_allocator) {
        .userdata.ptr = (void*)persistent_arena,
        .alloc = nk_custom_alloc,
        .free  = nk_custom_free
    };
    *transient = (struct nk_allocator) {
        .userdata.ptr = (void*)transient_arena,
        .alloc = nk_custom_alloc,
        .free  = nk_custom_free
    };
    nk_font_atlas_init_custom(&mockup->atlas, persistent, transient);
    nk_font_atlas_begin(&mockup->atlas);
}

NK_API void font_stash_end(Mockup* mockup) {
    void *image; int w, h;
    image = (void*)nk_font_atlas_bake(&mockup->atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    device_upload_atlas(&mockup->ogl, image, w, h);
    nk_font_atlas_end(&mockup->atlas, nk_handle_id((int)mockup->ogl.font_tex), &mockup->ogl.null);
    if (mockup->atlas.default_font)
        nk_style_set_font(&mockup->ctx, &mockup->atlas.default_font->handle);
}
