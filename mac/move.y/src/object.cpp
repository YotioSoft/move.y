//
//  object.cpp
//  ablEditor
//
//  Created by YotioSoft on 2020/02/21.
//

#include "object.hpp"

void Object::setFont(Font& argFont, Color argColor, String argString, Position argObjectPos, int argStartFrameNum, int argEndFrameNum) {
	StringObject newString;
	newString.font = &argFont;
	newString.string = argString;
	objectColor = argColor;
	string = &newString;
	
	objectPos = argObjectPos;
	objectBeginFrameNum = argStartFrameNum;
	objectEndFrameNum = argEndFrameNum;
	
	RectF fontDrawSize = (*newString.font)(newString.string).region(objectPos.x, objectPos.y);
	objectSize.x = fontDrawSize.w;
	objectSize.y = fontDrawSize.h;
	
	objectImage = Image(fontDrawSize.w, fontDrawSize.h, Color(0, 0));
	(*newString.font)(newString.string).overwrite(objectImage, 0, 0, objectColor);
	MultiPolygon mp = objectImage.alphaToPolygons().simplified(1).movedBy(-400, -500).scaled(0.025);
	
	objectTexture  = DynamicTexture(objectImage);
	
	objectType = object_type::string;
}

void Object::setRect(Rect &argRect, Color argColor, int argStartFrameNum, int argEndFrameNum) {
	rect = &argRect;
	objectColor = argColor;
	
	objectPos.x = rect->x;
	objectPos.y = rect->y;
	
	rect->x = 0;
	rect->y = 0;
	
	objectBeginFrameNum = argStartFrameNum;
	objectEndFrameNum = argEndFrameNum;
	
	objectSize.x = rect->w;
	objectSize.y = rect->h;
	
	objectImage = Image(objectSize.x, objectSize.y, Color(0, 0));
	rect->overwrite(objectImage, objectColor);
	
	objectTexture  = DynamicTexture(objectImage);
	
	objectType = object_type::rect;
}

void Object::setCircle(Circle &argCircle, Color argColor, int argStartFrameNum, int argEndFrameNum) {
	circle = &argCircle;
	objectColor = argColor;
	
	objectBeginFrameNum = argStartFrameNum;
	objectEndFrameNum = argEndFrameNum;
	
	objectType = object_type::circle;
}

pair<int, int> Object::getFrameTime() {
	return pair<int, int>{objectBeginFrameNum, objectEndFrameNum};
}

Position Object::getPos() {
	return objectPos;
}

Size Object::getSize() {
	return objectSize;
}

const Image& Object::getImage() {
	return objectImage;
}

DynamicTexture* Object::getTexture() {
	return &objectTexture;
}

object_type::Type Object::getObjectType() {
	return objectType;
}
