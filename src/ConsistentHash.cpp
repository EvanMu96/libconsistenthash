#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include "ConsistentHash.hpp"
#include "shim.hpp"

uint64_t Ring::hash(const std::string& key) const{
    auto hash_val_str =  _md5(key).substr(0, 16);
    uint64_t hash_val = std::stoul(hash_val_str, nullptr, 16);
    return hash_val;
}

Ring::Ring(size_t max_size): _max_size(max_size) {}

void Ring::add(const RingNode& node) {
    if (_nodes.size() >= _max_size) {
        return;
    }
    
   _nodes[hash(node.data)] = node;
}

bool Ring::remove(const RingNode& node) {
    if (_nodes.empty()) {
        return false;
    }

    _nodes.erase(hash(node.data));
    return true;
}

RingNode Ring::find_node(const std::string& key) const{
    // short circuit when empty
    if(_nodes.empty()) {
        return RingNode{}; 
    }
    
    uint64_t hash_val = hash(key);

    auto it = _nodes.upper_bound(hash_val);
    if (it == _nodes.end()) {
        return _nodes.begin()->second;
    }
    return it->second;
}


std::map<uint64_t, RingNode> Ring::get_nodes() const { return _nodes; }


