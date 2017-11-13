#include <windows.h>
#define XM_RESOURCE 0
#define XM_MEMORY   1
#define XM_FILE     2

#ifdef __cplusplus
	extern "C" {
#endif

/* The uFMOD_PlaySong function starts playing an XM song
   --------------
   HWAVEOUT* uFMOD_PlaySong(
     LPVOID lpXM,
	 DWORD  param,
	 DWORD  fdwSong
   );
   Parameters:
     lpXM
        Specifies the song to play. If this parameter is NULL,
        any currently playing song is stopped. In such a case, function
		does not return a meaningful value
		fdwSong parameter determines whether this value is interpreted
        as a filename, as a resource identifier or a pointer to an image
        of the song in memory
     param
		Handle to the executable file that contains the resource to be
		loaded or size of the image of the song in memory. This parameter
		is ignored unless XM_RESOURCE or XM_MEMORY is specified in fdwSong
     fdwSong
        Flags for playing the song. The following values are defined
        Value        Meaning
        XM_FILE      lpXM points to filename
                     param is ignored
        XM_MEMORY    lpXM points to an image of a song in memory
                     param is the image size
        XM_RESOURCE  lpXM Specifies the name of the resource
                     param identifies the module whose executable file
                     contains the resource
                     The resource type must be RT_RCDATA
  Return Values:
     Returns a pointer to HWAVEOUT on success or NULL otherwise
*/
void* __stdcall uFMOD_PlaySong(LPVOID lpXM, DWORD  param,DWORD  fdwSong);
#define uFMOD_StopSong() uFMOD_PlaySong(0, 0, 0)

/* The uFMOD_GetStats function returns the current RMS volume coefficients
   in L and R channels.
   --------------
   unsigned int uFMOD_GetStats();
   Return Values:
      low-order word : RMS volume in R channel
      hi-order  word : RMS volume in L channel
*/
unsigned int __stdcall uFMOD_GetStats();

/* The uFMOD_GetTime function returns the time in milliseconds since the
   song was started. This is useful for synchronizing purposes.
   --------------
   unsigned int uFMOD_GetTime();
   Return Values:
      Returns the time in milliseconds since the song was started
*/
unsigned int __stdcall uFMOD_GetTime();

/* The uFMOD_GetTitle function returns the current track's title, if any.
   --------------
   const char* uFMOD_GetTitle();
   Return Values:
      Returns the track's title in ASCIIZ format
*/
const char* __stdcall uFMOD_GetTitle();

#ifdef __cplusplus
	}
#endif
