/***
  This file is part of PulseAudio. MODIFIED
 
  PulseAudio is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation; either version 2.1 of the License,
  or (at your option) any later version.
 
  PulseAudio is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.
 
  You should have received a copy of the GNU Lesser General Public License
  along with PulseAudio; if not, see <http://www.gnu.org/licenses/>.
***/
 
 
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <threads.h>
 
#include <pulse/simple.h>
#include <pulse/error.h>

#include <Windows.h>
#define ID_ALT_NUM(ch) (ch - '0')
#define ID_NUM(ch) (ch - '0' + 10)
#define ID_LETTER(ch) (ch - 'A' + 20)
#define VK(ch) (ch)
 
#define BUFSIZE 1024
 
int play_file(FILE *fd) {
 
    /* The Sample format to use */
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 48000,
        .channels = 1
    };
 
    pa_simple *s = NULL;
    int ret = EXIT_FAILURE;
    int error;
 
    /* Create a new playback stream */
    if (!(s = pa_simple_new(NULL, "soundpad", PA_STREAM_PLAYBACK, NULL, "soundpad", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }
 
    for (;;) {
        unsigned char buf[BUFSIZE];
        size_t num;
 
        /* Read some data ... */
        if ((num = fread (buf, sizeof (buf[0]), BUFSIZE, fd)) < BUFSIZE) {
            if (feof (fd)) /* EOF */
                break;
 
            fprintf(stderr, __FILE__": fread() failed: %s\n", strerror(errno));
            goto finish;
        }
 
        /* ... and play it */
        if (pa_simple_write(s, buf, num, &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
            goto finish;
        }
    }
 
    /* Make sure that every single sample was played */
    if (pa_simple_drain(s, &error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
        goto finish;
    }
 
    ret = EXIT_SUCCESS;
 
finish:
 
    if (s)
        pa_simple_free(s);
 
    return ret;
}


bool unregister_num_hot_keys () {
  UnregisterHotKey (NULL, ID_NUM ('0'));
  UnregisterHotKey (NULL, ID_NUM ('1'));
  UnregisterHotKey (NULL, ID_NUM ('2'));
  UnregisterHotKey (NULL, ID_NUM ('3'));
  UnregisterHotKey (NULL, ID_NUM ('4'));
  UnregisterHotKey (NULL, ID_NUM ('5'));
  UnregisterHotKey (NULL, ID_NUM ('6'));
  UnregisterHotKey (NULL, ID_NUM ('7'));
  UnregisterHotKey (NULL, ID_NUM ('8'));
  UnregisterHotKey (NULL, ID_NUM ('9'));
}

char *error_sound = "samples/Doomfist_-_Rising_Uppercut.s16le";
char *success_sound = "samples/Doomfist_-_Rising_fist.s16le";

void play_file_async (FILE *file_name);
void play_file_name_async (char *file_name);
void try_play_file (char *file_name) {
  FILE *fd;
  if ((fd = fopen(file_name, "rb")) != NULL) return;
  play_file_async (fd);
  fclose (fd);
}
#define error_play() do { \
  fprintf (stderr, "%s: Error occured\n", __func__); \
  try_play_file (error_sound); \
} while (0)
#define success_play() do { \
  try_play_file (success_sound); \
} while (0)

int play_file_name (char *file_name) {
  FILE *fd;
  if ((fd = fopen (file_name, "rb")) == NULL) {
    fprintf(stderr, __FILE__": fopen() failed: %s\n", strerror(errno));
    error_play();
    return EXIT_FAILURE;
  }
  if (play_file (fd) == EXIT_FAILURE) {
    fclose (fd);
    error_play();
    return EXIT_FAILURE;
  }
  fclose (fd);
  return EXIT_SUCCESS;
}

thrd_t foo_thrd;
void play_file_async (FILE *file) {
  if (thrd_create (&foo_thrd, (thrd_start_t) &play_file, file) != thrd_success)
    fprintf (stderr, "%s failed\n", __func__);
}
void play_file_name_async (char *file_name) {
  if (thrd_create (&foo_thrd, (thrd_start_t) &play_file_name, file_name) != thrd_success)
    fprintf (stderr, "%s failed\n", __func__);
}

// Breathing3.wav
// Breathing4.wav
// Breathing7.wav
// Doomfist_-_Rising_Uppercut.pcm
// Doomfist_-_Rising_Uppercut.s16le
// Doomfist_-_Rising_Uppercut.wav
// Doomfist_-_Rising_fist.wav
// Laugh1.wav
// Laugh2.wav
// Laughter.wav
// Mercy_-_Dying_sounds.wav
// Mercy_-_Dying_sounds2.wav
// Mercy_-_Dying_sounds3.wav
// Mercy_-_Dying_sounds4.wav
// Mercy_-_Dying_sounds5.wav
// Mercy_-_Dying_sounds6.wav
// Mercy_-_Prolonged_screaming.wav
// Mercy_-_Prolonged_screaming2.wav
// Mercy_-_Prolonged_screaming3.wav
// Mercy_-_Scream.wav
// Mercy_-_Scream2.wav
// Mercy_-_Scream3.wav
// Mercy_-_Scream4.wav
// Mr. Gone.wav
// Tryhard.wav
// You_amussed_me.wav
// ayomymom.wav
// dracula.wav
// forlorn.wav
// laitouxi.wav
// parot.wav
// parot2.wav
// src
// watertop.wav
char *sound_files[] = {
  "samples/Doomfist_-_Rising_Uppercut.s16le",
  "samples/Mercy_-_Dying_sounds.s16le",
  "samples/Mercy_-_Dying_sounds2.s16le",
  "samples/Mercy_-_Dying_sounds3.s16le",
  "samples/Mercy_-_Dying_sounds4.s16le",
  "samples/Mercy_-_Dying_sounds5.s16le",
  "samples/Mercy_-_Dying_sounds6.s16le",
  "samples/Mercy_-_Prolonged_screaming.s16le",
  "samples/Mercy_-_Prolonged_screaming2.s16le",
  "samples/Mercy_-_Prolonged_screaming3.s16le",
  "samples/Mercy_-_Scream.s16le",
  "samples/Mercy_-_Scream2.s16le",
  "samples/Mercy_-_Scream3.s16le",
  "samples/Mercy_-_Scream4.s16le",
};

int main (int argc, char *argv[]) {
  bool num_registered = false;
  if (!RegisterHotKey (NULL, ID_ALT_NUM ('0'), MOD_ALT | MOD_NOREPEAT, VK('0'))) {
    fprintf (stderr, __FILE__" RegisterHotKey () failed\n");
    goto finish;
  }
  if (!RegisterHotKey (NULL, ID_ALT_NUM ('1'), MOD_ALT | MOD_NOREPEAT, VK('1'))) goto finish;
  assert (play_file_name (success_sound) == EXIT_SUCCESS);
  
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0) && msg.message == WM_HOTKEY) {
    int id = msg.wParam;
    switch (id) {
    case ID_ALT_NUM('0'):
      fprintf (stderr, "Quit\n");
      success_play ();
      PostQuitMessage(0);
      break;
    case ID_ALT_NUM('1'):
      if (num_registered) {
        num_registered = false;
        unregister_num_hot_keys ();
        success_play ();
        fprintf (stderr, "Unregistered hot keys\n");
      } else {
        if (RegisterHotKey (NULL, ID_NUM ('0'), MOD_NOREPEAT, VK('0')) &&
            RegisterHotKey (NULL, ID_NUM ('1'), MOD_NOREPEAT, VK('1')) &&
            RegisterHotKey (NULL, ID_NUM ('2'), MOD_NOREPEAT, VK('2')) &&
            RegisterHotKey (NULL, ID_NUM ('3'), MOD_NOREPEAT, VK('3')) &&
            RegisterHotKey (NULL, ID_NUM ('4'), MOD_NOREPEAT, VK('4')) &&
            RegisterHotKey (NULL, ID_NUM ('5'), MOD_NOREPEAT, VK('5')) &&
            RegisterHotKey (NULL, ID_NUM ('6'), MOD_NOREPEAT, VK('6')) &&
            RegisterHotKey (NULL, ID_NUM ('7'), MOD_NOREPEAT, VK('7')) &&
            RegisterHotKey (NULL, ID_NUM ('8'), MOD_NOREPEAT, VK('8')) &&
            RegisterHotKey (NULL, ID_NUM ('9'), MOD_NOREPEAT, VK('9'))) {
          num_registered = true;
          success_play ();
          fprintf (stderr, "Registered hot keys\n");
        } else
          unregister_num_hot_keys ();
          error_play ();
      }
      break;
    case ID_NUM('0'):
    case ID_NUM('1'):
    case ID_NUM('2'):
    case ID_NUM('3'):
    case ID_NUM('4'):
    case ID_NUM('5'):
    case ID_NUM('6'):
    case ID_NUM('7'):
    case ID_NUM('8'):
    case ID_NUM('9'):
      play_file_name_async (sound_files[id - ID_NUM ('0')]);
      break;
    default:
      error_play ();
    }
  }

finish:
  unregister_num_hot_keys ();
  UnregisterHotKey(NULL, ID_ALT_NUM('1'));
  UnregisterHotKey(NULL, ID_ALT_NUM('0'));

#if 0
  if (argc < 2) {
    fprintf (stderr, "Please specify a file name\n");
    return EXIT_FAILURE;
  }
  FILE *fd = fopen (argv[1], "rb");
  if (fd == NULL) {
    fprintf(stderr, __FILE__": fopen() failed: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }
  play_file (fd);
  fclose(fd);
#endif

  return 0;
}
