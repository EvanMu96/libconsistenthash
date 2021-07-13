// author: Sen Mu
// 2021.06.17
#pragma once
#include <string>
#include <set>
#include <utility>
#include <cstdint>
#include <functional>
#include <map>

using HashFunc = std::function<std::string(const std::string& text)>;

struct RingNode {
    RingNode() = default;
    RingNode(const std::string& host_addr): data(host_addr) {}
    RingNode(const RingNode& rhs) = default;
    std::string data;
    bool operator<(const RingNode& rn) const {
        return data < rn.data;
    }
    bool operator==(const RingNode& rn) const {
        return data == rn.data;
    }
};

class Ring {
public:
    Ring(size_t max_size, HashFunc hash_func);

    void add(const RingNode& node);

    bool remove(const RingNode& node);
    
    RingNode find_node(const std::string& key) const;
protected:
    std::map<uint64_t, RingNode> get_nodes() const;

private:
    uint64_t hash(const std::string& key) const;

    std::map<uint64_t, RingNode> _nodes;
    size_t _max_size;
    HashFunc _hash;
};
