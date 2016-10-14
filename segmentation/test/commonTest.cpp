#define BOOST_TEST_MODULE LocalResliceParticleSimCommon

#include <iostream>
#include <numeric>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_log.hpp>
#include <opencv2/core/core.hpp>
#include "segmentation/lrps/common.h"

using namespace volcart::segmentation;

static const double floatComparePercentTolerance = 0.01;  // %

////////////////////////////////////////////////////////////////////////////////
// zip() testing
// Note: explicitly not testing different sizes since an assert is generated.
// This is a programmer error, not a runtime error, so it doesn't make much
// sense to unit test
BOOST_AUTO_TEST_SUITE(Zip)

BOOST_AUTO_TEST_CASE(SizeEmpty)
{
    std::vector<int32_t> v1, v2;
    auto pairs = zip(v1, v2);
    BOOST_CHECK(pairs.empty());
}

BOOST_AUTO_TEST_CASE(SizeOne)
{
    std::vector<int32_t> v1{0};
    std::vector<int32_t> v2{0};
    auto pairs = zip(v1, v2);
    BOOST_CHECK_EQUAL(pairs.size(), 1);
    std::pair<int32_t, int32_t> zeroPair(0, 0);
    BOOST_CHECK_EQUAL(pairs[0].first, zeroPair.first);
    BOOST_CHECK_EQUAL(pairs[0].second, zeroPair.second);
}

BOOST_AUTO_TEST_CASE(DifferentTypes)
{
    std::vector<int32_t> v1{1};
    std::vector<double> v2{1.0};
    auto pairs = zip(v1, v2);
    BOOST_CHECK_EQUAL(pairs.size(), 1);
    std::pair<int32_t, double> mixedPair(1, 1);
    BOOST_CHECK_EQUAL(pairs[0].first, mixedPair.first);
    BOOST_CHECK_EQUAL(pairs[0].second, mixedPair.second);
}

BOOST_AUTO_TEST_SUITE_END()

////////////////////////////////////////////////////////////////////////////////
// unzip() testing
BOOST_AUTO_TEST_SUITE(Unzip)

BOOST_AUTO_TEST_CASE(SizeEmpty)
{
    std::vector<cv::Vec3d> vs;
    std::vector<double> xs, ys;
    std::tie(xs, ys) = unzip(vs);
    BOOST_CHECK(xs.empty());
    BOOST_CHECK(ys.empty());
}

BOOST_AUTO_TEST_CASE(SizeOne)
{
    std::vector<cv::Vec3d> vs{{0, 0, 0}};
    std::vector<double> xs, ys;
    std::tie(xs, ys) = unzip(vs);
    BOOST_CHECK_EQUAL(xs.size(), 1);
    BOOST_CHECK_EQUAL(ys.size(), 1);
    BOOST_CHECK_EQUAL(xs[0], 0);
    BOOST_CHECK_EQUAL(ys[0], 0);
}

BOOST_AUTO_TEST_CASE(SizeTwo)
{
    std::vector<cv::Vec3d> vs{{0, 0, 0}, {1, 1, 1}};
    std::vector<double> xs, ys;
    std::tie(xs, ys) = unzip(vs);
    BOOST_CHECK_EQUAL(xs.size(), vs.size());
    BOOST_CHECK_EQUAL(ys.size(), vs.size());
    for (size_t i = 0; i < vs.size(); ++i) {
        BOOST_CHECK_EQUAL(xs[i], vs[i](0));
        BOOST_CHECK_EQUAL(ys[i], vs[i](1));
    }
}

BOOST_AUTO_TEST_SUITE_END()

////////////////////////////////////////////////////////////////////////////////
// normalizeVector(built-in types) testing
BOOST_AUTO_TEST_SUITE(NormalizeVectorBuiltInTypes)

BOOST_AUTO_TEST_CASE(SizeEmpty)
{
    std::vector<int32_t> is;
    auto ds = normalizeVector(is);
    BOOST_CHECK(ds.empty());
}

BOOST_AUTO_TEST_CASE(SizeOneCheckNormalized)
{
    std::vector<int32_t> is{5};
    auto ds = normalizeVector(is);
    BOOST_CHECK_EQUAL(ds.size(), 1);
    BOOST_CHECK_EQUAL(ds[0], 1.0);
}

BOOST_AUTO_TEST_CASE(IntegersZeroToNineDefaultMinAndMax)
{
    std::vector<int32_t> is(10);
    std::iota(std::begin(is), std::end(is), 0);
    auto ds = normalizeVector(is);
    BOOST_CHECK_EQUAL(ds.size(), is.size());

    // sum + delta generates correct values as verified from python library
    // sklearn.processing.MinMaxScaler
    double sum = 0;
    double delta = 0.1111111111111111111;
    for (const auto e : ds) {
        BOOST_CHECK_CLOSE(e, sum, floatComparePercentTolerance);
        sum += delta;
    }
}

