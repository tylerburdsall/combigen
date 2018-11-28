#ifndef CLI_FUNCTIONS_H
#define CLI_FUNCTIONS_H

#include "combigen.h"

const void                   display_csv_keys(const vector<string> &keys, const string &delim);
const void                   display_help(void);
const void                   output_result(const vector<string> &result, const generation_args &args, const bool &for_optimization);
const possible_combinations  parse_file(const string &input);
const possible_combinations  parse_stdin(const string &input);
#endif
