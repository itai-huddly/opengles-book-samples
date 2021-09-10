//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

// ESUtil.c
//
//    A utility library for OpenGL ES.  This library provides a
//    basic common framework for the example applications in the
//    OpenGL ES 2.0 Programming Guide.
//

///
//  Includes
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "esUtil.h"

static const EGLint configAttribs[] = {
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_DEPTH_SIZE, 8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_NONE};

static const int pbufferWidth = 9;
static const int pbufferHeight = 9;

static const EGLint pbufferAttribs[] = {
    EGL_WIDTH,
    pbufferWidth,
    EGL_HEIGHT,
    pbufferHeight,
    EGL_NONE,
};

///
// CreateEGLContext()
//
//    Creates an EGL rendering context and all associated elements
//
EGLBoolean CreateEGLContext(EGLNativeWindowType hWnd, EGLDisplay *eglDisplay,
                            EGLContext *eglContext, EGLSurface *eglSurface,
                            EGLint attribList[])
{
    EGLint numConfigs;
    EGLint majorVersion;
    EGLint minorVersion;
    // EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
    EGLConfig config;
    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE};


    // Get Display
    void* test = fbGetDisplay();
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY)
    {
        esLogMessage("Failed to get display\n");
        esLogMessage("Error: %d\n", eglGetError());
        return EGL_FALSE;
    }

    esLogMessage("Display is 0x%08x\n", display);

    // Initialize EGL
    if (!eglInitialize(display, &majorVersion, &minorVersion))
    {
        esLogMessage("Failed with eglInitialize\n");
        esLogMessage("Error: 0x%x\n", eglGetError());
        return EGL_FALSE;
    }

    // Get configs
    if (!eglGetConfigs(display, NULL, 0, &numConfigs))
    {
        esLogMessage("Failed with eglGetConfigs\n");
        return EGL_FALSE;
    }

    // Choose config
    if (!eglChooseConfig(display, attribList, &config, 1, &numConfigs))
    {
        esLogMessage("Failed with eglChooseConfig\n");
        return EGL_FALSE;
    }

    // Create a surface
    surface = eglCreatePbufferSurface(display, config, pbufferAttribs);
    if (surface == EGL_NO_SURFACE)
    {
        esLogMessage("Failed with eglCreateBufferSurface\n");
        return EGL_FALSE;
    }

    // Create a GL context
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT)
    {
        return EGL_FALSE;
    }

    // Make the context current
    if (!eglMakeCurrent(display, surface, surface, context))
    {
        return EGL_FALSE;
    }

    *eglDisplay = display;
    *eglSurface = surface;
    *eglContext = context;
    return EGL_TRUE;
}

EGLBoolean FrameBufferCreate(ESContext *esContext, const char *title)
{
    return EGL_TRUE;
}

//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

///
//  esInitContext()
//
//      Initialize ES utility context.  This must be called before calling any other
//      functions.
//
void ESUTIL_API esInitContext(ESContext *esContext)
{
    if (esContext != NULL)
    {
        memset(esContext, 0, sizeof(ESContext));
    }
}

