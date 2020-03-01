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

class TimeLine {
public:
	TimeLine(Video* argVideo);
	
	void draw(Size argWindowSize);
	
private:
	Video* video;
};

#endif /* timeline_hpp */
