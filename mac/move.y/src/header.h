//
//  header.h
//  ablEditor
//
//  Created by YotioSoft on 2020/02/20.
//

#ifndef header_h
#define header_h

#define SIV3D_WINDOWS_HIGH_DPI

#include <Siv3D.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdio.h>
#include <iostream>

using namespace std;

#define DEFAULT_SCREEN_SIZE_W	1680
#define DEFAULT_SCREEN_SIZE_H	948

typedef struct Position {
	int x;
	int y;
} Position;

typedef struct VideoTime {
	int hour;
	int minutes;
	int second;
	int millisecond;
} VideoTime;

typedef struct StringObject {
	Font* font;
	String string;
} StringObject;

namespace object_type {
	enum Type {
		string		= 1,
		rect		= 2,
		circle		= 3,
	};
}

#endif /* header_h */
