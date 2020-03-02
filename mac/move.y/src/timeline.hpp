//
//  timeline.hpp
//  movey
//
//  Created by YotioSoft on 2020/03/01.
//

#ifndef timeline_hpp
#define timeline_hpp

#include "header.h"
#include "video.hpp"
#include "layer.hpp"

class TimeLine {
public:
	TimeLine(Video* argVideo);
	
	// マウス座標をタイムライン上の座標に変換
	Vec2 cursorPosOnTimeline(Size argWindowSize);
	
	void draw(Size argWindowSize, pair<int, int> argRangeOfFrames, Font& argFont);
	
private:
	Position timelinePos;
	Size timelineSize;
	
	bool needToUpdate;
	MSRenderTexture timelineTexture;
	
	Video* video;
	
	Array<pair<Object*, Rect>> objectRects;
	Object* selectedObject;
	int selectedFrame;
	
	pair<int, int> rangeOfFrames;		// 現在表示中のフレームの範囲
};

#endif /* timeline_hpp */
