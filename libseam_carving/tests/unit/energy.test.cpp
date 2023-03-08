//
// Created by Andy Mina on 3/7/23.
//

// project
#include <seam_carving/energy.hpp>
#include <seam_carving/tests/energy_data.hpp>
#include <seam_carving/tests/test_data.hpp>

// 3rd party
#include <gtest/gtest.h>

namespace sct = seam_carving::tests;
using sct::EnergyTest;

INSTANTIATE_TEST_SUITE_P(
    EnergyFromJSON,
    EnergyTest,
    testing::ValuesIn(
        sct::GetData<sct::EnergyData>(sct::json_data_file)
    )
);
