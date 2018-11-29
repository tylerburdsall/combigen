/* combigen.h
 *
 * Copyright (C) 2018 Tyler Burdsall
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COMBIGEN_H
#define COMBIGEN_H

#include <iostream>
#include <iterator>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <fstream>
#include <sstream>
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
using std::istringstream;
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

#ifdef USE_BOOST
const void                   generate_all(const uint1024_t &max_size, const generation_args &args);
const void                   generate_random_samples(const vector<uint1024_t> &range, const generation_args &args);
#else
const void                   generate_all(const unsigned long long &max_size, const generation_args &args);
const void                   generate_random_samples(const vector<unsigned long long> &range, const generation_args &args);
#endif
const void                   generate_random_samples_performance_mode(const generation_args &args);
const void                   parse_args(const generation_args &args);

#endif
