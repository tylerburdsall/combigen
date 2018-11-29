/* cli_functions.cpp
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

#ifndef CLI_FUNCTIONS_CPP
#define CLI_FUNCTIONS_CPP

#include "cli_functions.h"

const void display_help(void)
{
    cout << "Usage: combigen [options]" << "\n"
         << "   -h             Displays this help message" << "\n\n"
         << "   -a             Generates every possible combination, restricted to memory mode." << "\n"
         << "                  (Note: this should be used with caution when storing to disk)" << "\n\n"
         << "   -n <index>     Generate combination at nth index" << "\n\n"
         << "   -i <input>     Take the given .json file as input. Otherwise, input will come" << "\n"
         << "                  from stdin." << "\n"
         << "                  Example: \"{ \"foo\": [ \"a\", \"b\", \"c\" ], \"bar\": [ \"1\", \"2\" ] }\"" << "\n"
         << "                  Or:      \"[ [\"1\", \"2\"], [\"3\", \"4\", \"a\", \"b\"] ]\"" << "\n\n"
         << "   -t <type>      Output type (csv or json). Defaults to csv" << "\n\n"
         << "   -r <size>      Generate a random sample of size r from" << "\n"
	 << "                  the possible set of combinations" << "\n\n"
         << "   -d <delimiter> Set the delimiter when displaying combinations (default is ',')" << "\n\n"
         << "   -k             Display the keys on the first line of output (for .csv)" << "\n\n"
         << "   -p             Use performance mode to generate combinations faster at the" << "\n"
         << "                  expense of higher RAM usage." << "\n"
         << "                  (Note: this is only recommended for computers with large amounts" << "\n"
         << "                  of RAM when generating a large number of random combinations)" << "\n\n"
         << "   -v             Display version number" << "\n";
}


const void display_csv_keys(const vector<string> &keys, const string &delim)
{
    for (auto& s: keys)
    {
        if (&s == &keys.back())
        {
            cout << s;
        }
        else
        {
            cout << s << delim;
        }
    }
    cout << '\n';
}

const void output_result(const vector<string> &result, const generation_args &args, const bool &for_optimization)
{
    if (!args.display_json)
    {
        if (args.display_keys && !for_optimization)
        {
            display_csv_keys(args.pc.keys, args.delim);
        }
        for (auto &s: result)
        {
            if (&s == &result.back())
            {
                cout << s;
            }
            else
            {
                cout << s << args.delim;
            }
        }
        cout << "\n";
    }
    else
    {
        const unsigned long long key_size = args.pc.keys.size();
        if (!for_optimization) 
        {
            cout << "[\n";
        }
        json entry;
        for (unsigned long long j = 0; j < key_size; ++j)
        {
            entry[args.pc.keys[j]] = result[j];
        }
        cout << entry.dump(4);
        if (!for_optimization)
        {
            cout << "]\n";
        }
    }
}

const possible_combinations parse_file(const string &input)
{
    possible_combinations pc;
    try
    {
        ifstream i(input);
        json json_file;
        i >> json_file;

        if (json_file.type() == json::value_t::array)
        {
            for (json::iterator arr = json_file.begin(); arr != json_file.end(); ++arr)
            {
                pc.combinations.push_back(arr.value());
            }
        }
        else
        {
            for (auto obj = json_file.begin(); obj != json_file.end(); ++obj)
            {
                pc.keys.push_back(obj.key());
                pc.combinations.push_back(obj.value());
            }
        }
    }
    catch (const nlohmann::detail::parse_error&)
    {
        cerr << "ERROR: Couldn't parse the given file, please ensure the file is in valid .json format and is accessible." << '\n';
        exit(-1);
    }
    catch (const nlohmann::detail::type_error&)
    {
        cerr << "ERROR: All values in input must be an array containing strings" << '\n';
        exit(-1);
    }
    return pc;
}

const possible_combinations parse_stdin(const string &input)
{
    possible_combinations pc;
    try
    {
        auto parsed = json::parse(input);
        if (parsed.type() == json::value_t::array)
        {
            for (json::iterator arr = parsed.begin(); arr != parsed.end(); ++arr)
            {
                pc.combinations.push_back(arr.value());
            }
        }
        else
        {
            for (auto obj = parsed.begin(); obj != parsed.end(); ++obj)
            {
                pc.keys.push_back(obj.key());
                pc.combinations.push_back(obj.value());
            }
        }
        
    }
    catch (const nlohmann::detail::type_error&)
    {
        cerr << "ERROR: All values in input must be an array containing strings" << '\n';
        exit(-1);
    }
    catch (const nlohmann::detail::parse_error&)
    {
        cerr << "ERROR: Unable to parse the given input, please ensure a valid .json input has been provided" << '\n';
        exit(-1);
    }
    return pc;
}

#endif
