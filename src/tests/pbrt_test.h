#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_TESTS_PBRT_TEST_CPP_
#define PBRT_SRC_TESTS_PBRT_TEST_CPP_

#include <optional>
#include <string>
#include <stdio.h>
#include <string.h>

#include "tests/gtest/gtest.h"
#include "pbrt.h"
#include <map>

namespace pbrt {

static std::map<std::string, std::vector<std::shared_ptr<Primitive>>> PSCache;

bool loadModel(std::vector<std::shared_ptr<Primitive>> &ps, const std::string &path);
void generateTestRays(std::vector<Ray> &rays, int size = 10000);
void printTime(const std::string &prefix, const std::chrono::milliseconds &begin, const std::chrono::milliseconds &end);

} // namespace pbrt




#endif // PBRT_SRC_TESTS_PBRT_TEST_CPP_