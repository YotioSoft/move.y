//
//  video.hpp
//  ablEditor
//
//  Created by YotioSoft on 2020/02/21.
//

#ifndef video_hpp
#define video_hpp

#include "header.h"
#include "layer.hpp"
#include "object.hpp"
#include "converter.hpp"

class Video {
public:
	// コンストラクタ
	Video(Size argVideoSize, Size argWindowSize);
	
	// レイヤーを追加する
	int addLayer();
	
	// レイヤーのポインタを返す
	Array<Layer*> getLayers();
	
	// オブジェクトをレイヤー追加する
	bool addObjectToLayer(int argLayerNum, Object& argObject);
	
	// プレビュー部分の表示
	Size preview(int argFrameNum);
	
	// フレームの総数を返す
	int getTotalFrames();
	
	// ビデオ出力
	void encode(String argVideoFilePath);
	
	// 終了処理
	void close();
	
private:
	// ビデオファイルのファイルパス
	String videoFilePath;
	
	// ビデオの大きさ
	Size videoSize;
	
	// VideoWriter
	//VideoWriter videoWriter;
	cv::VideoWriter videoWriter;
	
	// 各レイヤーのポインタ
	Array<Layer*> layers;
	
	// ウィンドウの大きさ
	Size windowSize;
	
	// プレビュー部分の大きさ
	Size previewSize;
	
	// プレビュー部分
	MSRenderTexture previewTexture;
	
	// 書き出し用
	MSRenderTexture encordingTexture;
	
	// プレビュー部分の表示フレーム
	int previewingFrameNum;
	
	// プレビュー部分の更新の有無
	bool needToUpdatePreview;
	
	// 総フレーム数
	int totalFrames;
	
	// 書き出し済みのフレーム数
	int wroteFrames;
};

#endif /* video_hpp */


/*
 #include <opencv2/opencv.hpp>

 int main(int argc, char** argv) {
	 //入力動画ファイルを指定
	 cv::VideoCapture cap( "input.mp4" );
	 //出力動画ファイルの指定
	 cv::VideoWriter writer( "output.mp4", cv::VideoWriter::fourcc('H','2','6','4'), cap.get(cv::CAP_PROP_FPS), cv::Size(1920, 1080));
	 
	 cv::Mat	image(1080, 1920, CV_8UC3);
	 
	 // 画像を白色で塗りつぶす
	 // * Scalar(Blue, Green, Red)  RGB順ではないことに注意
	 image = cv::Scalar(127, 127, 127);
	 
	 cv::rectangle(image, cv::Rect(10,  60, 100, 100), cv::Scalar(255, 255, 255),  1, cv::LINE_4);

	 cv::Mat frame;
	 while( 1 ) {
		 //1フレーム読み込み
		 cap >> frame;
		 if( frame.empty() )
			 break;

		 //出力動画ファイルへ書き込み
		 writer << image;
	 }
	 return 0;
 }
 */
