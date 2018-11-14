/* combigen.h
 * 
 * (c) Tyler Burdsall - 2018
 */
#ifndef COMBIGEN
#define COMBIGEN

#define COMBIGEN_MAJOR_VERSION 1
#define COMBIGEN_MINOR_VERSION 2
#define COMBIGEN_REVISION_VERSION 2

#include <iostream>
#include <iterator>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdexcept>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "lib/win-getopt/getopt.h"
#else
#include <unistd.h>
#endif

#include <fstream>
#include <sstream>
#include "lib/nlohmann/json/single_include/nlohmann/json.hpp"
#include "lib/iamtheburd/lazy-cartesian-product/lazy-cartesian-product.hpp"


using std::cout;
using std::cin;
using std::cerr;
using std::vector;
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
    unsigned long long              entry_at = 0;
    unsigned long long              sample_size = 0;
    bool                            generate_all_combinations = false;
    bool                            display_keys = false;
    bool                            display_json = false;
    bool                            perf_mode = false;
    bool	                    entry_at_provided = false;
};

static const void                   display_csv_keys(const vector<string> &keys, const string &delim);
static const void                   display_help(void);
static const void                   generate_all(const unsigned long long &max_size, const generation_args &args);
static const void                   generate_random_samples(const vector<unsigned long long> &range, const generation_args &args);
static const void                   generate_random_samples_performance_mode(const generation_args &args);
static const void                   output_result(const vector<string> &result, const generation_args &args, const bool &for_optimization);
static const void                   parse_args(const generation_args &args);
static const possible_combinations  parse_file(const string &input);
static const possible_combinations  parse_stdin(const string &input);

#endif
