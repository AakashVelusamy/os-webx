#ifndef CACHE_H
#define CACHE_H

#include <string>

void initialize_cache();  // Add this function
bool cache_get(const std::string& url, std::string& content);
void cache_put(const std::string& url, const std::string& content);

#endif // CACHE_H
