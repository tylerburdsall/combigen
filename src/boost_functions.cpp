#ifndef BOOST_FUNCTIONS
#define BOOST_FUNCTIONS

#include "combigen.h"

static const void generate_random_samples_performance_mode(const generation_args &args)
{
    const uint1024_t sample_size(args.sample_size);
    const vector<vector<string>> results = lazy_cartesian_product::boost_generate_samples(args.pc.combinations, sample_size);
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

static const void parse_args(const generation_args &args)
{
    const uint1024_t max_size = lazy_cartesian_product::boost_compute_max_size(args.pc.combinations);
    if (args.generate_all_combinations)
    {
        generate_all(max_size, args);
        exit(0);
    }
    else
    {
        const uint1024_t sample_size(args.sample_size);
        if (sample_size == 0 && args.entry_at_provided && !args.generate_all_combinations)
        {
            const uint1024_t entry_at(args.entry_at);
            vector<string> result = lazy_cartesian_product::boost_entry_at(args.pc.combinations, args.entry_at);
            output_result(result, args, false);
            exit(0);
        }
        else if (sample_size >= 0)
        {
            const uint1024_t n(args.sample_size);
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
                vector<uint1024_t> range = lazy_cartesian_product::boost_generate_random_indices(n, max_size);
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
#endif
