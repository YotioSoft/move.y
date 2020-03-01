//
//  converter.cpp
//  movey
//
//  Created by YotioSoft on 2020/03/01.
//

#include "converter.hpp"

void imageToMat(cv::Mat& argMat, Image& argImage) {
	for (int y=0; y<argImage.size().y; y++) {
		cv::Vec3b* linePixelsPtr = argMat.ptr<cv::Vec3b>(y);
		
		for (int x=0; x<argImage.size().x; x++) {
			linePixelsPtr[x] = cv::Vec3b(argImage[y][x].b, argImage[y][x].g, argImage[y][x].r);
		}
	}
}

void addObjectToMat(cv::Mat& argTo, Object& argFrom, int argFrameNum) {
	Position objectImagePos = argFrom.getPos(argFrameNum);
	Size objectImageSize = argFrom.getSize();
	
	for (int y=0; y<objectImageSize.y; y++) {
		cv::Vec3b* linePixelsPtr = argTo.ptr<cv::Vec3b>(objectImagePos.y+y);
		
		for (int x=0; x<objectImageSize.x; x++) {
			linePixelsPtr[objectImagePos.x+x] = cv::Vec3b(argFrom.getImage()[y][x].b, argFrom.getImage()[y][x].g, argFrom.getImage()[y][x].r);
		}
	}
}
