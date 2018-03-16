#include "test.hpp"
#include "cpp-semver.hpp"

#include <tuple>
#include <string>
#include <vector>
#include <iostream>

const std::vector<
                   std::tuple< std::string,              /* version */
                               std::vector<std::string>, /* intersected */
                               std::vector<std::string>  /* not intersected */> > test_data
{
  {
    ">=1.2.7",
    { "1.2.7", "1.2.8", "2.5.3", "1.3.9" },
    { "1.2.6", "1.1.0", "1.2.7-pre", "6.6.6-pre" }
  },
  {
    ">=1.2.7 <1.3.0",
    { "1.2.7", "1.2.8", "1.2.99" },
    { "1.2.6", "1.3.0", "1.1.0", "1.2.7-pre", "1.2.9-pre" }
  },
  {
    "1.2.7 || >=1.2.9 <2.0.0",
    { "1.2.7", "1.2.9", "1.4.6", "1.2.9" },
    { "1.2.8", "2.0.0", "2.*", "1.2.9-pre", "1.3.0-pre" }
  },
  {
    ">1.2.3-alpha.3",
    { "1.2.3-alpha.7", "1.2.3", "2.0.0" },
    { "1.0.0", "1.2.3-alpha.1", "3.4.5-alpha.9" }
  },
  {
    "<1.2.3-alpha.3",
    { "1.2.3-alpha.1", "1.2.1" },
    { "1.2.3", "1.1.5-alpha.1", "1.2.3-alpha.4" }
  },
  {
    "1.2.3 - 2.3.4",
    { "1.2.3", "2.3.4", "1.*", "2.3.*" },
    { "1.0.0", "2.5.*" }
  },
  {
    "1.2 - 2.3.4",
    { "1.2.0", "1.2.*", "2.3.4" },
    { "1.0.0", "2.5.*" }
  },
  {
    "1.2.3 - 2.3",
    { "1.2.*", "2.3.9999" },
    { "2.4", "1.2.2", "0 - 1.1.0" }
  },
  {
    "  1.2.3   -    2.3  ",
    { "1.2.*", "2.3.9999" },
    { "2.4", "1.2.2", "0 -   1.1.0  " }
  },
  {
    "1.2.3 - 2",
    { "1.2.3", "2.999.999" },
    { "3.0.0", "<1.2.3" }
  },
  {
    "1.X",
    { "1.0.0", "1.*.*" },
    { "2.0.0" }
  },
  {
    "1.2.x",
    { "1.2.0", "1.2.99" },
    { "1.3.0" }
  },
  {
    "1.2.*",
    { "1.2", "1.*" },
    { "1.3.0" }
  },
  {
    "*",
    { "*.*.*", "0.0.0" },
    { "1.1.1-pre" }
  },
  {
    "",
    { "*.*.*", "0.0.0" },
    { "1.1.1-pre" }
  },
  {
    "1",
    { "1.0.0" },
    { "2.0.0" }
  },
  {
    "1.2",
    { "1.2.0" },
    { "1.3.0" }
  },
  {
    "~1.2.3",
    { ">=1.2.3 <1.3.0", "1.2.3" },
    { "1.3.0" }
  },
  {
    "~1.2",
    { ">=1.2.0 <1.3.0", "1.2.3", "1.2.x" },
    { "1.3.0" }
  },
  {
    "~1",
    { "1.x", ">=1.0.0 <2.0.0" },
    { "2.0.0" }
  },
  {
    "~0.2.3",
    { ">=0.2.3 <0.3.0", "0.2.3" },
    { "0.3.0" }
  },
  {
    "~0.2",
    { ">=0.2.0 <0.3.0", "0.2.x" },
    { "0.3" }
  },
  {
    "~0",
    { ">=0.0.0 <1.0.0", "0.x" },
    { "1.0.0" }
  },
  {
    "~1.2.3-beta.2",
    { ">=1.2.3-beta.2 <1.3.0", "1.2.3-beta.4" },
    { "1.2.3-alpha", "1.3.0", "1.2.4-beta.2" }
  },
  {
    "^1.2.3",
    { "1.2.3", ">=1.2.3 <2.0.0" },
    { "2.0.0", "1.2.4-rx" }
  },
  {
    "^0.2.3",
    { ">=0.2.3 <0.3.0" },
    { "0.3.0" }
  },
  {
    "^0.0.3",
    { ">=0.0.3 <0.0.4" },
    { "0.0.4" }
  },
  {
    "^1.2.3-beta.2",
    { ">=1.2.3-beta.2 <2.0.0", "1.2.3-beta.4", "1.2.3" },
    { "1.2.4-beta.2", "2.*" }
  },
  {
    "^0.0.3-beta",
    { ">=0.0.3-beta <0.0.4", "0.0.3-pr.2", "0.0.3" },
    { "0.1.3-pr.2", "0.0.4" }
  },
  {
    "^1.2.x",
    { "1.2.0", "1.99.11", ">=1.2.0 <2.0.0" },
    { "2.0.0" }
  },
  {
    "^0.0.x",
    { "0.0.0", "0.0.11", ">=0.0.0 <0.1.0" },
    { "0.1.0" }
  },
  {
    "^0.0",
    { "0.0.0", "0.0.11", ">=0.0.0 <0.1.0" },
    { "0.1.0" }
  },
  {
    "^1.x",
    { "1.0.0", ">=1.0.0 <2.0.0" },
    { "1.0.0-pre", "2.0.0" }
  },
  {
    "^0.x",
    { "0.0.0", "0.99.99", ">=0.0.0 <1.0.0" },
    { "1.0.0", "0.0.1-pre" }
  },
  {
    "=v1.2.3",
    { "1.2.3" },
    {}
  },
  {
    " || || ",
    { "6.6.6" },
    { "1.1.1-pre" }
  },
  {
    " * || || *.*",
    { "6.6.6" },
    { "1.1.1-pre" }
  },
  {
    "1.2.3-beta.1.a.3+build1",
    { "1.2.3-beta.1.a.3", "1.2.3-beta.1.a.3+build110" },
    { "1.2.3-beta.2", "1.2.*" }
  },
  {
    "  0.0.1||  1.2.*  ||2.3.4  ||3.4.5|| 7.8.9 ",
    { "1.2.9", "7.8.9" },
    { "1.1.*" }
  },
  {
    " 1.0.0 - 2.0.0 || >=3.3.3 <6.6.6 || >=1.2.6-beta.2 ",
    { "1.0.0", "2.0.0", "1.1.*", "3.3.4", "6.6.6", "1.2.6-rc.2" },
    { "0.2.5", "6.6.6-rc.2" }
  },
  {
    "1.*.*",
    { "1.0.0", "1.1.0" },
    { "0.1.0", "2.0.0", "0.*", "2.*", "1.0.0-rc" }
  },
  {
    "1.0.*-alpha",
    { "1.0.0", "1.0.1" },
    { "0.1.0", "2.0.0", "1.0.1-alpha" }
  },
  {
    "1.5.0-a - 2.0.0-r",
    { "1.5.0", "1.5.0-b", "2.0.0-a", "1.5.5" },
    { "1.8.9-a", "2.0.0-z", "1.5.5-a" }
  },
  {
    "<1.5.*-rc",
    { "1.4.999" },
    { "1.5.0", "1.5.0-alpha", "1.0.0-alpha" }
  },
  {
    "<2.*.*-rc",
    { "1.4.9" },
    { "2.0.0", "2.0.0-alpha", "1.0.0-alpha" }
  },
  {
    "<=2.0.*-rc",
    { "1.9.9", "2.0.0", "2.0.2", },
    { "2.0.0-alpha", "1.0.0-alpha" }
  },
  {
    "<=2.*.*-rc",
    { "1.9.9", "2.0.0", "2.9.9" },
    { "2.0.0-alpha", "1.0.0-alpha" }
  },
  {
    ">1.3.*-rc",
    { "1.4.0" },
    { "1.3.0", "1.3.88", "1.3.0-rc1", "1.4.0-rc1" }
  },
  {
    ">1.*.*-rc",
    { "2.0.0" },
    { "1.10.0", "1.0.0-rc1", "2.0.0-rc1" }
  },
  {
    ">=2.0.*-rc",
    { "2.0.0", "2.0.2", },
    { "2.0.0-rc1", "3.0.0-rc1" }
  },
  {
    ">=2.*.*-rc",
    { "2.0.0", "2.9.9" },
    { "2.0.0-alpha", "1.0.0-alpha" }
  },
  {
    "2.0.*-rc",
    { "2.0.0", "2.0.9" },
    { "2.1.0", "2.0.0-rc", "2.0.1-rc" }
  },
  {
    "2.*.*-rc",
    { "2.0.0", "2.9.9" },
    { "1.1.0", "2.0.0-rc", "2.9.9-rc" }
  },
  {
    "~2.0.*-a",
    { "2.0.0" },
    { "2.0.0-beta", "2.1.0" }
  },
  {
    "~2.*.*-a",
    { "2.0.0", "2.9.9" },
    { "2.0.0-beta", "1.1.0", "3.0.0" }
  },
  {
    "^1.5.6-a",
    { ">=1.5.6-a <2.0.0", "1.5.6-beta", "1.5.6", "1.9.9" },
    { "2.0.0", "1.5.7-b" }
  },
  {
    "^1.5.*-a",
    { "1.5.0", "1.5.6", "1.9.9" },
    { "1.5.0-b", "1.5.7-b" }
  },
  {
    "^1.*.*-a",
    { "1.0.0", "1.5.6", "1.9.9" },
    { "1.0.0-b", "1.5.7-b" }
  },
  {
    "^0.5.6-a",
    { ">=0.5.6-a <0.6.0", "0.5.6-beta", "0.5.6", "0.5.8" },
    { "0.6.0", "0.5.8-b" }
  },
  {
    "^0.5.6-a",
    { ">=0.5.6-a <0.6.0", "0.5.6-beta", "0.5.6", "0.5.8" },
    { "0.6.0", "0.5.8-b" }
  },
  {
    "^0.0.6-a2",
    { ">=0.0.6-a <0.0.7", "0.0.6-beta", "0.0.6" },
    { "0.0.7", "0.0.6-a1" }
  },
  {
    "*.2.3",
    { "*" },
    {}
  },
  {
    "1.*.3",
    { "1.*" },
    { "0.0.1" }
  },
  {
    ">2 <1",
    {  },
    { "*" }
  },
  {
    "",
    { "1.2.3" },
    {}
  },
  {
    "1.1.1-pre--.--.--.000",
    { },
    { }
  }
};

