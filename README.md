[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

AIGoFuzzing : Mutation-Based AIG Fuzzer
=================================================

To compile use `./configure.sh` and then `make`.

Usage:
----------------------------------

  `./aigofuzzing  <inp> [out] [-h] [-m n] [-s n] [-t n]`

Mandatory:  


     inp     name of input file that contains the AIG

Optional:  


    out     name of output file (if ommited no output file is generated)   
    -h      prints this help  
    -m n    sets the number of modifications to 'n', (default: 1)  
    -s n    sets the seed to 'n', (default: randomly generated)  
    -t n    sets the selected technique to 'n' (default: random number)  
            possible values: 1 switch internal signs  
                             2 switch sign of output  
                             3 modify input  
                             4 insert constant  
