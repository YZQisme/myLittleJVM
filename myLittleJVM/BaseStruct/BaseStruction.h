//
// Created by YZQ on 25-8-5.
//

#ifndef BASESTRUCTION_H
#define BASESTRUCTION_H
#include <cstdint>
#include <unordered_map>
#include <vector>

struct myClassLoader;
struct MCP_String;
extern std::unordered_map<char,int> TypeMapToSlotSize;
struct Execute;
extern int baseClassLoaded;
extern std::unordered_map<char,int> TypeToBtyeSize;
extern std::vector<MCP_String*> stringsBeforeClassLoaded;

struct hashCodeGen {
    int32_t nextCode=0;
    int32_t getCode(){return nextCode++;}
};
extern hashCodeGen my_hash_code;
#endif //BASESTRUCTION_H
