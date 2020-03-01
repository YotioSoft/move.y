#include "header.h"
#include "object.hpp"
#include "video.hpp"
#include "converter.hpp"
#include "rpn.hpp"

void Main() {
	ReversePolishNotation rpn;
	rpn.set(U"2*3");
	
    Scene::SetBackground(Color(Palette::Snow));
	
	Size videoSize = Size(1920, 1080);
	Video video(videoSize, Scene::Size());
	
	Rect rectBack(0, 0, videoSize.x, videoSize.y);
	Object back1;
	back1.setRect(rectBack, Color(Palette::Darkgray), 50, 80);
	Object back2;
	back2.setRect(rectBack, Color(Palette::Blue), 81, 110);
	Object back3;
	back3.setRect(rectBack, Color(Palette::Darkgreen), 111, 140);
	Object back4;
	back4.setRect(rectBack, Color(Palette::Orange), 141, 170);
	Object back5;
	back5.setRect(rectBack, Color(Palette::Snow), 171, 200);
	video.addLayer();
	video.addObjectToLayer(0, back1);
	video.addObjectToLayer(0, back2);
	video.addObjectToLayer(0, back3);
	video.addObjectToLayer(0, back4);
	video.addObjectToLayer(0, back5);

    Font font(36);
	Object mobj;
	mobj.setFont(font, Color(255, 255, 255), U"Hello World!", {10, 10}, 0, 99);
	Object mobj1;
	mobj1.setFont(font, Color(255, 255, 255), U"OpenSiv3D + OpenCV4で動画書き出ししてみるテスト中", {10, 10}, 100, 170);
	Object mobj2;
	mobj2.setFont(font, Color(0, 0, 0), U"OpenSiv3D + OpenCV4で動画書き出ししてみるテスト中", {10, 10}, 171, 200);
	video.addLayer();
	video.addObjectToLayer(1, mobj);
	video.addObjectToLayer(1, mobj1);
	video.addObjectToLayer(1, mobj2);
	
	Rect rect1(100, 100, 100, 100);
	Object robj1;
	robj1.setRect(rect1, Color(Palette::Red), 0, 200);
	video.addLayer();
	video.addObjectToLayer(2, robj1);
	
	Rect rect2(150, 150, 100, 100);
	Object robj2;
	robj2.setRect(rect2, Color(Palette::Green), 0, 200);
	video.addLayer();
	video.addObjectToLayer(3, robj2);
	
	Font font24(24);
	Object aboutobj;
	aboutobj.setFont(font24, Color(255, 255, 255), U"About\nFile Name : output.mp4\nSize : 1920x1080\nTotal Frames : 200\nFPS : 30\nFormat : H264", {1400, 700}, 0, 170);
	Object aboutobj1;
	aboutobj1.setFont(font24, Color(0, 0, 0), U"About\nFile Name : output.mp4\nSize : 1920x1080\nTotal Frames : 200\nFPS : 30\nFormat : H264", {1400, 700}, 171, 200);
	video.addLayer();
	video.addObjectToLayer(4, aboutobj);
	video.addObjectToLayer(4, aboutobj1);
	
	/*
	for (i=0; i<1; i++) {
		Object mobj2;
		mobj2.setFont(font, Color(255, 255, 255), U"Frame Number : 0", {10, 1000}, 0, 100);
		video.addObjectToLayer(3, mobj2);
	}*/
	
	double previewFrameTmp = 0;
	int previewFrame = 0;
	bool writing = false;
	
	Font font16(16);

    while (System::Update()) {
		video.preview(previewFrame);
		
		if (writing) {
			cout << "書き出し中..." << endl;
			video.encode(U"videos/output.mp4");
			cout << "書き出し完了" << endl;
			
			writing = false;
		}
		if (SimpleGUI::Button(U"Write", Vec2(Scene::Size().x-150, Scene::Size().y-100)))
        {
			writing = true;
			font16(U"書き出し中").draw(Scene::Size().x-150, Scene::Size().y-50, Color(Palette::Black));
        }
		
		SimpleGUI::Slider(U"Frame {:d}"_fmt(previewFrame = (int)(previewFrameTmp*video.getTotalFrames())), previewFrameTmp, Vec2(10, Scene::Size().y-100), 200, 200);
	}
}
