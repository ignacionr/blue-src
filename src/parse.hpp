#pragma once
#include <regex>
#include <string>
#include <ranges>
#include <algorithm>

void parse(std::string_view html_contents, auto && callback)
{
    // Simple patterns to find block boundaries
    static const std::regex title_start_regex{R"(<a class="title"[^>]*>([^<]+)</a>)"};
    static const std::regex values_start_regex{R"(<div class="values">)"};
    static const std::regex block_end_regex{R"(<a class="title"|<div class="tile update">|</div>\s*</div>\s*<div class="tile is-child">)"};
    static const std::regex value_regex{R"(<div class="label">([^<]+)</div>\s*<div class="val">([^<]+)</div>)"};
    
    auto begin = html_contents.data();
    auto end = begin + html_contents.length();
    std::cmatch title_match, values_match, end_match;
    
    // Iterate through the HTML content
    while (std::regex_search(begin, end, title_match, title_start_regex)) {
        // Extract the title
        auto raw_title = title_match[1].str();
        begin = title_match[0].second;
        
        // Find the start of values section
        if (std::regex_search(begin, end, values_match, values_start_regex)) {
            begin = values_match[0].second;
            
            // Find the end of the current block
            if (std::regex_search(begin, end, end_match, block_end_regex)) {
                // Extract content between values start and block end
                std::string inner_content(begin, end_match[0].first);
                
                // Process values within the block
                auto values = std::ranges::subrange(
                    std::sregex_iterator(inner_content.begin(), inner_content.end(), value_regex),
                    std::sregex_iterator()
                );
                
                // Convert to pairs and filter empty ones
                auto processed_values = values 
                    | std::views::transform([](const auto& match) -> std::pair<std::string_view, std::string_view> {
                        return {std::string_view{&(*match[1].first), static_cast<size_t>(match[1].length())}, 
                                std::string_view{&(*match[2].first), static_cast<size_t>(match[2].length())}};
                    })
                    | std::views::filter([](const auto& pair) { return !pair.first.empty(); });
                
                for (const auto& [label, value] : processed_values) {
                    callback(raw_title, label, value);
                }
                
                // Move to the start of the next block
                begin = end_match[0].first;
            } else {
                // No end marker found, move to the end
                begin = end;
            }
        } else {
            // No values section found, move to the next title
            begin = end;
        }
    }
}
