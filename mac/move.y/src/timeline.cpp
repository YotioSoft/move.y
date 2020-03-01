//
//  timeline.cpp
//  movey
//
//  Created by YotioSoft on 2020/03/01.
//

#include "timeline.hpp"

TimeLine::TimeLine(Video* argVideo) {
	video = argVideo;
}

void TimeLine::draw(Size argWindowSize) {
	Rect background(10, argWindowSize.y-argWindowSize.y/5, argWindowSize.x-10*2, argWindowSize.y/5-10);
	background.draw(Color(50, 50, 50));
	
	Array<Layer*> layers = video->getLayers();
	
	for (int lineY = argWindowSize.y-argWindowSize.y/5; lineY < argWindowSize.y; lineY += 30) {
		Line(10, lineY, argWindowSize.x-10, lineY).draw(Color(Palette::White));
	}
	/*
	for (int i=0; i<layers.size(); i++) {
		Rect layerBackground(10, argWindowSize.y-argWindowSize.y/4+55*i, argWindowSize.x-10*2, 50);
		layerBackground.draw(Color(Palette::Whitesmoke));
	}*/
}