///
//  esCreateWindow()
//
//      title - name for title bar of window
//      width - width of window to create
//      height - height of window to create
//      flags  - bitwise or of window creation flags
//          ES_WINDOW_ALPHA       - specifies that the framebuffer should have alpha
//          ES_WINDOW_DEPTH       - specifies that a depth buffer should be created
//          ES_WINDOW_STENCIL     - specifies that a stencil buffer should be created
//          ES_WINDOW_MULTISAMPLE - specifies that a multi-sample buffer should be created
//
GLboolean ESUTIL_API esCreateFrameBuffer(ESContext *esContext, const char *title, GLint width, GLint height, GLuint flags)
{
    EGLint attribList[] =
        {
            EGL_RED_SIZE, 5,
            EGL_GREEN_SIZE, 6,
            EGL_BLUE_SIZE, 5,
            EGL_ALPHA_SIZE, (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
            EGL_DEPTH_SIZE, (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
            EGL_STENCIL_SIZE, (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
            EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
            EGL_NONE};

    if (esContext == NULL)
    {
        esLogMessage("Can't create window with null context\n");
        return GL_FALSE;
    }

    esContext->width = width;
    esContext->height = height;

    if (!FrameBufferCreate(esContext, title))
    {
        esLogMessage("Can't create frame buffer\n");
        return GL_FALSE;
    }

    if (!CreateEGLContext(esContext->hWnd,
                          &esContext->eglDisplay,
                          &esContext->eglContext,
                          &esContext->eglSurface,
                          attribList))
    {
        esLogMessage("Failed to create EGLContext\n");
        return GL_FALSE;
    }

    esLogMessage("Successfully created window\n");
    return GL_TRUE;
}

///
//  esMainLoop()
//
//    Start the main loop for the OpenGL ES application
//

void ESUTIL_API esMainLoop(ESContext *esContext)
{
    struct timeval t1, t2;
    struct timezone tz;
    float deltatime;
    float totaltime = 0.0f;
    unsigned int frames = 0;

    gettimeofday(&t1, &tz);

    esLogMessage("Starting esMainLoop\n");

    int num_of_frames_to_render = 5000;

    while (frames < num_of_frames_to_render)
    {
        gettimeofday(&t2, &tz);
        deltatime = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
        t1 = t2;

        if (esContext->updateFunc != NULL)
            esContext->updateFunc(esContext, deltatime);
        if (esContext->drawFunc != NULL)
            esContext->drawFunc(esContext);

        eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

        totaltime += deltatime;
        frames++;
        if (totaltime > 2.0f)
        {
            printf("%4d frames rendered in %1.4f seconds -> FPS=%3.4f\n", frames, totaltime, frames / totaltime);
            totaltime -= 2.0f;
            frames = 0;
        }
    }
}

///
//  esRegisterDrawFunc()
//
void ESUTIL_API esRegisterDrawFunc(ESContext *esContext, void(ESCALLBACK *drawFunc)(ESContext *))
{
    esContext->drawFunc = drawFunc;
}

///
//  esRegisterUpdateFunc()
//
void ESUTIL_API esRegisterUpdateFunc(ESContext *esContext, void(ESCALLBACK *updateFunc)(ESContext *, float))
{
    esContext->updateFunc = updateFunc;
}

///
//  esRegisterKeyFunc()
//
void ESUTIL_API esRegisterKeyFunc(ESContext *esContext,
                                  void(ESCALLBACK *keyFunc)(ESContext *, unsigned char, int, int))
{
    esContext->keyFunc = keyFunc;
}

///
// esLogMessage()
//
//    Log an error message to the debug output for the platform
//
void ESUTIL_API esLogMessage(const char *formatStr, ...)
{
    va_list params;
    char buf[BUFSIZ];

    va_start(params, formatStr);
    vsprintf(buf, formatStr, params);

    printf("%s", buf);

    va_end(params);
}

///
// esLoadTGA()
//
//    Loads a 24-bit TGA image from a file. This is probably the simplest TGA loader ever.
//    Does not support loading of compressed TGAs nor TGAa with alpha channel. But for the
//    sake of the examples, this is sufficient.
//

char *ESUTIL_API esLoadTGA(char *fileName, int *width, int *height)
{
    char *buffer = NULL;
    FILE *f;
    unsigned char tgaheader[12];
    unsigned char attributes[6];
    unsigned int imagesize;

    f = fopen(fileName, "rb");
    if (f == NULL)
        return NULL;

    if (fread(&tgaheader, sizeof(tgaheader), 1, f) == 0)
    {
        fclose(f);
        return NULL;
    }

    if (fread(attributes, sizeof(attributes), 1, f) == 0)
    {
        fclose(f);
        return 0;
    }

    *width = attributes[1] * 256 + attributes[0];
    *height = attributes[3] * 256 + attributes[2];
    imagesize = attributes[4] / 8 * *width * *height;
    buffer = malloc(imagesize);
    if (buffer == NULL)
    {
        fclose(f);
        return 0;
    }

    if (fread(buffer, 1, imagesize, f) != imagesize)
    {
        free(buffer);
        return NULL;
    }
    fclose(f);
    return buffer;
}
