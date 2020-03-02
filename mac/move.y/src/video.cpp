//
//  video.cpp
//  ablEditor
//
//  Created by YotioSoft on 2020/02/21.
//

#include "video.hpp"

Video::Video(Size argVideoSize) {
	videoSize = argVideoSize;
	
	previewTexture = MSRenderTexture(videoSize.x, videoSize.y, Color(Palette::Black));
	encordingTexture = MSRenderTexture(videoSize.x, videoSize.y, Color(Palette::Black));
	previewingFrameNum = 0;
	
	totalFrames = 0;
	wroteFrames = 0;
}

int Video::addLayer() {
	layers.push_back(new Layer());
	return (int)layers.size();
}

Array<Layer*> Video::getLayers() {
	return layers;
}

bool Video::addObjectToLayer(int argLayerNum, Object* argObject) {
	if (argLayerNum > layers.size()) {
		return false;
	}
	
	needToUpdatePreview = true;
	
	bool ret = layers[argLayerNum]->addObject(argObject);
	if (layers[argLayerNum]->maxFrameNum() > totalFrames) {
		totalFrames = layers[argLayerNum]->maxFrameNum();
	}
	
	return ret;
}

int Video::cursorOnObject(int argFrameNum, Vec2 argCursorPos) {
	int clickedLayerNumber = -1;
	Vec2 cursorPosOnRenderTexture = argCursorPos;
	
	for (int i=0; i<layers.size(); i++) {
		if (layers[i]->getObject(argFrameNum) == nullptr) {
			continue;
		}
		
		Position objectPos = layers[i]->getObject(argFrameNum)->getPos(argFrameNum);
		Size objectSize = layers[i]->getObject(argFrameNum)->getSize(argFrameNum);
		
		if (objectPos.x <= cursorPosOnRenderTexture.x &&
			objectPos.y <= cursorPosOnRenderTexture.y &&
			objectPos.x+objectSize.x >= cursorPosOnRenderTexture.x &&
			objectPos.y+objectSize.y >= cursorPosOnRenderTexture.y) {
			
			clickedLayerNumber = i;
		}
	}
	
	return clickedLayerNumber;
}

Vec2 Video::cursorPosOnPreviewer() {
	// マウス座標をRenderTexture上の座標に変換
	Vec2 cursorPosOnRenderTexture = {
		(int)((Cursor::Pos().x-10)/(windowSize.x*0.75)*videoSize.x),
		(int)((Cursor::Pos().y-10)/(videoSize.y/(double)videoSize.x*windowSize.x*0.75)*videoSize.y)
	};
	
	return cursorPosOnRenderTexture;
}

Size Video::preview(int argFrameNum, Size argWindowSize) {
	windowSize = argWindowSize;
	
	if (argFrameNum != previewingFrameNum) {
		needToUpdatePreview = true;
		previewingFrameNum = argFrameNum;
	}
	
	Vec2 cursorPosOnRenderTexture = cursorPosOnPreviewer();
	if (cursorPosOnRenderTexture.x >= 0 && cursorPosOnRenderTexture.y >= 0 &&
		cursorPosOnRenderTexture.x <= videoSize.x && cursorPosOnRenderTexture.y <= videoSize.y) {
		
		int coo = cursorOnObject(argFrameNum, cursorPosOnRenderTexture);
		if (coo != layerUnderCursor) {
			needToUpdatePreview = true;
			layerUnderCursor = coo;
		}
		
		if (MouseL.down() && selectedObject != layerUnderCursor) {
			needToUpdatePreview = true;
			selectedObject = layerUnderCursor;
		}
	}
	else if (layerUnderCursor >= 0) {
		needToUpdatePreview = true;
		layerUnderCursor = -1;
	}
	
	// プレビューの更新
	if (needToUpdatePreview) {
		previewTexture = MSRenderTexture(videoSize.x, videoSize.y, Color(Palette::Black));
		ScopedRenderTarget2D target(previewTexture);
		
		// 各レイヤーの描画
		for (int i=0; i<layers.size(); i++) {
			Object* tempObject = layers[i]->getObject(previewingFrameNum);
			
			if (tempObject == nullptr) {
				continue;
			}
			
			tempObject->getTexture()->draw(tempObject->getPos(argFrameNum).x, tempObject->getPos(argFrameNum).y);
			
			if (selectedObject == i) {
				Rect(tempObject->getPos(argFrameNum).x, tempObject->getPos(argFrameNum).y,
				tempObject->getSize(argFrameNum).x, tempObject->getSize(argFrameNum).y).drawFrame(2, 2, Color(230, 126, 34));
			}
			else if (layerUnderCursor == i) {
				Rect(tempObject->getPos(argFrameNum).x-2, tempObject->getPos(argFrameNum).y-2,
					 tempObject->getSize(argFrameNum).x+4, tempObject->getSize(argFrameNum).y+4).draw(Color(0, 162, 232, 128));
			}
		}
		
		needToUpdatePreview = false;
	}
	
	// プレビュー部分の用意
	if (videoSize.x >= videoSize.y) {
		previewSize = {(int)(windowSize.x*0.75), (int)(videoSize.y/(double)videoSize.x*windowSize.x*0.75)};
	}
	else {
		previewSize = {(int)(videoSize.x/(double)videoSize.x*windowSize.x*0.42), (int)(windowSize.y*0.42)};
	}
	
	Graphics2D::Flush();
	previewTexture.resolve();
	previewTexture.resized(previewSize).draw(10, 10);
	
	return previewSize;
}

