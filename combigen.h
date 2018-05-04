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
const possible_combinations parse_input(const string &input);
const void display_results(const vector<vector<string>>& results, const char &delim);
const void display_results(const vector<string>& row, const char &delim);

#endif