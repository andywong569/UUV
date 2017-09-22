#ifndef stdafx_h__
#define stdafx_h__


/** C RunTime Header Files	*/
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <strsafe.h>

#include <sstream>
#include <comutil.h>


/** ����UUVͷ�ļ� */ 
#include "../uuv/UUV_Command.h"
#include "../uuv/UUV_Command_Param.h"
#include "../uuv/IUUVObject.h"


/** ����ffmpegͷ�ļ� */ 
extern "C"
{
#include "../include/ffmpeg/libavcodec/avcodec.h"
#include "../include/ffmpeg/libavformat/avformat.h"
#include "../include/ffmpeg/libswscale/swscale.h"
};


/** ����opencvͷ�ļ� */ 
#include "../include/opencv2/highgui.hpp"
#include "../include/opencv2/imgproc.hpp"
#include "../include/opencv2/core.hpp"
#include "../include/opencv2/objdetect.hpp"


/** ����ffmpeg�� */ 
#pragma comment(lib, "../lib/avcodec.lib")
#pragma comment(lib, "../lib/avformat.lib")
#pragma comment(lib, "../lib/avutil.lib")
#pragma comment(lib, "../lib/swscale.lib")


/** ����opencv�� */ 
#pragma comment(lib, "../lib/opencv_core2413.lib")
#pragma comment(lib, "../lib/opencv_imgproc2413.lib")
#pragma comment(lib, "../lib/opencv_highgui2413.lib")
#pragma comment(lib, "../lib/opencv_objdetect2413.lib")


/** ����opencv�� */ 
#pragma comment(lib, "../lib/UUV.lib")


#endif // stdafx_h__
