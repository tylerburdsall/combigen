#include "combigen.h"

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
                    iss >> args.entry_at;
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

static const void display_csv_keys(const vector<string> &keys, const string &delim)
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

static const void display_help(void)
{
    cout << "Usage: combigen [options]" << "\n"
         << "   -h             Displays this help message" << "\n\n"
         << "   -a             Generates every possible combination, restricted to memory mode." << "\n"
         << "                  (Note: this should be used with caution when storing to disk)" << "\n\n"
         << "   -n <index>     Generate combination at nth index" << "\n\n"
         << "   -i <input>     Take the given .json file as input. Otherwise, input will come" << "\n"
         << "                  from stdin." << "\n"
         << "                  Example: \"{ \"foo\": [ \"a\", \"b\", \"c\" ], \"bar\": [ \"1\", \"2\" ] }\"" << "\n\n"
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

static const void generate_all(const long &max_size, const generation_args &args)
{
    if (!args.display_json)
    {
        if (args.display_keys)
        {
            display_csv_keys(args.pc.keys, args.delim);
        }
    }
    else
    {
        cout << "[\n";
    }
    const long last = max_size - 1;
    for (long i = 0; i < max_size; ++i)
    {
        vector<string> result = lazy_cartesian_product::entry_at(args.pc.combinations, i);
        output_result(result, args, true);
        if (args.display_json && i != last)
        {
            cout << ",";
        }
    }
    if (args.display_json)
    {
        cout << "]\n";
    }
}

static const void generate_random_samples(const vector<long> &range, const generation_args &args)
{
    if (!args.display_json)
    {
        if (args.display_keys)
        {
            display_csv_keys(args.pc.keys, args.delim);
        }
    }
    else
    {
        cout << "[\n";
    }
    for (const long &i: range)
    {
        vector<string> result = lazy_cartesian_product::entry_at(args.pc.combinations, i);
        output_result(result, args, true);
        if (args.display_json && &i != &range.back())
        {
            cout << ",";
        }
    }
    if (args.display_json)
    {
        cout << "]\n";
    }
}

static const void generate_random_samples_performance_mode( const generation_args &args)
{
    const vector<vector<string>> results = lazy_cartesian_product::generate_samples(args.pc.combinations, args.sample_size);
    
    if (!args.display_json)
    {
        if (args.display_keys)
        {
            display_csv_keys(args.pc.keys, args.delim);
        }
    }
    else
    {
        cout << "[\n";
    }
    for( const vector<string> &row: results)
    {
        output_result(row, args, true);
        if (args.display_json && &row != &results.back())
        {
            cout << ",";
        }
    }
    if (args.display_json)
    {
        cout << "]\n";
    }
}

static const void output_result(const vector<string> &result, const generation_args &args, const bool &for_optimization)
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
        const long key_size = args.pc.keys.size();
        if (!for_optimization) 
        {
            cout << "[\n";
        }
        json entry;
        for (long j = 0; j < key_size; ++j)
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

static const void parse_args(const generation_args &args)
{
    long max_size = lazy_cartesian_product::compute_max_size(args.pc.combinations);
    if (args.generate_all_combinations)
    {
        generate_all(max_size, args);
        exit(0);
    }
    else
    {
        if (args.sample_size == 0 && args.entry_at > -1 && !args.generate_all_combinations)
        {
            vector<string> result = lazy_cartesian_product::entry_at(args.pc.combinations, args.entry_at);
            output_result(result, args, false);
            exit(0);
        }
        else if (args.sample_size > 0)
        {
            const long n = args.sample_size;
            if (n > max_size)
            {
                cerr << "ERROR: Sample size cannot be greater than maximum possible combinations\n";
                exit(-1);
            }
            if (args.perf_mode)
            {
                generate_random_samples_performance_mode(args);
            }
            else
            {
                vector<long> range = lazy_cartesian_product::generate_random_indices(n, max_size);
                generate_random_samples(range, args);
            }
            exit(0);
        }
        else
        {
            display_help();
            exit(-1);
        }
    }
}

static const possible_combinations parse_file(const string &input)
{
    possible_combinations pc;
    try
    {
        ifstream i(input);
        json json_file;
        i >> json_file;

        for (auto obj = json_file.begin(); obj != json_file.end(); ++obj)
        {
            pc.keys.push_back(obj.key());
            vector<string> vals = json_file[obj.key()];
            pc.combinations.push_back(vals);
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

static const possible_combinations parse_stdin(const string &input)
{
    possible_combinations pc;
    try
    {
        auto parsed = json::parse(input);
        for (auto obj = parsed.begin(); obj != parsed.end(); ++obj)
        {
            pc.keys.push_back(obj.key());
            vector<string> vals = parsed[obj.key()];
            pc.combinations.push_back(vals);
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

