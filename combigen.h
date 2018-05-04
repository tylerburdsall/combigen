#ifndef COMBIGEN
#define COMBIGEN

#include <iostream>
#include <iomanip>
#include "../lazy-cartesian-product/lazy-cartesian-product.hpp"
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "json.hpp"


using std::cout;
using std::vector;
using std::string;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::cerr;

using lazycp::lazy_cartesian_product;
using json = nlohmann::json;

struct possible_combinations
{
    vector<string> keys;
    vector<vector<string>> combinations;
};

const void display_help(void);
const void display_keys(const vector<string> &keys, const char &delim);
const possible_combinations parse_input(const string &input);
const void display_results(const vector<vector<string>> &results, const vector<string> &keys, const char &delim, const bool &display_json);
const void display_results(const vector<string >&row, const char &delim, const bool &display_json);
const void write_to_file(const string &file, const vector<vector<string>> &results, const vector<string> &keys, const char &delim, const bool &display_keys, const bool &display_json);

#endif