#include "../include/definitions.h"
using namespace std;

unordered_map<string, string> cache;
vector<string> cache_order;

void initialize_cache() {
    cache.clear();
    cache_order.clear();
}

string get_from_cache(string url) {
    if (cache.find(url) != cache.end()) {
        cache_order.erase(remove(cache_order.begin(), cache_order.end(), url), cache_order.end());
        cache_order.push_back(url);
        return cache[url];
    }
    return "";
}

void add_to_cache(string url, string content) {
    if (cache.find(url) != cache.end()) {
        cache_order.erase(remove(cache_order.begin(), cache_order.end(), url), cache_order.end());
    }

    if (cache.size() >= CACHE_SIZE) {
        string oldest = cache_order.front();
        cache.erase(oldest);
        cache_order.erase(cache_order.begin());
    }

    cache[url] = content;
    cache_order.push_back(url);
}
