#define BOOST_TEST_MODULE OrderedPointSetIOTest

#include <iostream>

#include <boost/test/unit_test.hpp>
#include <opencv2/core.hpp>

#include "core/io/PointSetIO.h"
#include "core/types/Exceptions.h"
#include "core/types/OrderedPointSet.h"

using namespace volcart;

struct Vec3iOrderedPointSet {
    OrderedPointSet<cv::Vec3i> ps;

    Vec3iOrderedPointSet() : ps{3}
    {
        ps.pushRow({{1, 1, 1}, {2, 2, 2}, {3, 3, 3}});
    }
};

BOOST_FIXTURE_TEST_CASE(
    WriteThenReadBinaryOrderedPointSet, Vec3iOrderedPointSet)
{
    // Binary IO is default
    PointSetIO<cv::Vec3i>::WriteOrderedPointSet("binary.vcps", ps);
    auto readPs = PointSetIO<cv::Vec3i>::ReadOrderedPointSet("binary.vcps");
    BOOST_CHECK_EQUAL(readPs(0, 0), ps(0, 0));
    BOOST_CHECK_EQUAL(readPs(1, 0), ps(1, 0));
    BOOST_CHECK_EQUAL(readPs(2, 0), ps(2, 0));
}

BOOST_FIXTURE_TEST_CASE(WriteThenReadAsciiOrderedPointSet, Vec3iOrderedPointSet)
{
    PointSetIO<cv::Vec3i>::WriteOrderedPointSet(
        "ascii.vcps", ps, IOMode::ASCII);
    auto readPs =
        PointSetIO<cv::Vec3i>::ReadOrderedPointSet("ascii.vcps", IOMode::ASCII);
    BOOST_CHECK_EQUAL(readPs(0, 0), ps(0, 0));
    BOOST_CHECK_EQUAL(readPs(1, 0), ps(1, 0));
    BOOST_CHECK_EQUAL(readPs(2, 0), ps(2, 0));
}
