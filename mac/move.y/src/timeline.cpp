//
//  timeline.cpp
//  movey
//
//  Created by YotioSoft on 2020/03/01.
//

#include "timeline.hpp"
#include "video.hpp"

TimeLine::TimeLine(Video* argVideo) {
	needToUpdate = true;
	video = argVideo;
}

Vec2 TimeLine::cursorPosOnTimeline(Size argWindowSize) {
	Vec2 cursorPosOnRenderTexture = {
		//(int)((Cursor::Pos().x-10)/(windowSize.x*0.75)*videoSize.x),
		//(int)((Cursor::Pos().y-10)/(videoSize.y/(double)videoSize.x*windowSize.x*0.75)*videoSize.y)
	};
	
	return cursorPosOnRenderTexture;
}

void TimeLine::selectObject(Object* argObjectP) {
	selectedObject = argObjectP;
	needToUpdate = true;
}

void TimeLine::draw(Size argWindowSize, pair<int, int> argRangeOfFrames, Font& argFont) {
	timelinePos.x = 10;
	timelinePos.y = argWindowSize.y-argWindowSize.y/5;
	
	timelineSize.x = argWindowSize.x-10*2;
	timelineSize.y = argWindowSize.y/5-10;
	
	if (rangeOfFrames.first != argRangeOfFrames.first || rangeOfFrames.second != argRangeOfFrames.second) {
		rangeOfFrames = argRangeOfFrames;
		needToUpdate = true;
	}
	
	int length = rangeOfFrames.second - rangeOfFrames.first;
	
	Array<Layer*> layers = video->getLayers();
	
	// オブジェクトの選択
	if (MouseL.down() &&
		timelinePos.x <= Cursor::Pos().x && timelinePos.y <= Cursor::Pos().y &&
		timelinePos.x+timelineSize.x >= Cursor::Pos().x && timelinePos.y+timelinePos.y >= Cursor::Pos().y) {
		
		selectedObject = nullptr;
		needToUpdate = true;
		
		for (int i=0; i<objectRects.size(); i++) {
			if (objectRects[i].second.x+timelinePos.x <= Cursor::Pos().x &&
				objectRects[i].second.y+timelinePos.y <= Cursor::Pos().y &&
				objectRects[i].second.x+objectRects[i].second.w+timelinePos.x >= Cursor::Pos().x &&
				objectRects[i].second.y+objectRects[i].second.h+timelinePos.y >= Cursor::Pos().y) {
				
				selectedObject = objectRects[i].first;
				selectedFrame = (double)(Cursor::Pos().x-timelinePos.x)/(double)timelineSize.x*length;
			}
		}
		
		video->selectObject(selectedObject);		// プレビューにも反映
	}
	
	// タイムラインの表示
	if (needToUpdate) {
		timelineTexture = MSRenderTexture(timelineSize.x, timelineSize.y, Color(50, 50, 50));
		ScopedRenderTarget2D target(timelineTexture);
		
		objectRects.clear();
		
		for (int lineY = 0; lineY < argWindowSize.y/5-10; lineY += 30) {
			Line(0, lineY, argWindowSize.x-10*2, lineY).draw(Color(Palette::White));
		}
		
		for (int i=0; i<layers.size(); i++) {
			Array<Object*> layerObjects = layers[i]->getObjects();
			
			for (int obj=0; obj<layerObjects.size(); obj++) {
				pair<int, int> objectFrames = layerObjects[obj]->getFrameTime();
				
				if ((objectFrames.first <= rangeOfFrames.first && rangeOfFrames.first <= objectFrames.second) ||
					(objectFrames.first <= rangeOfFrames.second && rangeOfFrames.second <= objectFrames.second) ||
					(rangeOfFrames.first <= objectFrames.first && objectFrames.first <= rangeOfFrames.second) ||
					(rangeOfFrames.first <= objectFrames.second && objectFrames.second <= rangeOfFrames.second)) {
					
					int firstX = (int)((double)(objectFrames.first-rangeOfFrames.first)/(double)length*(argWindowSize.x-10*2));
					int secondX = (int)((double)(objectFrames.second+1-rangeOfFrames.first)/(double)length*(argWindowSize.x-10*2));
					
					objectRects.push_back(pair<Object*, Rect>{layerObjects[obj], Rect(firstX, i*30+2, secondX-firstX, 26)});
					Color rectColor;
					switch (layerObjects[obj]->getObjectType()) {
						case object_type::string:
							rectColor = Color(149, 165, 166);
							break;
						case object_type::rect:
						case object_type::circle:
							rectColor = Color(108, 122, 137);
							break;
					}
					
					if (layerObjects[obj] == selectedObject) {
						// 選択されたオブジェクトをハイライト
						objectRects.back().second.draw(rectColor).drawFrame(2, 2, Color(230, 126, 34));
					}
					else {
						objectRects.back().second.draw(rectColor).drawFrame(1, 1, Color(Palette::Black));
					}
					
					argFont(layerObjects[obj]->getName()).draw(firstX+10, i*30+2, Color(Palette::White));
				}
			}
		}
		
		needToUpdate = false;
	}
	
	Graphics2D::Flush();
	timelineTexture.resolve();
	timelineTexture.draw(10, argWindowSize.y-argWindowSize.y/5);
}
