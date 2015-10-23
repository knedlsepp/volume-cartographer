#include "particle.h"

using namespace volcart::segmentation;

Particle::Particle(cv::Vec3f position) : position_(position), isStopped_(false) {
}

Particle::Particle(float x, float y, float z) : isStopped_(false) {
    position_ = cv::Vec3f(x, y, z);
}

// Position in 3D space (Slice, X, Y)
cv::Vec3f Particle::position() const {
    return position_;
}

// Returns true if particle is stopped
bool Particle::isMoving() const {
    return !isStopped_;
}

// Sets particle as being stopped
void Particle::stop() {
    isStopped_ = true;
}

Particle& Particle::operator+=(const Particle& rhs) const {
    position_ += rhs.position();
    return *this;
}

Particle operator+(Particle lhs, const Particle& rhs) {
    lhs += rhs;
    return lhs;
}

Particle& Particle::operator-=(const Particle& rhs) const {
    position_ -= rhs.position();
    return *this;
}

Particle operator-(Particle lhs, const Particle& rhs) {
    lhs -= rhs;
    return lhs;
}

float Particle::operator()(int index) const {
    return position_(index);
}
