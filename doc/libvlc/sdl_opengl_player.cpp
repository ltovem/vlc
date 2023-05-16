//g++ sdl_opengl_player.cpp $(pkg-config --cflags --libs libvlc sdl2 gl)

/* SPDX-License-Identifier: WTFPL  */
/* Written by Pierre Lamot */

#include <cstdio>
#include <cstdlib>

#include <exception>
#include <mutex>
#include <memory>
#include <vector>

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <vlc/vlc.h>

/*
 * This program show how to use libvlc_video_set_output_callbacks API.
 *
 * The main idea is to set up libvlc to render into FBO, and to use a
 * triple buffer mechanism to share textures between VLC and the rendering
 * thread of our application
 */

// Shader sources
const GLchar* vertexSource =
    "attribute vec4 a_position;    \n"
    "attribute vec2 a_uv;          \n"
    "varying vec2 v_TexCoordinate; \n"
    "void main()                   \n"
    "{                             \n"
    "    v_TexCoordinate = a_uv;   \n"
    "    gl_Position = vec4(a_position.xyz, 1.0);  \n"
    "}                             \n";

const GLchar* fragmentSource =
    "uniform sampler2D u_videotex; \n"
    "varying vec2 v_TexCoordinate; \n"
    "void main()                   \n"
    "{                             \n"
    "    gl_FragColor = texture2D(u_videotex, v_TexCoordinate); \n"
    "}                             \n";

