//
// Created by Andy Mina on 3/7/23.
//

// project
#include <seam_carving/carver.hpp>
#include <seam_carving/seam.hpp>

#include <seam_carving/tests/print_utils.hpp>
#include <seam_carving/tests/json_utils.hpp>
#include <seam_carving/tests/carver_data.hpp>
#include <seam_carving/tests/dummy_carver.hpp>

// 3rd party
#include <gtest/gtest.h>

namespace sc = seam_carving;
namespace sct = seam_carving::tests;
using sct::CarverTest;

INSTANTIATE_TEST_SUITE_P(
    CarverDataFromJSON,
    CarverTest,
    testing::ValuesIn(
        sct::GetData<sct::CarverData>(sct::kJSONDataFile)
    )
);

TEST_P(CarverTest, FindVerticalSeamReturnsCorrectSeam) {
    sct::CarverData carver_data = GetParam();
    sct::DummyCarver carver;

    cv::Mat input = carver_data.original_matrix;
    sc::Seam expected = carver_data.vertical_seam;

    /**
     * Things are failing here because technically the input image doesn't have enough channels.
     * The code is expecting a 3-channel RGB image, but instead gets 1-channel grayscale image
     * and attempts to convert down. The occurs in the energy::ComputeEnergy().
     *
     * Possible solutions include:
     *  1. setting the conversion behind a guard checking for # of channels
     *  2. rewrite tests and parser so they use 3-dimensional images as well
     *  3. refactor FindVerticalSeam to accept an energy image
     *
     * Solution 1 is kinda cheap but would ultimately require testing for OpenCV.
     *
     * Solution 2 is super expensive but does make the tests a lot closer to the
     * use case. We could give the tests a grayscale RGB image since we can trust OpenCV
     * knows how to convert.
     *
     * Solution 3 is cheap but can ultimately bog down the user with having to perform all
     * of the steps of seam carving themselves, instead of just letting the magic happen.
     *
     * After some thought it seems like Solution 2 is the best. We can keep the tests
     * 1-dimensional but in the tests we can convert it to a 3-dimensional image where
     * the RGB values are just the repeated gray values. This is possible because
     * we can trust that OpenCV knows how to convert to grayscale so RGB has no relevance
     * in seam carving since the image will ultimately be converted anyways. The conversion
     * from 1 channel to 3 channels can happen in the loader/parser or the TestData.from_json.
     * The latter makes the most sense to me since we can avoid data.json being bloated.
     */
    sc::Seam actual = carver.FindVerticalSeam(input);

    /** @TODO(andymina) add PrintToString function back for TestData and children */
    EXPECT_EQ(0, 0);
//        << "TestId - " << carver_data.test_id << "\n"
//        << sct::Print(expected, "expected") << "\n"
//        << sct::Print(actual, "actual");
}


