// Written by Magnus Larsen (Nopey Nope)
// for use with POSIX systems, to replace some of windows.h
#include <signal.h>
#include <cstdint>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#undef min
#undef max

// The first two are used by gameui_interface.cpp
#define __stdcall /* Nothing! */

//inline, because we don't want to actually have a function call.
//static, in case we have to anyways.
inline static void __debugbreak(){
    raise(SIGTRAP);
}

// The following are used by optionssubmultiplayer.cpp
inline static bool CopyFile( char const *lpExistingFileName, char const *lpNewFileName, bool bFailIfExists){
    size_t const CHUNK = 8192;

    char buf[CHUNK];
    size_t size;

    int source = open("from.ogv", O_RDONLY, 0);
    if (!source) return false;
    
    int dest = open(lpNewFileName, O_WRONLY | O_CREAT | O_TRUNC | (bFailIfExists && O_EXCL), 0644);
    if (!dest) {
        close(source);
        return false;
    }

    while ((size = read(source, buf, CHUNK)))
    {
        (void)! write(dest, buf, size);
    }
    close(source);
    close(dest);
    return true;
}
inline static bool DeleteFile(char const *lpFileName){
    return !remove(lpFileName);
}
typedef struct tagRGBQUAD {
  unsigned char rgbBlue;
  unsigned char rgbGreen;
  unsigned char rgbRed;
  unsigned char rgbReserved;
} RGBQUAD;
typedef struct tagBITMAPFILEHEADER {
  uint16_t bfType;
  uint32_t bfSize;
  uint16_t bfReserved1;
  uint16_t bfReserved2;
  uint32_t bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER {
  uint32_t biSize;
  uint64_t  biWidth;
  uint64_t  biHeight;
  uint16_t  biPlanes;
  uint16_t  biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImage;
  uint64_t  biXPelsPerMeter;
  uint64_t  biYPelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
typedef struct tagBITMAPINFO {
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD          bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO, *PBITMAPINFO;