BOOST_AUTO_TEST_CASE(IntegersZeroToNineShiftRangeByFive)
{
    std::vector<int32_t> is(10);
    std::iota(std::begin(is), std::end(is), 0);
    auto newMin = double(is.front() + 5);
    auto newMax = double(is.back() + 5);
    auto ds = normalizeVector(is, newMin, newMax);
    BOOST_CHECK_EQUAL(ds.size(), is.size());
    double baseVal = newMin;
    for (const auto e : ds) {
        BOOST_CHECK_CLOSE(e, baseVal, floatComparePercentTolerance);
        baseVal += 1;
    }
}

BOOST_AUTO_TEST_CASE(IntegersZeroToNineCompressRangeByHalf)
{
    std::vector<int32_t> is(10);
    std::iota(std::begin(is), std::end(is), 0);
    auto newMin = double(is.front());
    auto newMax = is.back() / 2.0;
    auto ds = normalizeVector(is, newMin, newMax);
    BOOST_CHECK_EQUAL(ds.size(), is.size());
    double baseVal = newMin;
    for (const auto e : ds) {
        BOOST_CHECK_CLOSE(e, baseVal, floatComparePercentTolerance);
        baseVal += 0.5;
    }
}

BOOST_AUTO_TEST_SUITE_END()

////////////////////////////////////////////////////////////////////////////////
// normalizeVector(cv::Vec<T, Len>) overload testing
// XXX Should more tests be added? This function works on a per-element basis,
// not across the entire vector. If it works for one element, it should work for
// all elements
BOOST_AUTO_TEST_SUITE(NormalizeVectorCvVec)

BOOST_AUTO_TEST_CASE(SizeEmpty)
{
    std::vector<cv::Vec3i> is;
    auto ds = normalizeVector(is);
    BOOST_CHECK(ds.empty());
}

BOOST_AUTO_TEST_CASE(SizeOneCheckNormalized)
{
    std::vector<cv::Vec3i> is{{1, 2, 3}};

    // Computed via Python
    cv::Vec3d expectedResult{0.26726124191, 0.5345224838, 0.801783725737};

    auto ds = normalizeVector(is);
    BOOST_CHECK_EQUAL(ds.size(), 1);
    for (int32_t i = 0; i < 3; ++i) {
        BOOST_CHECK_CLOSE(
            ds.front()(i), expectedResult(i), floatComparePercentTolerance);
    }

    // Check that the resulting vector norm is 1
    BOOST_CHECK_CLOSE(cv::norm(ds.front()), 1.0, floatComparePercentTolerance);
}

BOOST_AUTO_TEST_SUITE_END()

////////////////////////////////////////////////////////////////////////////////
// squareDiff() testing
BOOST_AUTO_TEST_SUITE(SquareDiff)

BOOST_AUTO_TEST_CASE(SizeEmpty)
{
    std::vector<cv::Vec3d> v1, v2;
    auto testResult = squareDiff(v1, v2);
    BOOST_CHECK(testResult.empty());
}

BOOST_AUTO_TEST_CASE(SizeOne)
{
    std::vector<cv::Vec3d> v1{{0, 0, 0}}, v2{{1, 1, 1}};
    auto testResult = squareDiff(v1, v2);
    BOOST_CHECK_EQUAL(testResult.size(), 1);
    double expectedResult = std::sqrt(3);
    BOOST_CHECK_CLOSE(
        testResult[0], expectedResult, floatComparePercentTolerance);
}

BOOST_AUTO_TEST_SUITE_END()

////////////////////////////////////////////////////////////////////////////////
// sumSquareDiff() testing
BOOST_AUTO_TEST_SUITE(SumSquareDiff)

BOOST_AUTO_TEST_CASE(SizeEmpty)
{
    std::vector<double> v1, v2;
    auto testResult = sumSquareDiff(v1, v2);
    BOOST_CHECK_EQUAL(testResult, 0.0);
}

BOOST_AUTO_TEST_CASE(SizeOne)
{
    std::vector<double> v1{0}, v2{1};
    double testResult = sumSquareDiff(v1, v2);
    double expectedResult = 1.0;
    BOOST_CHECK_CLOSE(testResult, expectedResult, floatComparePercentTolerance);
}

BOOST_AUTO_TEST_CASE(SizeTwo)
{
    std::vector<double> v1{0, 1}, v2{1, 0};
    double testResult = sumSquareDiff(v1, v2);
    double expectedResult = std::sqrt(2);
    BOOST_CHECK_CLOSE(testResult, expectedResult, floatComparePercentTolerance);
}

BOOST_AUTO_TEST_SUITE_END()
