#ifndef MAIN
#define MAIN

#define COMBIGEN_MAJOR_VERSION 1
#define COMBIGEN_MINOR_VERSION 3
#define COMBIGEN_REVISION_VERSION 0

#include <iostream>
#include <iterator>
#include <string>
#include <cstdlib>
#include <sstream>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "lib/win-getopt/getopt.h"
#else
#include <unistd.h>
#endif

#include "combigen.h"

using std::cout;
using std::cin;
using std::cerr;
using std::string;
using std::istringstream;
using std::istreambuf_iterator;

int main(int argc, char* argv[])
{
    int             c;
    bool            args_provided = false;
    generation_args args;
    while ( (c = getopt(argc, argv, "han:i:t:r:d:kvp")) != -1)
    {
        switch (c)
        {
            case 'h':
                display_help();
                exit(0);
            case 'a':
                args_provided = true;
                args.generate_all_combinations = true;
                break;
            case 'n':
                if (optarg)
                {
                    istringstream iss (optarg);
		    string s = optarg;
		    if (s.at(0) == '-')
		    {
		        display_help();
			exit(-1);
		    }
                    iss >> args.entry_at;
		    args.entry_at_provided = true;
                    args_provided = true;
                }
                break;
            case 'i':
                if (optarg)
                {
                    args.input = optarg;
                    args_provided = true;
                }
                break;
            case 't':
                if (optarg)
                {
                    string s = optarg;
                    if (s == "json")
                    {
                        args.display_json = true;
                    }
                    else if (s != "csv")
                    {
                        display_help();
                        exit(-1);
                    }
                }
                break;
            case 'r':
                if (optarg)
                {
                    istringstream iss (optarg);
                    iss >> args.sample_size;
                    args_provided = true;
                }
                break;
            case 'd':
                if (optarg)
                {
                    args.delim = optarg;
                }
                break;
            case 'k':
                args_provided = true;
                break;
            case 'v':
                cout << "combigen - v" << COMBIGEN_MAJOR_VERSION << '.' << COMBIGEN_MINOR_VERSION << '.' << COMBIGEN_REVISION_VERSION << '\n';
                exit(0);
            case 'p':
                args.perf_mode = true;
                break;
            default: 
                display_help();
                exit(-1);
        }
    }
    if (!args_provided)
    {
        display_help();
        exit(0);
    }
    if (args.input.empty())
    {
        istreambuf_iterator<char> begin(cin), end;
        string input_stream(begin, end);
        args.input = input_stream;
        args.pc = parse_stdin(args.input);
    }
    else
    {
        args.pc = parse_file(args.input);
    }
    
    try
    {
        parse_args(args);
    }
    catch (const lazycp::errors::index_error&)
    {
        cerr << "ERROR: the given index cannot be out of range\n";
    }
    catch (const lazycp::errors::empty_list_error&)
    {
        cerr << "ERROR: an empty list cannot be a value for a key\n";
    }
    catch (const lazycp::errors::empty_answers_error&)
    {
        cerr << "ERROR: an empty list cannot be a value for a key\n";
    }
    catch (const lazycp::errors::invalid_sample_size_error&)
    {
        cerr << "ERROR: the given sample size cannot be out of range\n";
    }
    catch (...)
    {
        cerr << "ERROR: an unknown error occurred\n";
    }
    return 0;
}
#endif
