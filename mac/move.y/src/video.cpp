//
//  video.cpp
//  ablEditor
//
//  Created by YotioSoft on 2020/02/21.
//

#include "video.hpp"
#include "timeline.hpp"

Video::Video(Size argVideoSize) {
	videoSize = argVideoSize;
	
	previewTexture = MSRenderTexture(videoSize.x, videoSize.y, Color(Palette::Black));
	encordingTexture = MSRenderTexture(videoSize.x, videoSize.y, Color(Palette::Black));
	previewingFrameNum = 0;
	
	totalFrames = 0;
	wroteFrames = 0;
	
	fps = 30.0;
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
	
	updateSet(argObject->getFrameTime().first, argObject->getFrameTime().second);
	
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

void Video::selectObject(Object* argObjectP) {
	selectedObject = argObjectP;
	needToUpdatePreview = true;
}

void Video::linkToTimeline(TimeLine* argTimeLine) {
	timeline = argTimeLine;
}

Size Video::preview(int argFrameNum, Size argWindowSize) {
	windowSize = argWindowSize;
	
	//updateFrames();
	
	if (argFrameNum != previewingFrameNum) {
		needToUpdatePreview = true;
		previewingFrameNum = argFrameNum;
	}
	
	Vec2 cursorPosOnRenderTexture = cursorPosOnPreviewer();
	if (cursorPosOnRenderTexture.x >= 0 && cursorPosOnRenderTexture.y >= 0 &&
		cursorPosOnRenderTexture.x <= videoSize.x && cursorPosOnRenderTexture.y <= videoSize.y) {
		
		Object* obj;
		int coo = cursorOnObject(argFrameNum, cursorPosOnRenderTexture);
		if (coo >= 0 && coo < layers.size()) {
			obj = layers[cursorOnObject(argFrameNum, cursorPosOnRenderTexture)]->getObject(previewingFrameNum);
			if (obj != objectUnderCursor) {
				needToUpdatePreview = true;
				objectUnderCursor = obj;
			}
			
			if (MouseL.down()) {
				if (selectedObject != objectUnderCursor) {
					needToUpdatePreview = true;
					selectedObject = objectUnderCursor;
					
					if (timeline != nullptr) {
						timeline->selectObject(selectedObject);
					}
				}
			}
		}
		else {
			needToUpdatePreview = true;
			objectUnderCursor = nullptr;
			
			if (MouseL.down()) {
				needToUpdatePreview = true;
				selectedObject = nullptr;
				
				if (timeline != nullptr) {
					timeline->selectObject(selectedObject);
				}
			}
		}
	}
	else {
		needToUpdatePreview = true;
		objectUnderCursor = nullptr;
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
			
			if (selectedObject == tempObject) {
				Rect(tempObject->getPos(argFrameNum).x, tempObject->getPos(argFrameNum).y,
				tempObject->getSize(argFrameNum).x, tempObject->getSize(argFrameNum).y).drawFrame(2, 2, Color(230, 126, 34));
			}
			else if (objectUnderCursor == tempObject) {
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

double Video::getFPS() {
	return fps;
}

void Video::updateSet(int argStartFrame, int argEndFrame) {
	if (argStartFrame < writeToMatStartFrame || argEndFrame > writeToMatEndFrame) {
		writeToMatStartFrame = argStartFrame;
		writeToMatEndFrame = argEndFrame;
	}
	if (argStartFrame != writeToMatStartFrame || argEndFrame != writeToMatEndFrame) {
		wroteToMatFrames = writeToMatStartFrame;
	}
}

void Video::updateFrames() {
	if (wroteToMatFrames > writeToMatEndFrame) {
		return;
	}
	
	updateMat(wroteToMatFrames);
	wroteToMatFrames ++;
}

void Video::updateMat(int argFrameNum) {
	// frameMatに各オブジェクトを書き足す
	cv::Mat frameMat(videoSize.y, videoSize.x, CV_8UC3);
	Image frameImage(videoSize.x, videoSize.y);
	Object* tempObject;
	
	ScopedRenderTarget2D target(encordingTexture);
	//for (int f=argStartFrame; f<argEndFrame; f++) {
		encordingTexture.clear(Color(0, 0));
		matArray.push_back(new cv::Mat(videoSize.y, videoSize.x, CV_8UC3));
		
		for (int l=0; l<layers.size(); l++) {
			// 各レイヤーの描画
			tempObject = layers[l]->getObject(argFrameNum);
				
			if (tempObject == nullptr) {
				continue;
			}
				
			tempObject->getTexture()->draw(tempObject->getPos(argFrameNum).x, tempObject->getPos(argFrameNum).y);
			//addObjectToMat((*matArray[f]), *tempObject, f);
		}
		
		Graphics2D::Flush();
		encordingTexture.resolve();
		encordingTexture.readAsImage(frameImage);
		
		matArray.push_back(new cv::Mat(videoSize.y, videoSize.x, CV_8UC3));
		imageToMat((*matArray[argFrameNum]), frameImage);
		
		cout << "書き込み中: " << argFrameNum << "/" << totalFrames << " -> " << matArray[argFrameNum] << endl;
	//}
	cout << "Done!" << endl;
}

void Video::encode(String argVideoFilePath) {
	videoFilePath = argVideoFilePath;
	videoWriter = cv::VideoWriter(videoFilePath.toUTF8(), cv::VideoWriter::fourcc('H','2','6','4'), fps, cv::Size(videoSize.x, videoSize.y));
	
	
	//updateMat(0, 200);
	
	// Imageに各オブジェクトを書き足す
	Array<Image*> imageArray(1000, nullptr);
	
	ScopedRenderTarget2D target(encordingTexture);
	int imageNum = 0;
	
	Object* tempObject;
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
		imageArray[imageNum] = new Image(videoSize.x, videoSize.y);
		encordingTexture.readAsImage(*imageArray[imageNum]);
		
		imageNum ++;
		if (imageNum == 1000 || f+1 == totalFrames) {
			Array<cv::Mat*> frameMat(imageNum);
			
			for (int i=0; i<imageNum; i++) {
				frameMat[i] = new cv::Mat(videoSize.y, videoSize.x, CV_8UC3);
			}
			
			Range(0, imageNum-1).parallel_each([this, imageArray, frameMat](int32 i)
			{
				cout << "書き込み開始: " << i << endl;
				imageToMat(*frameMat[i], *imageArray[i]);
				cout << "  書き込み完了: " << i << endl;
			});
			
			for (int i=0; i<imageNum; i++) {
				videoWriter << *frameMat[i];
				
				delete(frameMat[i]);
				delete(imageArray[i]);
			}
			imageNum = 0;
		}
	}
	
	/*
	for (int f=0; f<=totalFrames; f++) {
		videoWriter << *(matArray[f]);
		cout << "出力中: " << f << "/" << totalFrames << endl;
	}
	 */
	videoWriter.release();
	
	//videoWriter.release();
	
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
	
	for (int i=0; i<matArray.size(); i++) {
		free(matArray[i]);
	}
}
