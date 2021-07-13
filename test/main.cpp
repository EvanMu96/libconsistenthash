#include <iostream>
#include <charconv>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "ConsistentHash.hpp"
#include "gtest/gtest.h"
#include "shim.hpp"

using namespace std;

static const int kNodeNum = 1024;
static const int kMaxStringSize = 2048;
static int kMaxStart = 256;
const char* prefix = "prefix";

typedef std::vector<std::string> VecRN;

size_t check(VecRN& lhs, VecRN& rhs) 
{
    if(lhs.size() != rhs.size())
    {
        return false;
    }
    
    size_t size = rhs.size();
    size_t diff = 0;
    for(size_t i = 0; i < size; ++i) 
    {
        // std::cout << "before: " << lhs[i] << ", after: " << rhs[i] << std::endl; 
        if(lhs[i] != rhs[i]) diff++;
    } 
    std::cout << "Number for rerouted key: " << diff << std::endl;
    std::cout << "Total key number:  " << kNodeNum << std::endl;
    return diff;
}

namespace {
    TEST(AddNodes, Positive) {
        srand (time(NULL));
        int start = rand() % kMaxStart;

        RingNode rn1("127.0.0.1:8080");
        RingNode rn2("127.0.0.1:8081");

        Ring r(5, MD5());
        r.add(rn1);
        r.add(rn2);

        VecRN before;
        for(int i = start; i < start + kNodeNum; ++i) 
        {
            char routeKey[kMaxStringSize];
            strcpy(routeKey, prefix);
            strcat(routeKey, std::to_string(i).c_str());
            before.push_back(r.find_node(routeKey).data);
        }   

        RingNode rn3("127.0.0.1:8082");
        r.add(rn3);
        
        VecRN after;
        for(int i = start; i < start + kNodeNum; ++i) 
        {
            char routeKey[kMaxStringSize];
            strcpy(routeKey, prefix);
            strcat(routeKey, std::to_string(i).c_str());
            after.push_back(r.find_node(routeKey).data);
        }  
        
        EXPECT_LE(check(before, after), kNodeNum / 2);
    }

    TEST(RemoveNodes, Positive) {
        srand (time(NULL));
        int start = rand() % kMaxStart;

        RingNode rn1("127.0.0.1:8080");
        RingNode rn2("127.0.0.1:8081");
        RingNode rn3("127.0.0.1:8082");

        Ring r(5, MD5());
        r.add(rn1);
        r.add(rn2);
        r.add(rn3);

        VecRN before;
        for(int i = start; i < start + kNodeNum; ++i) 
        {
            char routeKey[kMaxStringSize];
            strcpy(routeKey, prefix);
            strcat(routeKey, std::to_string(i).c_str());
            before.push_back(r.find_node(routeKey).data);
        }   

        r.remove(rn1);

        VecRN after;
        for(int i = start; i < start + kNodeNum; ++i) 
        {
            char routeKey[kMaxStringSize];
            strcpy(routeKey, prefix);
            strcat(routeKey, std::to_string(i).c_str());
            after.push_back(r.find_node(routeKey).data);
        }  
        
        EXPECT_LT(check(before, after), kNodeNum / 2);
    }

}

