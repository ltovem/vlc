/* compile: cc scree_player.cpp -o scree_player.exe -L<path/libvlc> -lvlc -ld3d11 -luuid -ldxgi */
// Copyright © 2023 VideoLabs, VLC authors and VideoLAN
// SPDX-License-Identifier: ISC
//
// Authors: Steve Lhomme <robux4@videolabs.io>

#include <vlc/vlc.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>
#include <d3d11_1.h>

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#define SCREEN_WIDTH  1500
#define SCREEN_HEIGHT  900

struct vlc_context
{
    libvlc_instance_t     *p_libvlc = nullptr;
    libvlc_media_player_t *p_mediaplayer = nullptr;

    ComPtr<IDXGIOutputDuplication> duplication;
};


static int OpenD3D11Src(void *opaque, void **datap, uint64_t *sizep)
{
    auto pContext = static_cast<vlc_context*>(opaque);
    ComPtr<IDXGIFactory1> factory;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
        return -1;
    ComPtr<IDXGIAdapter1> adapter;
    for (UINT i=0;;i++)
    {
        if (FAILED(factory->EnumAdapters1(i, adapter.GetAddressOf())))
            break;

        UINT creationFlags = 0;
#ifndef NDEBUG
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        ComPtr<ID3D11Device> d3ddev;
        // ComPtr<ID3D11DeviceContext> d3dctx;
        D3D11CreateDevice(adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr,
                          creationFlags, nullptr, 0, D3D11_SDK_VERSION, &d3ddev,
                          nullptr, nullptr);

        ComPtr<IDXGIOutput1> output1;
        for (UINT j=0; output1.Get()==nullptr; j++)
        {
            ComPtr<IDXGIOutput> output;
            if (FAILED(adapter->EnumOutputs(j, &output)))
                break;

            if (FAILED(output.As(&output1)))
                continue;

            HRESULT hr = output1->DuplicateOutput(d3ddev.Get(), pContext->duplication.GetAddressOf());
            if (FAILED(hr))
                output1.Reset();
        }

        if (output1.Get()!=nullptr)
            break;
    }

    *datap = opaque;
    return 0;
}

static ptrdiff_t GetD3D11(void *opaque, void **buf, int64_t *pts_us)
{
    auto pContext = static_cast<vlc_context*>(opaque);

    ComPtr<IDXGIResource> resource;
    ComPtr<ID3D11Resource> d3d11res;

    HRESULT hr;
    DXGI_OUTDUPL_FRAME_INFO frameInfo;
    do {
        hr = pContext->duplication->ReleaseFrame();
        hr = pContext->duplication->AcquireNextFrame(1000, &frameInfo, &resource);
        if (FAILED(hr))
            return -1;
    } while (frameInfo.AccumulatedFrames == 0);

    hr = resource.As(&d3d11res);
    if (FAILED(hr))
        return -1;

    *buf = d3d11res.Detach();

    *pts_us = frameInfo.LastPresentTime.QuadPart / 10;

    return 1;
}

static void ReleaseD3D11(void *opaque, void *buf)
{
    auto pContext = static_cast<vlc_context*>(opaque);
    auto tex = static_cast<ID3D11Resource *>(buf);
    tex->Release();
}

static void CloseD3D11Src(void *opaque)
{
    auto pContext = static_cast<vlc_context*>(opaque);
    pContext->duplication->ReleaseFrame();
    pContext->duplication.Reset();
}

static const char *AspectRatio = nullptr;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if( message == WM_CREATE )
    {
        /* Store p_mediaplayer for future use */
        CREATESTRUCT *c = (CREATESTRUCT *)lParam;
        SetWindowLongPtr( hWnd, GWLP_USERDATA, (LONG_PTR)c->lpCreateParams );
        return 0;
    }

    LONG_PTR p_user_data = GetWindowLongPtr( hWnd, GWLP_USERDATA );
    if( p_user_data == 0 )
        return DefWindowProc(hWnd, message, wParam, lParam);
    struct vlc_context *ctx = (struct vlc_context *)p_user_data;

    switch(message)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            {
                int key = tolower( MapVirtualKey( (UINT)wParam, 2 ) );
                if (key == 'a')
                {
                    if (AspectRatio == nullptr)
                        AspectRatio = "16:10";
                    else if (strcmp(AspectRatio,"16:10")==0)
                        AspectRatio = "16:9";
                    else if (strcmp(AspectRatio,"16:9")==0)
                        AspectRatio = "4:3";
                    else if (strcmp(AspectRatio,"4:3")==0)
                        AspectRatio = "185:100";
                    else if (strcmp(AspectRatio,"185:100")==0)
                        AspectRatio = "221:100";
                    else if (strcmp(AspectRatio,"221:100")==0)
                        AspectRatio = "235:100";
                    else if (strcmp(AspectRatio,"235:100")==0)
                        AspectRatio = "239:100";
                    else if (strcmp(AspectRatio,"239:100")==0)
                        AspectRatio = "5:3";
                    else if (strcmp(AspectRatio,"5:3")==0)
                        AspectRatio = "5:4";
                    else if (strcmp(AspectRatio,"5:4")==0)
                        AspectRatio = "1:1";
                    else if (strcmp(AspectRatio,"1:1")==0)
                        AspectRatio = nullptr;
                    libvlc_video_set_aspect_ratio( ctx->p_mediaplayer, AspectRatio );
                }
                break;
            }
        default: break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wc;
    char *file_path;
    struct vlc_context Context;
    libvlc_media_t *p_media;
    (void)hPrevInstance;
    HWND hWnd;

    /* remove "" around the given path */
    if (lpCmdLine[0] == '"')
    {
        file_path = _strdup( lpCmdLine+1 );
        if (file_path[strlen(file_path)-1] == '"')
            file_path[strlen(file_path)-1] = '\0';
    }
    else
        file_path = _strdup( lpCmdLine );

    Context.p_libvlc = libvlc_new( 0, nullptr );
    p_media = libvlc_media_new_timed_callbacks( libvlc_media_source_d3d11, OpenD3D11Src, GetD3D11, ReleaseD3D11, CloseD3D11Src, &Context );
    free( file_path );
    Context.p_mediaplayer = libvlc_media_player_new_from_media(
                                                   Context.p_libvlc, p_media );

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = "WindowClass";

    RegisterClassEx(&wc);

    RECT wr = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowEx(0,
                          "WindowClass",
                          "libvlc Demo app",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          wr.right - wr.left,
                          wr.bottom - wr.top,
                          nullptr,
                          nullptr,
                          hInstance,
                          &Context);

    libvlc_media_player_set_hwnd(Context.p_mediaplayer, hWnd);

    ShowWindow(hWnd, nCmdShow);

    libvlc_media_player_play( Context.p_mediaplayer );

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if(msg.message == WM_QUIT)
            break;
    }

    libvlc_media_player_stop_async( Context.p_mediaplayer );

    libvlc_media_release( libvlc_media_player_get_media( Context.p_mediaplayer ) );
    libvlc_media_player_release( Context.p_mediaplayer );
    libvlc_release( Context.p_libvlc );

    return (int)msg.wParam;
}
