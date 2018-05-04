#include "combigen.h"


int main(int argc, char* argv[])
{
    int c;
    long k = 0;
    long n = -1;
    bool verbose, generate_subset, generate_json = false;
    char delim = ',';
    string input;
    possible_combinations pc;
    while( (c = getopt(argc, argv, "hn:i:k:d:v")) != -1)
    {
        switch(c)
        {
            case 'h':
                display_help();
                exit(0);
            case 'n':
                if (optarg)
                {
                    istringstream iss (optarg);
                    iss >> n;
                }
                break;
            case 'i':
                if (optarg)
                {
                    input = optarg;
                }
                break;
            case 'k':
                if (optarg)
                {
                    istringstream iss (optarg);
                    iss >> k;
                    generate_subset = true;
                }
                break;
            case 'd':
                if (optarg)
                {
                    delim = *optarg;
                }
                break;
            case 'v':
                verbose = true;
                break;
            default: 
                display_help();
                exit(-1);
        }
    }
    if (input.empty())
    {
        display_help();
        exit(-1);
    }

    pc = parse_input(input);

    if (k == 0 && n > -1)
    {
        try
        {
            vector<string> result = lazy_cartesian_product::entry_at(pc.combinations, n);
            display_results(result, delim);
            return 0;
        }
        catch (char const* e)
        {
            cerr << "ERROR: " << e << "\n";
            exit(-1);
        }

    }

    return 0;
}

const void display_help(void)
{
    cout << "Usage: combigen [options]" << "\n"
         << "   -h             Displays this help message" << "\n"
         << "   -n <index>     Generate combination at nth index" << "\n"
         << "   -i <input>     Take the given .json file or string as input for the combinations" << "\n"
         << "                  Example: \"{ \"foo\": [ \"a\", \"b\", \"c\" ], \"bar\": [ \"1\", \"2\" ] }\"" << "\n"
         << "   -k <size>      Generate a random sample of size k from the possible set of combinations" << "\n"
         << "   -d <delimiter> Set the delimiter when displaying combinations (default is ',')"
         << "   -v             Verbosely display all of the combinations to stdout when generating a subset" << "\n";
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
            cerr << "ERROR: Couldn't parse the given input, please ensure the input is in valid .json format and is accessible." << "\n";
            exit(-1);
        }
    }
}

const void display_results(const vector<vector<string>> &results, const char &delim)
{
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
                cout << s << delim << " ";
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

const void display_results(const vector<string> &row, const char &delim)
{
    for (auto &s: row)
    {
        if (&s == &row.back())
        {
            cout << s;
        }
        else
        {
            cout << s << delim << " ";
        }
    }
    cout << "\n";
}