class VLCTexture
{
public:
    VLCTexture(unsigned width, unsigned height)
    {
        static unsigned textCount = 0;

        texId = ++textCount;
        glGenTextures(1, &m_tex);
        glGenFramebuffers(1, &m_fbo);

        glBindTexture(GL_TEXTURE_2D, m_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex, 0);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (status != GL_FRAMEBUFFER_COMPLETE) {
            printf("invalid FBO\n");
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    ~VLCTexture()
    {
        glDeleteTextures(1, &m_tex);
        glDeleteFramebuffers(1, &m_fbo);
    }

    void bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    }

    GLuint texture()
    {
        return m_tex;
    }

    unsigned texId = 0;
private:
    GLuint m_tex = 0;
    GLuint m_fbo = 0;
};

class VLCVideo
{
public:
    VLCVideo(SDL_Window *window):
        m_win(window)
    {
        const char *args[] = {
            "--verbose=4"
        };
        m_vlc = libvlc_new(sizeof(args) / sizeof(*args), args);
        m_win = SDL_CreateWindow("offscreen",
                                 0, 0, 1, 1,
                                 SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
        m_ctx = SDL_GL_CreateContext(m_win);
    }

    ~VLCVideo()
    {
        stop();
        if (m_vlc)
            libvlc_release(m_vlc);
        if (m_ctx)
            SDL_GL_DeleteContext(m_ctx);
        if (m_win)
            SDL_DestroyWindow(m_win);
    }

    bool playMedia(const char* url)
    {
        m_media = libvlc_media_new_location(url);
        if (m_media == NULL) {
            fprintf(stderr, "unable to create media %s", url);
            return false;
        }
        m_mp = libvlc_media_player_new_from_media (m_vlc, m_media);
        if (m_mp == NULL) {
            fprintf(stderr, "unable to create media player");
            libvlc_media_release(m_media);
            return false;
        }
        // Define the opengl rendering callbacks
        libvlc_video_set_output_callbacks(m_mp, libvlc_video_engine_opengl,
            setup, cleanup, set_window_callbacks, resize, swap,
            make_current, get_proc_address, nullptr, nullptr,
            this);

        // Play the video
        libvlc_media_player_play (m_mp);

        return true;
    }

    void stop()
    {
        if (m_mp) {
            libvlc_media_player_release(m_mp);
            m_mp = nullptr;
        }
        if (m_media) {
            libvlc_media_release(m_media);
            m_media = nullptr;
        }
    }

    /// return the texture to be displayed
    std::shared_ptr<VLCTexture> getVideoFrame(bool* out_updated)
    {
        std::lock_guard<std::mutex> lock(m_text_lock);
        if (out_updated)
            *out_updated = m_updated;
        if (m_updated) {
            std::swap(m_tex_swap, m_tex_display);
            m_updated = false;
        }
        return m_tex_display;
    }

    /// this callback will create the surfaces and FBO used by VLC to perform its rendering
    static bool resize(void* data, const libvlc_video_render_cfg_t *cfg,
                       libvlc_video_output_cfg_t *render_cfg)
    {
        VLCVideo* that = static_cast<VLCVideo*>(data);

        {
            std::lock_guard<std::mutex> lock(that->m_text_lock);
            that->m_tex_render = std::make_shared<VLCTexture>(cfg->width, cfg->height);
            that->m_tex_swap = std::make_shared<VLCTexture>(cfg->width, cfg->height);
            that->m_tex_display = std::make_shared<VLCTexture>(cfg->width, cfg->height);
            that->m_updated = false;

            that->m_tex_render->bind();
        }

        render_cfg->opengl_format = GL_RGBA;
        render_cfg->full_range = true;
        render_cfg->colorspace = libvlc_video_colorspace_BT709;
        render_cfg->primaries  = libvlc_video_primaries_BT709;
        render_cfg->transfer   = libvlc_video_transfer_func_SRGB;
        render_cfg->orientation = libvlc_video_orient_top_left;

        return true;
    }

    // This callback is called during initialisation.
    static bool setup(void** data, const libvlc_video_setup_device_cfg_t *cfg,
                      libvlc_video_setup_device_info_t *out)
    {
        VLCVideo* that = static_cast<VLCVideo*>(*data);
        that->m_updated = false;
        return true;
    }


    // This callback is called to release the texture and FBO created in resize
    static void cleanup(void* data)
    {
        VLCVideo* that = static_cast<VLCVideo*>(data);
        that->m_tex_render.reset();
        that->m_tex_swap.reset();
        that->m_tex_display.reset();
    }

    //This callback is called after VLC performs drawing calls
    static void swap(void* data)
    {
        VLCVideo* that = static_cast<VLCVideo*>(data);
        std::lock_guard<std::mutex> lock(that->m_text_lock);
        //ensure we finish drawing before sharing the texture
        glFinish();
        that->m_updated = true;
        std::swap(that->m_tex_swap, that->m_tex_render);
        that->m_tex_render->bind();
    }

    // This callback is called to set the OpenGL context
    static bool make_current(void* data, bool current)
    {
        VLCVideo* that = static_cast<VLCVideo*>(data);
        int ret;
        if (current)
            ret = SDL_GL_MakeCurrent(that->m_win, that->m_ctx);
        else
            ret = SDL_GL_MakeCurrent(that->m_win, 0);
        return ret == 0;
    }

    // This callback is called by VLC to get OpenGL functions.
    static void* get_proc_address(void* /*data*/, const char* current)
    {
        if (strncmp(current, "egl", 3) == 0)
            return nullptr;
        return SDL_GL_GetProcAddress(current);
    }


    // This callback is called by VLC to provide functions to report window size and mouse events
    static void set_window_callbacks(void* data,
        libvlc_video_output_resize_cb report_size_change,
        libvlc_video_output_mouse_move_cb report_mouse_move,
        libvlc_video_output_mouse_press_cb report_mouse_press,
        libvlc_video_output_mouse_release_cb report_mouse_release,
        libvlc_video_output_mouse_double_click_cb report_mouse_dblclick,
        void* reportOpaque)
    {
        VLCVideo* that = static_cast<VLCVideo*>(data);
        std::lock_guard<std::mutex> lock(that->m_window_lock);
        that->m_reportOpaque = reportOpaque;
        that->m_report_size_change = report_size_change;
        that->m_report_mouse_move = report_mouse_move;
        that->m_report_mouse_press = report_mouse_press;
        that->m_report_mouse_release = report_mouse_release;
        that->m_report_mouse_dblclick = report_mouse_dblclick;

        //report initial size
        if (that->m_report_size_change)
            that->m_report_size_change(that->m_reportOpaque, that->m_width, that->m_height);
    }

    void onResize(int width, int height)
    {
        std::lock_guard<std::mutex> lock(m_window_lock);
        m_width = width;
        m_height = height;
        if (!m_report_size_change)
            return;
        m_report_size_change(m_reportOpaque, width, height);
    }

    void onMouseMove(int x, int y)
    {
        std::lock_guard<std::mutex> lock(m_window_lock);
        if (!m_report_mouse_move)
            return;
        m_report_mouse_move(m_reportOpaque, x, y);
    }

    void onMousePress(libvlc_video_output_mouse_button_t button)
    {
        std::lock_guard<std::mutex> lock(m_window_lock);
        if (!m_report_mouse_press)
            return;
        m_report_mouse_press(m_reportOpaque, button);
    }

    void onMouseRelease(libvlc_video_output_mouse_button_t button)
    {
        std::lock_guard<std::mutex> lock(m_window_lock);
        if (!m_report_mouse_press)
            return;
        m_report_mouse_release(m_reportOpaque, button);
    }

    void onMouseDoubleClick(libvlc_video_output_mouse_button_t button)
    {
        std::lock_guard<std::mutex> lock(m_window_lock);
        if (!m_report_mouse_dblclick)
            return;
        m_report_mouse_dblclick(m_reportOpaque, button);
    }


private:
    //VLC objects
    libvlc_instance_t*  m_vlc = nullptr;
    libvlc_media_player_t* m_mp = nullptr;
    libvlc_media_t* m_media = nullptr;

    //FBO data
    unsigned m_width = 0;
    unsigned m_height = 0;
    std::mutex m_text_lock;
    std::shared_ptr<VLCTexture> m_tex_render;
    std::shared_ptr<VLCTexture> m_tex_swap;
    std::shared_ptr<VLCTexture> m_tex_display;
    bool m_updated = false;

    //window callbacks
    std::mutex m_window_lock;
    void* m_reportOpaque = nullptr;
    libvlc_video_output_resize_cb m_report_size_change = nullptr;
    libvlc_video_output_mouse_move_cb m_report_mouse_move = nullptr;
    libvlc_video_output_mouse_press_cb m_report_mouse_press = nullptr;
    libvlc_video_output_mouse_release_cb m_report_mouse_release = nullptr;
    libvlc_video_output_mouse_double_click_cb m_report_mouse_dblclick = nullptr;

    //SDL context
    SDL_Window* m_win;
    SDL_GLContext m_ctx;
};

int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <uri>\n", argv[0]);
        return 1;
    }

    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    //VLC opengl context needs to be shared with SDL context
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    SDL_GL_SetSwapInterval(0);