const std::vector<std::string> intersected_comp
{
  "1.0.0", ">=1.0.0", ">=1.0.0 <2.0.0", "<1.0.0 || >2.0.0"
};

const std::vector<std::string> not_intersected_comp
{
  "<=1.0.0 >2.0.0", "1.0.0 >2.0.0"
};

void run_test()
{
  for (const std::string& input : intersected_comp)
  {
    std::cout << "[Test] intersects( \"" << input << "\" )" << std::endl;
    TEST_ASSERT(semver::intersects(input));
  }

  for (const std::string& input : not_intersected_comp)
  {
    std::cout << "[Test] !intersects( \"" << input << "\" )" << std::endl;
    TEST_ASSERT(!semver::intersects(input));
  }

  for (const auto& tpl : test_data)
  {
    const auto& input = std::get<0>(tpl);
    const auto& intersected = std::get<1>(tpl);
    const auto& not_intersected = std::get<2>(tpl);

    for (const auto& match : intersected)
    {
      std::cout << "[Test] intersects( \"" << input << "\", \"" << match << "\" )" << std::endl;
      TEST_ASSERT(semver::intersects(input, match));

      std::cout << "[Test] intersects( \"" << match << "\", \"" << input << "\" )" << std::endl;
      TEST_ASSERT(semver::intersects(match, input));
    }

    for (const auto& not_match : not_intersected)
    {
      std::cout << "[Test] !intersects( \"" << input << "\", \"" << not_match << "\" )" << std::endl;
      TEST_ASSERT(!semver::intersects(input, not_match));

      std::cout << "[Test] !intersects( \"" << not_match << "\", \"" << input << "\" )" << std::endl;
      TEST_ASSERT(!semver::intersects(not_match, input));
    }

  }
}