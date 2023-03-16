// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * dmo.h : DirectMedia Object codec module for vlc
 *****************************************************************************
 * Copyright (C) 2002, 2003 VLC authors and VideoLAN
 *
 * Author: Gildas Bazin <gbazin@videolan.org>
 *****************************************************************************/

extern const GUID IID_IUnknown;
extern const GUID IID_IClassFactory;
extern const GUID IID_IWMCodecPrivateData;
extern const GUID IID_IMediaObject;
extern const GUID IID_IMediaBuffer;
extern const GUID MEDIATYPE_Video;
extern const GUID MEDIATYPE_Audio;
extern const GUID MEDIASUBTYPE_PCM;
extern const GUID FORMAT_VideoInfo;
extern const GUID FORMAT_WaveFormatEx;
extern const GUID GUID_NULL;
extern const GUID MEDIASUBTYPE_I420;
extern const GUID MEDIASUBTYPE_YV12;
extern const GUID MEDIASUBTYPE_RGB24;
extern const GUID MEDIASUBTYPE_RGB565;

#include <mediaobj.h>
#include <dmoreg.h>


#ifndef _WIN32
void* CoTaskMemAlloc(unsigned long cb);
void CoTaskMemFree(void* cb);
#endif

typedef struct _IWMCodecPrivateData IWMCodecPrivateData;

#ifndef STDCALL
#define STDCALL __stdcall
#endif

#define DMO_INPUT_DATA_BUFFERF_SYNCPOINT 1
#define DMO_INPUT_DATA_BUFFERF_TIME 2
#define DMO_INPUT_DATA_BUFFERF_TIMELENGTH 4
#define DMO_OUTPUT_DATA_BUFFERF_SYNCPOINT 1
#define DMO_OUTPUT_DATA_BUFFERF_TIME 2
#define DMO_OUTPUT_DATA_BUFFERF_TIMELENGTH 4
#define DMO_PROCESS_OUTPUT_DISCARD_WHEN_NO_BUFFER 1
#define DMO_E_NOTACCEPTING 0x80040204

/*
 * IWMCodecPrivateData interface
 */
typedef struct IWMCodecPrivateData_vt
{
    long (STDCALL *QueryInterface)(IWMCodecPrivateData *This, const GUID* riid,
                                   void **ppvObject);
    long (STDCALL *AddRef)(IWMCodecPrivateData *This) ;
    long (STDCALL *Release)(IWMCodecPrivateData *This) ;


    long (STDCALL *SetPartialOutputType)(IWMCodecPrivateData * This,
                                         DMO_MEDIA_TYPE *pmt);

    long (STDCALL *GetPrivateData )(IWMCodecPrivateData * This,
                                    uint8_t *pbData, uint32_t *pcbData);
} IWMCodecPrivateData_vt;

struct _IWMCodecPrivateData { IWMCodecPrivateData_vt* vt; };

/* Implementation of IMediaBuffer */
typedef struct _CMediaBuffer
{
    IMediaBuffer intf;
    int i_ref;
    block_t *p_block;
    int i_max_size;
    bool b_own;

} CMediaBuffer;

CMediaBuffer *CMediaBufferCreate( block_t *, int, bool );
