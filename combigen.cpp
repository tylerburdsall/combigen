#include <iostream>
#include "../lazy-cartesian-product/lazy-cartesian-product.hpp"
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include "json.hpp"

using std::cout;
using std::vector;
using std::string;
using std::endl;
using std::ifstream;

using lazycp::lazy_cartesian_product;
using json = nlohmann::json;

int main(int argc, char* argv[])
{
    vector<vector<string>> combos = 
    {
        {"one", "two", "three", "four"},
        {"I", "declare", "a", "thumb", "war"},
        {"x", "y", "z"}
    };

    vector<vector<string>> result = lazy_cartesian_product::generate_samples(combos, 10);
    for( vector<string> row: result)
    {
        for( string s: row)
        {
            cout << " " << s;
        }
        cout << "\n";
    }

    cout << "command line args:\n";
    int c;
    while( (c = getopt(argc, argv, "h:o:s:t:")) != -1)
    {
        switch(c)
        {
            case 'h':
                if (optarg)
                {
                    cout << "-h was: " << optarg << "\n";
                }
                break;
            case 'o':
                if (optarg)
                {
                    cout << "-o was: " << optarg << "\n";
                }
                break;
            case 's':
                if (optarg)
                {
                    cout << "-s was: " << optarg << "\n";
                }
                break;
            case 't':
                if (optarg)
                {
                    cout << "-t was: " << optarg << "\n";
                }
                break;
            default: break;
        }
    }

    ifstream i("combinations.json");
    json j;
    i >> j;
    cout << std::setw(4) << j << std::endl;

    for(auto it = j.begin(); it != j.end(); ++it)
    {
        vector<string> vals = j[it.key()];
        for(string s : vals)
        {
            cout << s << "\n";
        }
    }

    return 0;
}
