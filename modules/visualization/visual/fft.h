/*****************************************************************************
 * fft.h: Headers for iterative implementation of a FFT
 *****************************************************************************
 *
 * Mainly taken from XMMS's code
 *
 * Authors: Richard Boulton <richard@tartarus.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef VLC_VISUAL_FFT_H_
#define VLC_VISUAL_FFT_H_

#define FFT_BUFFER_SIZE_LOG 9

#define FFT_BUFFER_SIZE (1 << FFT_BUFFER_SIZE_LOG)

/* sound sample - should be an signed 16 bit value */
typedef short int sound_sample;

struct _struct_fft_state {
     /* Temporary data stores to perform FFT in. */
     float real[FFT_BUFFER_SIZE];
     float imag[FFT_BUFFER_SIZE];

     /* */
     unsigned int bitReverse[FFT_BUFFER_SIZE];

     /* The next two tables could be made to use less space in memory, since they
      * overlap hugely, but hey. */
     float sintable[FFT_BUFFER_SIZE / 2];
     float costable[FFT_BUFFER_SIZE / 2];
};

/* FFT prototypes */
typedef struct _struct_fft_state fft_state;
fft_state *visual_fft_init (void);
void fft_perform (const sound_sample *input, float *output, fft_state *state);
void fft_close (fft_state *state);


#endif /* include-guard */
