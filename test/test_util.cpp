#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <simulation/util.hpp>

#include <tuple>

TEST_CASE("for_each_in_line", "[util]") {
  auto[x, y, dx, dy, expected]= GENERATE(
      std::tuple{0, 0, 5, 5, std::vector<std::tuple<int, int>>{
          {1, 1},
          {2, 2},
          {3, 3},
          {4, 4},
          {5, 5},
      }},
      std::tuple{0, 0, 5, -5, std::vector<std::tuple<int, int>>{
          {1, -1},
          {2, -2},
          {3, -3},
          {4, -4},
          {5, -5},
      }},
      std::tuple{0, 0, -5, 5, std::vector<std::tuple<int, int>>{
          {-1, 1},
          {-2, 2},
          {-3, 3},
          {-4, 4},
          {-5, 5},
      }},
      std::tuple{0, 0, -5, -5, std::vector<std::tuple<int, int>>{
          {-1, -1},
          {-2, -2},
          {-3, -3},
          {-4, -4},
          {-5, -5},
      }});

  std::vector<std::tuple<int, int>> points{};
  for_each_in_line(x, y, dx, dy, [&points](int x, int y) {
    points.emplace_back(x, y);
    return false;
  });
  CHECK(points == expected);
}
