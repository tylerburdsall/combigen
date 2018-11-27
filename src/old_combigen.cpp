#include "combigen.h"

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


static const void generate_random_samples_performance_mode(const generation_args &args)
{
#ifdef USE_BOOST
    const uint1024_t sample_size(args.sample_size);
    const vector<vector<string>> results = lazy_cartesian_product::boost_generate_samples(args.pc.combinations, sample_size);
#else
    const vector<vector<string>> results = lazy_cartesian_product::generate_samples(args.pc.combinations, args.sample_size);
#endif
    
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

static const void parse_args(const generation_args &args)
{
#ifdef USE_BOOST
    const uint1024_t max_size = lazy_cartesian_product::boost_compute_max_size(args.pc.combinations);
#else
    const unsigned long long max_size = lazy_cartesian_product::compute_max_size(args.pc.combinations);
#endif
    if (args.generate_all_combinations)
    {
        generate_all(max_size, args);
        exit(0);
    }
    else
    {
#ifdef USE_BOOST
        const uint1024_t sample_size(args.sample_size);
#else
        const unsigned long long sample_size = stoull(args.sample_size, 0, 10);
#endif
        if (sample_size == 0 && args.entry_at_provided && !args.generate_all_combinations)
        {
#ifdef USE_BOOST
            const uint1024_t entry_at(args.entry_at);
            vector<string> result = lazy_cartesian_product::boost_entry_at(args.pc.combinations, args.entry_at);
            output_result(result, args, false);
            exit(0);
#else
            const unsigned long long entry_at = stoull(args.entry_at, 0, 10);
            vector<string> result = lazy_cartesian_product::entry_at(args.pc.combinations, entry_at);
            output_result(result, args, false);
            exit(0);
#endif
        }
        else if (sample_size >= 0)
        {
#ifdef USE_BOOST
            const uint1024_t n(args.sample_size);
#else
            const unsigned long long n = stoull(args.sample_size, 0, 10);
#endif
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
#ifdef USE_BOOST
                vector<uint1024_t> range = lazy_cartesian_product::boost_generate_random_indices(n, max_size);
#else
                vector<unsigned long long> range = lazy_cartesian_product::generate_random_indices(n, max_size);
#endif
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

#ifdef USE_BOOST
static const void generate_random_samples(const vector<uint1024_t> &range, const generation_args &args)
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
    for (const uint1024_t &i: range)
    {
        vector<string> result = lazy_cartesian_product::boost_entry_at(args.pc.combinations, i.convert_to<string>());
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

static const void generate_all(const uint1024_t &max_size, const generation_args &args)
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
    const uint1024_t last = max_size - 1;
    for (uint1024_t i = 0; i < max_size; ++i)
    {
        vector<string> result = lazy_cartesian_product::boost_entry_at(args.pc.combinations, i.convert_to<string>());
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
#else
static const void generate_all(const unsigned long long &max_size, const generation_args &args)
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
    const unsigned long long last = max_size - 1;
    for (unsigned long long i = 0; i < max_size; ++i)
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

static const void generate_random_samples(const vector<unsigned long long> &range, const generation_args &args)
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
    for (const unsigned long long &i: range)
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
#endif
