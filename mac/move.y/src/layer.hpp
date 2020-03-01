//
//  layer.hpp
//  ablEditor
//
//  Created by YotioSoft on 2020/02/21.
//

#ifndef layer_hpp
#define layer_hpp

#include "header.h"
#include "object.hpp"

class Layer {
public:
	bool addObject(Object& argObject);
	
	Object* getObject(int argFrameNum);
	
	int maxFrameNum();
	
private:
	Array<Object*> objects;
};

#endif /* layer_hpp */