    SDL_Window* wnd = SDL_CreateWindow("test",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);



    SDL_GLContext glc = SDL_GL_CreateContext(wnd);
    SDL_GL_MakeCurrent(wnd, glc);
    glewInit();

    VLCVideo video(wnd);
    SDL_GL_MakeCurrent(wnd, glc);

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    //vertex X, vertex Y, UV X, UV Y
    GLfloat vertices[] = {
        -1.f,  1.f, 0.f, 1.f,
        -1.f, -1.f, 0.f, 0.f,
         1.f,  1.f, 1.f, 1.f,
         1.f, -1.f, 1.f, 0.f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    {
        GLuint shader[] = {vertexShader, fragmentShader};
        const char *shaderName[] = {"vertex", "fragment"};
        for (int i = 0; i < 2; i++) {
            int len;
            glGetShaderiv(shader[i], GL_INFO_LOG_LENGTH, &len);
            if (len <= 1)
                continue;
            std::vector<char> infoLog(len);
            int charsWritten;
            glGetShaderInfoLog(shader[i], len, &charsWritten, infoLog.data());
            fprintf(stderr, "%s shader info log: %s\n", shaderName[i], infoLog.data());

            GLint status = GL_TRUE;
            glGetShaderiv(shader[i], GL_COMPILE_STATUS, &status);
            if (status == GL_FALSE) {
                fprintf(stderr, "compile %s shader failed\n", shaderName[i]);
                SDL_DestroyWindow(wnd);
                SDL_Quit();
                return 1;
            }
        }
    }

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    {
        int len;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &len);
        if (len > 1) {
            std::vector<char> infoLog(len);
            int charsWritten;
            glGetProgramInfoLog(shaderProgram, len, &charsWritten, infoLog.data());
            fprintf(stderr, "shader program: %s\n", infoLog.data());
        }

        GLint status = GL_TRUE;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
            fprintf(stderr, "unable to use program\n");
            SDL_DestroyWindow(wnd);
            SDL_Quit();
            return 1;
        }
    }

    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "a_position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);

    // Specify the layout of the vertex data
    GLint uvAttrib = glGetAttribLocation(shaderProgram, "a_uv");
    glEnableVertexAttribArray(uvAttrib);
    glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (GLvoid*)(2*sizeof(float)));

    // Specify the texture of the video
    GLint textUniform = glGetUniformLocation(shaderProgram, "u_videotex");
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(textUniform, /*GL_TEXTURE*/0);

    //start playing the video
    if (!video.playMedia(argv[1])) {
        SDL_DestroyWindow(wnd);
        SDL_Quit();
        return 1;
    }

    Uint32 windowID = SDL_GetWindowID(wnd);
    bool updated = false;
    bool quit = false;
    std::shared_ptr<VLCTexture> vlcTexture;
    while(!quit) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_WINDOWEVENT: {
                if (e.window.windowID == windowID)  {
                    switch (e.window.event) {
                    case SDL_WINDOWEVENT_EXPOSED: {
                        int width;
                        int height;
                        SDL_GL_GetDrawableSize(wnd, &width, &height);
                        video.onResize(width, height);
                        break;
                    }
                    case SDL_WINDOWEVENT_SIZE_CHANGED: {

                        int width;
                        int height;
                        SDL_GL_GetDrawableSize(wnd, &width, &height);
                        video.onResize(width, height);

                        //SDL seems to loose the current context after a resize
                        SDL_GL_MakeCurrent(wnd, glc);
                        glViewport(0, 0, width, height);

                        break;
                    }

                    case SDL_WINDOWEVENT_CLOSE: {
                        e.type = SDL_QUIT;
                        SDL_PushEvent(&e);
                        break;
                    }
                    }
                }
                break;
            }

            case SDL_MOUSEMOTION: {
                video.onMouseMove(e.motion.x, e.motion.y);
                break;
            }

            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN: {
                libvlc_video_output_mouse_button_t button;
                bool ok = true;
                switch (e.button.button) {
                case SDL_BUTTON_LEFT:
                    button = libvlc_video_output_mouse_button_left;
                    break;
                case SDL_BUTTON_MIDDLE:
                    button = libvlc_video_output_mouse_button_middle;
                    break;
                case SDL_BUTTON_RIGHT:
                    button = libvlc_video_output_mouse_button_right;
                    break;
                default:
                    ok = false;
                    break;
                }
                if (!ok)
                    break;

                if (e.button.state == SDL_PRESSED)
                    video.onMousePress(button);
                else
                    video.onMouseRelease(button);

                break;
            }
            }
        }

        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        // Get the current video texture and bind it
        auto newTexture = video.getVideoFrame(&updated);
        if (updated)
            vlcTexture = newTexture;

        if (vlcTexture)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, vlcTexture->texture());

            // Draw the video rectangle
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        SDL_GL_SwapWindow(wnd);
    };

    video.stop();

    SDL_DestroyWindow(wnd);
    SDL_Quit();

    return 0;
}
