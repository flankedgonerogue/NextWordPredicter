#ifndef ASSIGNMENT3_WORDGRAPH_H
#define ASSIGNMENT3_WORDGRAPH_H

#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <iostream>

namespace Flanky {

    class WordGraph {
        std::unordered_set<std::string> words;
        std::map<std::string_view, std::map<std::string_view, int>> adjanceyList;

    public:
        /*
         * Recursive template to add variable number of strings
        */
        template <typename... _strs, typename = std::enable_if_t<std::conjunction_v<std::is_same<std::decay_t<_strs>, std::string>...>>>
        void addWords(_strs &&... strs) {
            (words.insert(std::forward<_strs>(strs)), ...);
        }

        void createLinks(const std::string& fst, const std::string& snd);

        std::vector<std::string_view> getTopFourLinks(const std::string& word)
        {
            std::multimap<int, std::string_view, std::greater<>> reverse_map;
            const auto word_node = adjanceyList.find(word);
            if (word_node == adjanceyList.end()) return {};
            for (auto& [fst, snd] : word_node->second)
            {
                reverse_map.insert({snd, fst});
            }

            std::vector<std::string_view> _words;

            int i = 4;
            for (auto it = reverse_map.begin(); it != reverse_map.end() && i != 0; ++it, --i)
            {
                _words.push_back(it->second);
            }

            return _words;
        }

        [[maybe_unused]] std::string printLinks() const;
    };

} // Flanky

#endif //ASSIGNMENT3_WORDGRAPH_H
