//
// Created by YZQ on 25-8-5.
//

#include "BaseStruction.h"

std::unordered_map<char,int> TypeMapToSlotSize={
    {'B',1},{'C',1},{'D',2},{'F',1},{'I',1},{'J',2},{'L',1},{'S',1},{'Z',1},{'[',1}
};

int baseClassLoaded=0;

std::vector<MCP_String*> stringsBeforeClassLoaded{};

hashCodeGen my_hash_code{};

std::unordered_map<char,int> TypeToBtyeSize=
    {{'B',1},{'C',2},{'D',8},{'F',4},{'I',4},{'J',8},{'L',4},{'S',2},{'Z',1},{'[',4}};