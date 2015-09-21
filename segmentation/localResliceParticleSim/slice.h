#pragma once

#include <opencv2/opencv.hpp>

namespace volcart {

namespace segmentation {

// defines for uniform indexing
// should be moved somewhere more global
#define VC_INDEX_X 0
#define VC_INDEX_Y 1
#define VC_INDEX_Z 2

#define VC_DIRECTION_I cv::Vec3f(1,0,0)
#define VC_DIRECTION_J cv::Vec3f(0,1,0)
#define VC_DIRECTION_K cv::Vec3f(0,0,1)

// slice class used for keeping track of 2d sections
// in 3d space

#define DEBUG_DRAW_NONE                    0
#define DEBUG_DRAW_CENTER             1 << 0
#define DEBUG_DRAW_XYZ                1 << 1
#define DEBUG_DRAW_CORNER_COORDINATES 1 << 2

#define DEBUG_DRAW_ALL DEBUG_DRAW_CENTER | DEBUG_DRAW_XYZ | DEBUG_DRAW_CORNER_COORDINATES

class Slice {
public:
    Slice(cv::Mat, cv::Vec3f, cv::Vec3f, cv::Vec3f, cv::Vec3f);

    cv::Vec3f findNextPosition();

    void debugDraw(int = DEBUG_DRAW_NONE);

    cv::Mat mat();

    void drawSliceAndCenter();

private:
    cv::Mat _slice;
    cv::Vec3f _origin;
    cv::Vec3f _center;
    cv::Vec3f _xvec;
    cv::Vec3f _yvec;
};

}

}