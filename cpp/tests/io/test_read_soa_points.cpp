/*
 * Copyright (c) 2020, NVIDIA CORPORATION.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <tests/utilities/base_fixture.hpp>
#include <tests/utilities/column_utilities.hpp>
#include <tests/utilities/column_wrapper.hpp>
#include <tests/utilities/cudf_gtest.hpp>
#include <tests/utilities/type_lists.hpp>
#include <cuspatial/error.hpp>
#include <cuspatial/soa_readers.hpp>
#include "utility/legacy/utility.hpp"

using namespace cudf::test;

template <typename T>
struct INT64Test : public BaseFixture {};

using TestTypesInt64 = Types<int64_t>;
TYPED_TEST_CASE(INT64Test, TestTypesInt64);

template <typename T>
struct INTTest : public BaseFixture {};

using TestTypesInt32 = Types<int32_t>;
TYPED_TEST_CASE(INTTest, TestTypesInt32);

template <typename T>
struct POLYGONSOATest : public BaseFixture {};

using TestTypesPolygonSoa = Types<double>;
TYPED_TEST_CASE(POLYGONSOATest, TestTypesPolygonSoa);

TYPED_TEST(INT64Test, Empty)
{
    using T = TypeParam;

    TempDirTestEnvironment* const temp_env = static_cast<TempDirTestEnvironment*>(
        ::testing::AddGlobalTestEnvironment(new TempDirTestEnvironment));
    temp_env->SetUp();

    auto write_column = fixed_width_column_wrapper<T>({});
    auto to_host_result = to_host<T>(write_column).first;
    std::vector<T> h_write_column(to_host_result.size());
    thrust::copy(to_host_result.begin(), to_host_result.end(),
        h_write_column.begin());

    size_t write_result = cuspatial::detail::write_field_from_vec(
        temp_env->get_temp_filepath("soa_its.tmp").c_str(), h_write_column);
    CUSPATIAL_EXPECTS(write_result==h_write_column.size(), "Wrote an empty vec");

    auto read_result = cuspatial::experimental::read_timestamp_soa(
        temp_env->get_temp_filepath("soa_its.tmp").c_str()
    );

	expect_columns_equal(read_result->view(), write_column, true);
}


TYPED_TEST(INT64Test, Single)
{
    using T = TypeParam;

    TempDirTestEnvironment* const temp_env = static_cast<TempDirTestEnvironment*>(
        ::testing::AddGlobalTestEnvironment(new TempDirTestEnvironment));
    temp_env->SetUp();

    auto write_column = fixed_width_column_wrapper<T>({0});
    auto to_host_result = to_host<T>(write_column).first;
    std::vector<T> h_write_column(to_host_result.size());
    thrust::copy(to_host_result.begin(), to_host_result.end(),
        h_write_column.begin());

    size_t write_result = cuspatial::detail::write_field_from_vec(
        temp_env->get_temp_filepath("soa_its.tmp").c_str(), h_write_column);
    CUSPATIAL_EXPECTS(write_result==h_write_column.size(), "Wrote an empty vec");

    auto read_result = cuspatial::experimental::read_timestamp_soa(
        temp_env->get_temp_filepath("soa_its.tmp").c_str()
    );

	expect_columns_equal(read_result->view(), write_column, true);
}

TYPED_TEST(INT64Test, Triple)
{
    using T = TypeParam;

    TempDirTestEnvironment* const temp_env = static_cast<TempDirTestEnvironment*>(
        ::testing::AddGlobalTestEnvironment(new TempDirTestEnvironment));
    temp_env->SetUp();

    auto write_column = fixed_width_column_wrapper<T>({0, 1, 2});
    auto to_host_result = to_host<T>(write_column).first;
    std::vector<T> h_write_column(to_host_result.size());
    thrust::copy(to_host_result.begin(), to_host_result.end(),
        h_write_column.begin());

    size_t write_result = cuspatial::detail::write_field_from_vec(
        temp_env->get_temp_filepath("soa_its.tmp").c_str(), h_write_column);
    CUSPATIAL_EXPECTS(write_result==h_write_column.size(), "Wrote an empty vec");

    auto read_result = cuspatial::experimental::read_timestamp_soa(
        temp_env->get_temp_filepath("soa_its.tmp").c_str()
    );

	expect_columns_equal(read_result->view(), write_column, true);
}

TYPED_TEST(INT64Test, Negative)
{
    using T = TypeParam;

    TempDirTestEnvironment* const temp_env = static_cast<TempDirTestEnvironment*>(
        ::testing::AddGlobalTestEnvironment(new TempDirTestEnvironment));
    temp_env->SetUp();

    auto write_column = fixed_width_column_wrapper<T>({-1, -2});
    auto to_host_result = to_host<T>(write_column).first;
    std::vector<T> h_write_column(to_host_result.size());
    thrust::copy(to_host_result.begin(), to_host_result.end(),
        h_write_column.begin());

    size_t write_result = cuspatial::detail::write_field_from_vec(
        temp_env->get_temp_filepath("soa_its.tmp").c_str(), h_write_column);
    CUSPATIAL_EXPECTS(write_result==h_write_column.size(), "Wrote an empty vec");

    auto read_result = cuspatial::experimental::read_timestamp_soa(
        temp_env->get_temp_filepath("soa_its.tmp").c_str()
    );

	expect_columns_equal(read_result->view(), write_column, true);
}

TYPED_TEST(INTTest, EmptyUint32)
{
    using T = TypeParam;

    TempDirTestEnvironment* const temp_env = static_cast<TempDirTestEnvironment*>(
        ::testing::AddGlobalTestEnvironment(new TempDirTestEnvironment));
    temp_env->SetUp();

    auto write_column = fixed_width_column_wrapper<T>({});
    auto to_host_result = to_host<T>(write_column).first;
    std::vector<T> h_write_column(to_host_result.size());
    thrust::copy(to_host_result.begin(), to_host_result.end(),
        h_write_column.begin());

    size_t write_result = cuspatial::detail::write_field_from_vec(
        temp_env->get_temp_filepath("soa_int32.tmp").c_str(), h_write_column);
    CUSPATIAL_EXPECTS(write_result==h_write_column.size(), "Wrote an empty vec");

    auto read_result = cuspatial::experimental::read_int32_soa(
        temp_env->get_temp_filepath("soa_int32.tmp").c_str()
    );

	expect_columns_equal(read_result->view(), write_column, true);
}


TYPED_TEST(INTTest, SingleUint32)
{
    using T = TypeParam;

    TempDirTestEnvironment* const temp_env = static_cast<TempDirTestEnvironment*>(
        ::testing::AddGlobalTestEnvironment(new TempDirTestEnvironment));
    temp_env->SetUp();

    auto write_column = fixed_width_column_wrapper<T>({0});
    auto to_host_result = to_host<T>(write_column).first;
    std::vector<T> h_write_column(to_host_result.size());
    thrust::copy(to_host_result.begin(), to_host_result.end(),
        h_write_column.begin());

    size_t write_result = cuspatial::detail::write_field_from_vec(
        temp_env->get_temp_filepath("soa_int32.tmp").c_str(), h_write_column);
    CUSPATIAL_EXPECTS(write_result==h_write_column.size(), "Wrote an empty vec");

    auto read_result = cuspatial::experimental::read_int32_soa(
        temp_env->get_temp_filepath("soa_int32.tmp").c_str()
    );
	expect_columns_equal(read_result->view(), write_column, true);
}

TYPED_TEST(INTTest, TripleUint32)
{
    using T = TypeParam;

    TempDirTestEnvironment* const temp_env = static_cast<TempDirTestEnvironment*>(
        ::testing::AddGlobalTestEnvironment(new TempDirTestEnvironment));
    temp_env->SetUp();

    auto write_column = fixed_width_column_wrapper<T>({0, 1, 2});
    auto to_host_result = to_host<T>(write_column).first;
    std::vector<T> h_write_column(to_host_result.size());
    thrust::copy(to_host_result.begin(), to_host_result.end(),
        h_write_column.begin());

    size_t write_result = cuspatial::detail::write_field_from_vec(
        temp_env->get_temp_filepath("soa_int32.tmp").c_str(), h_write_column);
    CUSPATIAL_EXPECTS(write_result==h_write_column.size(), "Wrote an empty vec");

    auto read_result = cuspatial::experimental::read_int32_soa(
        temp_env->get_temp_filepath("soa_int32.tmp").c_str()
    );

	expect_columns_equal(read_result->view(), write_column, true);
}

TYPED_TEST(INTTest, NegativeUint32)
{
    using T = TypeParam;

    TempDirTestEnvironment* const temp_env = static_cast<TempDirTestEnvironment*>(
        ::testing::AddGlobalTestEnvironment(new TempDirTestEnvironment));
    temp_env->SetUp();

    auto write_column = fixed_width_column_wrapper<T>({-1, -2});
    auto to_host_result = to_host<T>(write_column).first;
    std::vector<T> h_write_column(to_host_result.size());
    thrust::copy(to_host_result.begin(), to_host_result.end(),
        h_write_column.begin());

    size_t write_result = cuspatial::detail::write_field_from_vec(
        temp_env->get_temp_filepath("soa_int32.tmp").c_str(), h_write_column);
    CUSPATIAL_EXPECTS(write_result==h_write_column.size(), "Wrote an empty vec");

    auto read_result = cuspatial::experimental::read_int32_soa(
        temp_env->get_temp_filepath("soa_int32.tmp").c_str()
    );

	expect_columns_equal(read_result->view(), write_column, true);
}

// TODO:
// Test read_points_lonlat
// Test read_points_xy

TYPED_TEST(POLYGONSOATest, PolygonSoaTest)
{
    using T = TypeParam;

    TempDirTestEnvironment* const temp_env = static_cast<TempDirTestEnvironment*>(
        ::testing::AddGlobalTestEnvironment(new TempDirTestEnvironment));
    temp_env->SetUp();

    uint32_t length[] {4};
    T points[] {1.0, 2.0, 3.0, 4.0};

    // create a polygon
    struct cuspatial::detail::polygons<T> polygon;
    polygon.num_group = 1;
    polygon.num_feature = 1;
    polygon.num_ring = 1;
    polygon.num_vertex = 1;
    polygon.group_length = length;
    polygon.feature_length = length;
    polygon.ring_length = length;
    polygon.x = points;
    polygon.y = points;

    // write polygon
    cuspatial::detail::write_polygon_soa<T>(
        temp_env->get_temp_filepath("soa_polygons.tmp"), &polygon);

    // read polygon
    struct cuspatial::detail::polygons<T> new_poly;
    cuspatial::detail::read_polygon_soa<T>(
        temp_env->get_temp_filepath("soa_polygons.tmp").c_str(), &new_poly);

    // validate read polygon is same as written polygon
    CUSPATIAL_EXPECTS(polygon.num_group == new_poly.num_group,
        "Number of groups inequal");
}
