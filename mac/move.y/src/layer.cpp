//
//  layer.cpp
//  ablEditor
//
//  Created by YotioSoft on 2020/02/21.
//

#include "layer.hpp"

bool Layer::addObject(Object& argObject) {
	objects.push_back(&argObject);
	
	return true;
}

Object* Layer::getObject(int argFrameNum) {
	for (int i=0; i<objects.size(); i++) {
		if (objects[i]->getFrameTime().first <= argFrameNum && argFrameNum <= objects[i]->getFrameTime().second) {
			return objects[i];
		}
	}
	return nullptr;
}

int Layer::maxFrameNum() {
	int max = 0;
	for (int i=0; i<objects.size(); i++) {
		if (objects[i]->getFrameTime().second > max) {
			max = objects[i]->getFrameTime().second;
		}
	}
	return max;
}
