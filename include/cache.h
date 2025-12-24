#ifndef CACHE_H
#define CACHE_H

#include <string>

void initialize_cache();
bool cache_get(const std::string& url, std::string& content);
void cache_put(const std::string& url, const std::string& content);
void clear_cache();

#endif // CACHE_H

