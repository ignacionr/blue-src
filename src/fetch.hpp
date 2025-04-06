#pragma once

#include <expected>
#include <string>
#include <curl/curl.h>

namespace fetch {
    size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
    
    std::expected<std::string, std::string> get(const std::string& url) {
        CURL* curl;
        CURLcode res;
        std::string readBuffer;
        std::expected<std::string, std::string> result;
    
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                result = std::unexpected(curl_easy_strerror(res));
            } else {
                result = readBuffer;
            }
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
        return result;
    }
}