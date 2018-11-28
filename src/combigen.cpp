#ifndef COMBIGEN_CPP
#define COMBIGEN_CPP

#include "combigen.h"
#include "cli_functions.h"

const void generate_random_samples_performance_mode(const generation_args &args)
{
    unsigned long long sample_size = stoull(args.sample_size, 0, 10);
    const vector<vector<string>> results = lazy_cartesian_product::generate_samples(args.pc.combinations, sample_size);
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


const void parse_args(const generation_args &args)
{
    const unsigned long long max_size = lazy_cartesian_product::compute_max_size(args.pc.combinations);
    if (args.generate_all_combinations)
    {
        generate_all(max_size, args);
        exit(0);
    }
    else
    {
        const unsigned long long sample_size = stoull(args.sample_size, 0, 10);
        if (sample_size == 0 && args.entry_at_provided && !args.generate_all_combinations)
        {
            const unsigned long long entry_at = stoull(args.entry_at, 0, 10);
            vector<string> result = lazy_cartesian_product::entry_at(args.pc.combinations, entry_at);
            output_result(result, args, false);
            exit(0);
        }
        else if (sample_size >= 0)
        {
            const unsigned long long n = stoull(args.sample_size, 0, 10);
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
                vector<unsigned long long> range = lazy_cartesian_product::generate_random_indices(n, max_size);
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


const void generate_all(const unsigned long long &max_size, const generation_args &args)
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

const void generate_random_samples(const vector<unsigned long long> &range, const generation_args &args)
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
