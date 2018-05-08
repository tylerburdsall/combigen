#include "combigen.h"

int main(int argc, char* argv[])
{
    int c;
    generation_args args;

    while( (c = getopt(argc, argv, "han:i:t:r:d:km:")) != -1)
    {
        switch(c)
        {
            case 'h':
                display_help();
                exit(0);
            case 'a':
                args.generate_all_combinations = true;
                break;
            case 'n':
                if (optarg)
                {
                    istringstream iss (optarg);
                    iss >> args.entry_at;
                }
                break;
            case 'i':
                if (optarg)
                {
                    args.input = optarg;
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
                }
                break;
            case 'd':
                if (optarg)
                {
                    args.delim = *optarg;
                }
                break;
            case 'k':
                args.display_keys = true;
                break;
            case 'm':
                if (optarg)
                {
                    string s = optarg;
                    if (s[0] == 'm' || s[0] == 'M')
                    {
                        args.perf_mode = false;
                    }
                    else if (s[0] != 'p' || s[0] != 'P')
                    {
                        display_help();
                        exit(-1);
                    }
                }
                break;
            default: 
                display_help();
                exit(-1);
        }
    }
    if (args.input.empty())
    {
        display_help();
        exit(-1);
    }

    args.pc = parse_input(args.input);
    try
    {
        parse_args(args);
    }
    catch (const char* e)
    {
        cerr << e << "\n";
        exit(-1);
    }
}

const void display_help(void)
{
    cout << "Usage: combigen [options]" << "\n"
         << "   -h             Displays this help message" << "\n\n"
         << "   -a             Generates every possible combination (use with caution)" << "\n\n"
         << "   -n <index>     Generate combination at nth index" << "\n\n"
         << "   -i <input>     Take the given .json file or string as" << "\n"
	     << "                  input for the combinations." << "\n"
         << "                  Example: \"{ \"foo\": [ \"a\", \"b\", \"c\" ], \"bar\": [ \"1\", \"2\" ] }\"" << "\n\n"
         << "   -t <type>      Output type (csv or json). Defaults to csv" << "\n\n"
         << "   -r <size>      Generate a random sample of size r from" << "\n"
	     << "                  the possible set of combinations" << "\n\n"
         << "   -d <delimiter> Set the delimiter when displaying combinations (default is ',')" << "\n\n"
         << "   -k             Display the keys on the first line of output (for .csv)" << "\n\n"
         << "   -m <mode>      Specify the mode for combigen to run in, " << "\n"
         << "                  (p)erformance- or (m)emory-oriented (default is performance)" << "\n"
         << "                      - p: When generating combinations the program will store all of the" << "\n"
         << "                           combinations in RAM before writing to a file/stdout. This is" << "\n"
         << "                           recommended for machines with large amounts of RAM and should" << "\n"
         << "                           offer performance benefits for incredibly large combination generation" << "\n"
         << "                      - m: Each combination will be generated and then written to a file/stdout" << "\n"
         << "                           before continuing to the next generation. This is recommended for" << "\n"
         << "                           with small amounts of RAM when generating large combinations so that" << "\n"
         << "                           disk space will become the only limiting factor." << "\n\n"
         << "Version:          " << COMBIGEN_MAJOR_VERSION << '.' << COMBIGEN_MINOR_VERSION << '.' << COMBIGEN_REVISION_VERSION << '\n';
}

const void display_keys(const vector<string> &keys, const char &delim)
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

