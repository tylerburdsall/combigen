/* cli_functions.h
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

#ifndef CLI_FUNCTIONS_H
#define CLI_FUNCTIONS_H

#include "combigen.h"

const void                   display_csv_keys(const vector<string> &keys, const string &delim);
const void                   display_help(void);
const void                   output_result(const vector<string> &result, const generation_args &args, const bool &for_optimization);
const possible_combinations  parse_file(const string &input);
const possible_combinations  parse_stdin(const string &input);
#endif
