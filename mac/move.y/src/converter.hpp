//
//  converter.hpp
//  movey
//
//  Created by YotioSoft on 2020/03/01.
//

#ifndef converter_hpp
#define converter_hpp

#include "header.h"
#include "object.hpp"

void imageToMat(cv::Mat& argMat, Image& argImage);

void addObjectToMat(cv::Mat& argTo, Object& argFrom, int argFrameNum);

#endif /* converter_hpp */