const possible_combinations parse_input(const string &input)
{
    try
    {
        possible_combinations pc;
        auto parsed = json::parse(input);
        for( auto it = parsed.begin(); it != parsed.end(); ++it)
        {
            pc.keys.push_back(it.key());
            vector<string> vals = parsed[it.key()];
            pc.combinations.push_back(vals);
        }
        return pc;
    }
    catch (nlohmann::detail::type_error)
    {
        cerr << "ERROR: All values in input must be an array containing strings" << '\n';
        exit(-1);
    }
    catch (nlohmann::detail::parse_error)
    {
        try
        {
            possible_combinations pc;
            ifstream i(input);
            json j;
            i >> j;
            for( auto it = j.begin(); it != j.end(); ++it)
            {
                pc.keys.push_back(it.key());
                vector<string> vals = j[it.key()];
                pc.combinations.push_back(vals);
            }
            return pc;
        }
        catch (nlohmann::detail::parse_error)
        {
            cerr << "ERROR: Couldn't parse the given input, please ensure the input is in valid .json format and is accessible." << '\n';
            exit(-1);
        }
        catch (nlohmann::detail::type_error)
        {
            cerr << "ERROR: All values in input must be an array containing strings" << '\n';
            exit(-1);
        }
    }
    catch (const char* e)
    {
        cerr << e << "\n";
        exit(-1);
    }
}

const void parse_args(const generation_args &args)
{
    long max_size = lazy_cartesian_product::compute_max_size(args.pc.combinations);
    long n = 0;
    if (args.generate_all_combinations)
    {
        n = max_size;
    }
    else
    {
        if (args.sample_size == 0 && args.entry_at > -1)
        {
            try
            {
                vector<string> result = lazy_cartesian_product::entry_at(args.pc.combinations, args.entry_at);
                output_results(result, args, false);
                exit(0);
            }
            catch (char const* e)
            {
                cerr << "ERROR: " << e << "\n";
                exit(-1);
            }
        }
        else if (args.sample_size > 0)
        {
            n = args.sample_size;
            if (n > max_size)
            {
                cerr << "ERROR: Sample size cannot be greater than maximum possible combinations\n";
                exit(-1);
            }
        }
        else
        {
            display_help();
            exit(-1);
        }
    }

    if (args.perf_mode)
    {
        vector<vector<string>> results = lazy_cartesian_product::generate_samples(args.pc.combinations, n);
        output_results(results, args);
        exit(0);
    }
    else
    {
        if (!args.display_json)
        {
            if (args.display_keys)
            {
                display_keys(args.pc.keys, args.delim);
            }
        }
        else
        {
            cout << "[\n";
        }
        const long last = n - 1;
        for (long i = 0; i < n; ++i)
        {
            vector<string> result = lazy_cartesian_product::entry_at(args.pc.combinations, i);
            output_results(result, args, true);
            if (args.display_json && i != last)
            {
                cout << ",";
            }
        }
        if (args.display_json)
        {
            cout << "]\n";
        }
        exit(0);
    }
}

const void output_results(const vector<vector<string>> &results, const generation_args &args)
{
    if (!args.display_json)
    {
        if (args.display_keys)
        {
            for (auto& s: args.pc.keys)
            {
                if (&s == &args.pc.keys.back())
                {
                    cout << s;
                }
                else
                {
                    cout << s << args.delim;
                }
            }
            cout << '\n';
        }
        for (auto &row : results)
        {
            for (auto &s: row)
            {
                if (&s == &row.back())
                {
                    cout << s;
                }
                else
                {
                    cout << s << args.delim;
                }
            }
            cout << '\n';
        }
    }
    else
    {
        const long key_size = args.pc.keys.size();
        const long results_size = results.size();
        cout << "[\n";
        for (long i = 0; i < results_size; ++i)
        {
            json entry;
            for (long j = 0; j < key_size; ++j)
            {
                entry[args.pc.keys[j]] = results[i][j];
            }
            cout << entry.dump(4);
            if (i != results_size - 1)
            {
                cout << ",";
            }
            cout << "\n";
        }
        cout << "]\n";
    }
}

const void output_results(const vector<string> &row, const generation_args &args, const bool &for_optimization)
{
    if (!args.display_json)
    {
        if (args.display_keys && !for_optimization)
        {
            display_keys(args.pc.keys, args.delim);
        }
        for (auto &s: row)
        {
            if (&s == &row.back())
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
        const long key_size = args.pc.keys.size();
        if (!for_optimization) 
        {
            cout << "[\n";
        }
        json entry;
        for (long j = 0; j < key_size; ++j)
        {
            entry[args.pc.keys[j]] = row[j];
        }
        cout << entry.dump(4);
        if (!for_optimization)
        {
            cout << "]\n";
        }
    }
}