int Video::getTotalFrames() {
	return totalFrames;
}

void Video::encode(String argVideoFilePath) {
	videoFilePath = argVideoFilePath;
	videoWriter = cv::VideoWriter(videoFilePath.toUTF8(), cv::VideoWriter::fourcc('H','2','6','4'), 30.0, cv::Size(videoSize.x, videoSize.y));
	
	// frameMatに各オブジェクトを書き足す
	cv::Mat frameMat(videoSize.y, videoSize.x, CV_8UC3);
	Image frameImage(videoSize.x, videoSize.y);
	Object* tempObject;
	
	ScopedRenderTarget2D target(encordingTexture);
	
	for (int f=0; f<totalFrames; f++) {
		encordingTexture.clear(Color(0, 0));
		
		for (int l=0; l<layers.size(); l++) {
			// 各レイヤーの描画
			tempObject = layers[l]->getObject(f);
				
			if (tempObject == nullptr) {
				continue;
			}
				
			tempObject->getTexture()->draw(tempObject->getPos(f).x, tempObject->getPos(f).y);
		}
		
		Graphics2D::Flush();
		encordingTexture.resolve();
		encordingTexture.readAsImage(frameImage);
		
		imageToMat(frameMat, frameImage);
		
		videoWriter << frameMat;
	}
	
	videoWriter.release();
	
	/*
	// プレビューの更新
	
	previewTexture.clear(Color(0, 0));
		ScopedRenderTarget2D target(previewTexture);
	
		// 各レイヤーの描画
		for (int i=0; i<layers.size(); i++) {
			Object* tempObject = layers[i]->getObject(wroteFrames);
			
			if (tempObject == nullptr) {
				continue;
			}
			
			tempObject->getTexture()->draw(tempObject->getPos().x, tempObject->getPos().y);
		}
		
		// プレビュー部分の用意
		if (videoSize.x >= videoSize.y) {
			previewSize = {(int)(windowSize.x*0.75), (int)(videoSize.y/(double)videoSize.x*windowSize.x*0.75)};
		}
		else {
			previewSize = {(int)(videoSize.x/(double)videoSize.x*windowSize.x*0.42), (int)(windowSize.y*0.42)};
		}
	
		Graphics2D::Flush();
		previewTexture.resolve();
		previewTexture.resized(previewSize).draw(10, 10);
	
		//previewTexture.readAsImage(i);
		//videoWriter.writeFrame(i);
		
		cout << "書き出し中..." << wroteFrames << '/' << totalFrames << endl;
	
	if (totalFrames > wroteFrames) {
		wroteFrames ++;
		return true;
	}
	
	needToUpdatePreview = true;
	return false;
	*/
	/*
	videoFilePath = argVideoFilePath;
	videoWriter = VideoWriter(videoFilePath, videoSize);
	Font font16(48);
	Image i(videoSize.x, videoSize.y);
	
	// ビデオ用の各フレームを描画
	// 描画先: encordingTexture
	Object* tempObject;
	
	int t=0;
	encordingTexture = MSRenderTexture(videoSize.x, videoSize.y);
	while (System::Update() && t<totalFrames) {
	//	if (t % 2 == 0) {
			ScopedRenderTarget2D target(encordingTexture);
			encordingTexture.clear(Color(0, 0));
			
			for (int l=0; l<layers.size(); l++) {
				tempObject = layers[l]->getObject(t);
				
				if (tempObject == nullptr) {
					continue;
				}
				
				tempObject->getTexture()->draw(tempObject->getPos().x, tempObject->getPos().y);
			}
			
			//Graphics2D::Flush();
			//encordingTexture.resolve();
		//	encordingTexture.readAsImage(i);
		//	videoWriter.writeFrame(i);
		//}
		
		//if (t % 2 != 0) {
			//Graphics2D::Flush();
		//	encordingTexture.draw(0, 0);
		//}
	
		t++;
	}*/
}

void Video::close() {
	for (int i=0; i<layers.size(); i++) {
		free(layers[i]);
	}
}
