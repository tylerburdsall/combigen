#include "combigen.h"

int main(int argc, char* argv[])
{
    int c;
    generation_args args;

    while ( (c = getopt(argc, argv, "han:i:t:r:d:kv")) != -1)
    {
        switch (c)
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
            case 'v':
                cout << "combigen - v" << COMBIGEN_MAJOR_VERSION << '.' << COMBIGEN_MINOR_VERSION << '.' << COMBIGEN_REVISION_VERSION << '\n';
                exit(0);
            default: 
                display_help();
                exit(-1);
        }
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
    catch (runtime_error e)
    {
        handle_exception(e);
    }
}

static const void display_csv_keys(const vector<string> &keys, const char &delim)
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
         << "   -a             Generates every possible combination" << "\n"
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
         << "   -v             Display version number" << "\n";
}

static const void handle_exception(const runtime_error &e)
{
    try 
    {
        throw e;
    }
    catch (lazycp::errors::index_error)
    {
        cerr << "ERROR: the given index cannot be out of range\n";
    }
    catch (lazycp::errors::empty_list_error)
    {
        cerr << "ERROR: an empty list cannot be a value for a key\n";
    }
    catch (lazycp::errors::empty_answers_error)
    {
        cerr << "ERROR: an empty list cannot be a value for a key\n";
    }
    catch (lazycp::errors::invalid_sample_size_error)
    {
        cerr << "ERROR: the given sample size cannot be out of range\n";
    }
    catch (...)
    {
        cerr << "ERROR: an unknown error occurred\n";
    }
    exit(-1);
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
    long n = 0;
    if (args.generate_all_combinations)
    {
        n = max_size;
    }
    else
    {
        if (args.sample_size == 0 && args.entry_at > -1 && !args.generate_all_combinations)
        {
            try
            {
                vector<string> result = lazy_cartesian_product::entry_at(args.pc.combinations, args.entry_at);
                output_result(result, args, false);
                exit(0);
            }
            catch (runtime_error e)
            {
                handle_exception(e);
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
    const long last = n - 1;
    for (long i = 0; i < n; ++i)
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
    exit(0);
}

static const possible_combinations parse_file(const string &input)
{
    try
    {
        possible_combinations pc;
        ifstream i(input);
        json json_file;
        i >> json_file;

        for (auto obj = json_file.begin(); obj != json_file.end(); ++obj)
        {
            pc.keys.push_back(obj.key());
            vector<string> vals = json_file[obj.key()];
            pc.combinations.push_back(vals);
        }
        return pc;
    }
    catch (nlohmann::detail::parse_error)
    {
        cerr << "ERROR: Couldn't parse the given file, please ensure the file is in valid .json format and is accessible." << '\n';
        exit(-1);
    }
    catch (nlohmann::detail::type_error)
    {
        cerr << "ERROR: All values in input must be an array containing strings" << '\n';
        exit(-1);
    }
    catch (runtime_error e)
    {
        handle_exception(e);
    }
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
    catch (nlohmann::detail::type_error)
    {
        cerr << "ERROR: All values in input must be an array containing strings" << '\n';
        exit(-1);
    }
    catch (nlohmann::detail::parse_error)
    {
        cerr << "ERROR: Unable to parse the given input, please ensure a valid .json input has been provided" << '\n';
        exit(-1);
    }
    catch (runtime_error e)
    {
        handle_exception(e);
    }
    return pc;
}