# combigen
An efficient CLI tool to generate possible combinations written in C++

## Introduction
Combigen aims to assist with data generation and exploration. Given a `.json` input where each key contains an array of string values, combigen can either generate every possible combination or a random subset of the possible combinations. It aims to be memory-efficient while maintaining high-performance. This can be especially useful when large amounts of data are needed for statistical analysis or mock data in an application.

It supports output as `.csv` and `.json`.

## Usage

Basic commands are listed below:


```
Usage: combigen [options]
   -h             Displays this help message

   -a             Generates every possible combination
                  (Note: this should be used with caution when storing to disk)

   -n <index>     Generate combination at nth index

   -i <input>     Take the given .json file as input. Otherwise, input will come
                  from stdin.
                  Example: "{ "foo": [ "a", "b", "c" ], "bar": [ "1", "2" ] }"

   -t <type>      Output type (csv or json). Defaults to csv

   -r <size>      Generate a random sample of size r from
                  the possible set of combinations

   -d <delimiter> Set the delimiter when displaying combinations (default is ',')

   -k             Display the keys on the first line of output (for .csv)

   -v             Display version number
```

## Installation


### Linux/UNIX

1. Clone the repository and `cd` into it:

```
$ git clone https://github.com/iamtheburd/combigen.git && cd combigen
```

2. Build with `make`:

```
$ make
```

3. Install:

```
$ sudo make install
```

Alternatively, you can also check out the [Releases](https://github.com/iamtheburd/combigen/releases) tab and directly download the `combigen` binary from there.

### Windows


1. Download Visual Studio and install first

2. Clone the repository to some directory

3. Open up the Developer Command Prompt (can usually be found by searching in the Start menu)

4. `cd` to where your cloned repository is

5. Build the file:

```
> cl src\combigen.cpp /EHsc /O2
```

6. Place the resulting `combigen.exe` wherever you desire

Alternatively, you can also check out the [Releases](https://github.com/iamtheburd/combigen/releases) tab and directly download the `combigen.exe` from there.

## Usage

Using the example `.json` data provided, here are some examples showcasing some features:

### Input

You can either use the `-i` flag and specify an input `.json` file:


```
$ combigen -i example_data/combinations.json -n 100  # Find the combination at index 100
```

Or you can feed in an input from `stdin`:


```
$ cat example_data/combinations.json | combigen -n 100  # Find the combination at index 100
```


### Output

It's recommended to use your OS's built-in output redirection to write out to a file for ease-of-use and performance:

```
$ combigen -i example_data/combinations.json -r 50000 > output.txt  # Generate 50,000 random combinations
                                                                    # and store them in output.txt
```


### Types

You can export in either `.csv` or `.json`. Use the `-t` flag to explicitly set the output:

#### CSV

If you need the first row to contain column headers, you can also use the `-k` flag to display the keys as column headers:


```
$ combigen -i example_data/combinations.json -r 5 -k  # Generate 5 random combinations and display the keys
Age,First Name,Last Name,Number of Children,Number of Pets,Primary Desktop OS,Primary Mobile Phone OS,Residence,State/Territory
35,Kevin,Long,0,4,macOS,iOS,House,MD
45,Samantha,Thomas,3,1,Linux,Windows,Other,IL
50,Katherine,Williams,4,1,macOS,Windows,Other,TN
60,Kevin,Johnson,5+,1,Windows,Other,Other,NE
90,Sally,Wilson,5+,0,BSD,Windows,RV,PR
$
```

You can also change the delimiter with the `-d` flag:

```
$ combigen -i example_data/combinations.json -r 3 -k -d "|"  # Generate 3 random combinations, display the keys,
                                                             # and set the delimiter to ||
Age||First Name||Last Name||Number of Children||Number of Pets||Primary Desktop OS||Primary Mobile Phone OS||Residence||State/Territory
20||Samantha||Harris||3||4||Windows||Other||RV||GA
25||Matthew||Thomas||2||0||Windows||Other||Town Home||IL
80||James||Jones||1||3||macOS||iOS||House||FM
$                                                               
```

#### JSON

Use the `-t` flag to specify a JSON output:


```
$ combigen -i example_data/combinations.json -r 5 -t json  # Generate 5 random combinations in .json format
[
{
    "Age": "40",
    "First Name": "Matthew",
    "Last Name": "Harris",
    "Number of Children": "5+",
    "Number of Pets": "3",
    "Primary Desktop OS": "Other",
    "Primary Mobile Phone OS": "iOS",
    "Residence": "RV",
    "State/Territory": "UT"
},{
    "Age": "50",
    "First Name": "Kimberly",
    "Last Name": "Anderson",
    "Number of Children": "3",
    "Number of Pets": "3",
    "Primary Desktop OS": "Linux",
    "Primary Mobile Phone OS": "Windows",
    "Residence": "Apartment",
    "State/Territory": "RI"
},{
    "Age": "70",
    "First Name": "Kevin",
    "Last Name": "Torres",
    "Number of Children": "5+",
    "Number of Pets": "0",
    "Primary Desktop OS": "Other",
    "Primary Mobile Phone OS": "iOS",
    "Residence": "Apartment",
    "State/Territory": "LA"
},{
    "Age": "80",
    "First Name": "Sally",
    "Last Name": "Gonzales",
    "Number of Children": "1",
    "Number of Pets": "1",
    "Primary Desktop OS": "BSD",
    "Primary Mobile Phone OS": "Windows",
    "Residence": "Condo",
    "State/Territory": "TN"
},{
    "Age": "90",
    "First Name": "Brooke",
    "Last Name": "Wilson",
    "Number of Children": "1",
    "Number of Pets": "4",
    "Primary Desktop OS": "Other",
    "Primary Mobile Phone OS": "Android",
    "Residence": "House",
    "State/Territory": "CO"
}]
$
```


## Third-Party Libraries

Combigen uses the following open-source libraries:

* [nlohmann/json](https://github.com/nlohmann/json) - An excellent C++ library for parsing JSON

* [lazy-cartesian-product](https://github.com/iamtheburd/lazy-cartesian-product) - Small C++ library I developed to generate the combinations

* [skandhurkat/Getopt-for-Visual-Studio](https://github.com/skandhurkat/Getopt-for-Visual-Studio) - Port of the MinGW version of `getopt.h` so that the CLI works on Windows


## Contributing
Pull-requests are always welcome

## License
Licensed under GPLv3, see [LICENSE](https://github.com/iamtheburd/blob/master/LICENSE)
