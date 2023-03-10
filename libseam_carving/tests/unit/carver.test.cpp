//
// Created by Andy Mina on 3/7/23.
//

// project
#include <seam_carving/carver.hpp>
#include <seam_carving/tests/test_data.hpp>
#include <seam_carving/tests/carver_data.hpp>

// 3rd party
#include <gtest/gtest.h>

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

}


