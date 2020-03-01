//
//  object.hpp
//  ablEditor
//
//  Created by YotioSoft on 2020/02/21.
//

#ifndef object_hpp
#define object_hpp

#include "header.h"
#include "rpn.hpp"

class Object {
public:
	// 各種オブジェクトのセット
	void setFont(Font& argFont, Color argColor, String argString, Position argObjectPos, int argStartFrameNum, int argEndFrameNum);
	void setRect(Rect& argRect, Color argColor, int argStartFrameNum, int argEndFrameNum);
	void setCircle(Circle &argCircle, Color argColor, int argStartFrameNum, int argEndFrameNum);
	
	// 数式のセット
	void setMoveX(String argFormula);		// オブジェクトの移動(x座標)
	void setMoveY(String argFormula);		// オブジェクトの移動(y座標)
	
	// オブジェクトのフレーム時間を取得
	pair<int, int> getFrameTime();
	
	// オブジェクトの位置を取得
	Position getPos(int argFrameNum);
	
	// オブジェクトのサイズを取得
	Size getSize();
	
	// イメージの取得
	const Image& getImage();
	
	// オブジェクトのTextureを取得
	DynamicTexture* getTexture();
	
	// オブジェクトの種類を返す
	object_type::Type getObjectType();
	
private:
	// 動画全体のサイズ
	Size videoSize;
	
	// オブジェクトの表示位置
	Position objectPos;
	
	// オブジェクトの色
	Color objectColor;
	
	// オブジェクトのImage
	Image objectImage;
	
	// オブジェクトのTexture
	DynamicTexture objectTexture;
	
	// オブジェクトの大きさ
	Size objectSize;
	
	// オブジェクト表示の開始時間と終了時間
	int objectBeginFrameNum;
	int objectEndFrameNum;
	
	// オブジェクトの種類
	object_type::Type objectType;
	
	// オブジェクトの各種類のポインタ
	StringObject* string;	// 文字列
	Rect* rect;				// 四角形
	Circle* circle;			// 円形
	
	// 数式
	pair<bool, ReversePolishNotation> moveRPNx;	// オブジェクト移動用(x座標)
	pair<bool, ReversePolishNotation> moveRPNy;	// オブジェクト移動用(y座標)
};

#endif /* object_hpp */
