#pragma once

#define DEFAULT_OFFSET -1

#include <opencv2/opencv.hpp>
#include <pcl/io/pcd_io.h>
#include <pcl/common/common.h>
#include <pcl/point_types.h>

#include "field.h"
#include "particle.h"

// this is similar to the chain class in structureTensor
// it tries to find the next position of the particles
// based on slices orthogonal to the chain

namespace volcart {

namespace segmentation {

class Chain {
public:
    Chain(pcl::PointCloud<pcl::PointXYZRGB>::Ptr, VolumePkg&, int, int, int=1);

    void step(Field&);

    bool hasMovingParticle();

    void debug(bool=false);

    void drawChainOnSlice(std::vector<Particle>);

    pcl::PointCloud<pcl::PointXYZRGB> orderedPCD();

private:
    // History of the chain at each iteration
    std::list<std::vector<Particle>> _history;
    VolumePkg&_volpkg;
    int _updateCount;

    // "reslicing" happens when we update the normals
    // we have to reslice every iteration regardless
    // since offets are from the center of the slice
    int _stepsBeforeReslice;

    // -- Chain Size Information -- //
    uint64_t _chainLength; // Number of particles in the chain & width of output PCD
    uint32_t _realIterationsCount; // Height of the output PCD To-Do: Do we need this?
    uint32_t _startIdx; // Starting slice index
    int32_t _targetIdx; // Target slice index
    int32_t _stepSize; // To-Do: What is this for now? We may not need this.

    cv::Vec3f calculateNormal(uint64_t, std::vector<Particle>);
};

}

}
