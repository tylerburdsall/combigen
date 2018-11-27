#ifndef COMBIGEN_H
#define COMBIGEN_H

#include <iostream>
#include <iterator>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include "lib/nlohmann/json/single_include/nlohmann/json.hpp"
#include "lib/iamtheburd/lazy-cartesian-product/lazy-cartesian-product.hpp"

#ifdef USE_BOOST
#include <boost/container/vector.hpp>
#include <boost/multiprecision/cpp_int.hpp>
using boost::container::vector;
using namespace boost::multiprecision;
#else
#include <vector>
using std::vector;
using std::stoull;
#endif

using std::cout;
using std::cin;
using std::cerr;
using std::string;
using std::ifstream;
using std::istreamstream;
using std::istreambuf_iterator;
using std::runtime_error;

using lazycp::lazy_cartesian_product;
using json = nlohmann::json;

struct possible_combinations
{
    vector<string>                  keys;
    vector<vector<string>>          combinations;
};

struct generation_args
{
    possible_combinations           pc;
    string                          input;
    string                          delim = ",";
    string                          entry_at = "0";
    string                          sample_size = "0";
    bool                            generate_all_combinations = false;
    bool                            display_keys = false;
    bool                            display_json = false;
    bool                            perf_mode = false;
    bool	                    entry_at_provided = false;
};



#endif
