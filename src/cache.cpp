#include "../include/definitions.h"
#include "../include/cache.h"
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

unordered_map<string, string> cache_map;  // Stores URL -> Response
vector<string> cache_order;               // Stores order of cache entries

void initialize_cache() {
    cache_map.clear();
    cache_order.clear();
}

// Function to check if a URL is in cache and get its response
bool cache_get(const string& url, string& response) {
    auto it = cache_map.find(url);
    if (it != cache_map.end()) {
        response = it->second;

        // Move accessed URL to the back (MRU)
        auto vec_it = find(cache_order.begin(), cache_order.end(), url);
        if (vec_it != cache_order.end()) {
            cache_order.erase(vec_it);
            cache_order.push_back(url);
        }

        return true; // Cache hit
    }
    return false; // Cache miss
}

// Function to store a new response in cache
void cache_put(const string& url, const string& content) {
    // Remove existing entry if present
    auto it = find(cache_order.begin(), cache_order.end(), url);
    if (it != cache_order.end()) {
        cache_order.erase(it);
    }

    // Evict the oldest entry if cache is full
    if (cache_order.size() >= CACHE_SIZE) {
        string oldest = cache_order.front();
        cache_order.erase(cache_order.begin());
        cache_map.erase(oldest);
    }

    // Add new entry
    cache_map[url] = content;
    cache_order.push_back(url);
}

// Clear cache
void clear_cache() {
    cache_map.clear();
    cache_order.clear();
}

