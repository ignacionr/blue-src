#pragma once
#include <regex>
#include <string>
#include <ranges>
#include <algorithm>

void parse(std::string_view html_contents, auto && callback)
{
    static const std::regex block_regex{R"(<a class="title"[^>]*>([^<]+)<\/a>\s*<div class="values">([\s\S]*?)(?:<a class="title"|<div class="tile update">|<\/div>\s*<\/div>\s*<div class="tile is-child">))"};
    static const std::regex value_regex{R"(<div class="label">([^<]+)<\/div>\s*<div class="val">([^<]+)<\/div>)"};
    
    auto blocks = std::ranges::subrange(
        std::cregex_iterator(html_contents.data(), html_contents.data() + html_contents.length(), block_regex),
        std::cregex_iterator()
    );

    for (const auto& block_match : blocks) {
        auto raw_title = block_match[1].str();
        auto inner_content = block_match[2].str();

        // Todos los matches
        auto values = std::ranges::subrange(
            std::sregex_iterator(inner_content.begin(), inner_content.end(), value_regex),
            std::sregex_iterator()
        );

        // Convertido a pares, y filtrados los vacíos
        auto processed_values = values 
            | std::views::transform([](const auto& match) -> std::pair<std::string_view, std::string_view> {
                return {std::string_view{&(*match[1].first), static_cast<size_t>(match[1].length())}, 
                        std::string_view{&(*match[2].first), static_cast<size_t>(match[2].length())}};
            })
            | std::views::filter([](const auto& pair) { return !pair.first.empty(); });

        for (const auto& [label, value] : processed_values) {
            callback(raw_title, label, value);
        }
    }
}
