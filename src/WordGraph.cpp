#include "../include/WordGraph.hpp"

#include <sstream>

namespace Flanky {
    [[maybe_unused]] std::string WordGraph::printLinks() const {
        std::stringstream ss;

        for (const auto& [word, links] : adjanceyList) {
            ss << word << ":\n";
            for (const auto& [link_word, weight] : links) {
                ss << '.' << link_word << " : " << weight << '\n';
            }

            ss << " --- ";
        }

        return ss.str();
    }

    void WordGraph::createLinks(const std::string &fst, const std::string &snd) {
        auto word_it = words.find(fst);

        bool increment = true;

        if (word_it == words.end()) {
            addWords(fst);
            word_it = words.find(fst);
        }

        const std::string_view word_str_view(*word_it);

        auto link_word_it = words.find(snd);

        if (link_word_it == words.end()) {
            addWords(snd);
            link_word_it = words.find(snd);

            increment = false;
        }

        const std::string_view link_word_str_view(*link_word_it);

        if (increment) {
            adjanceyList[word_str_view][link_word_str_view] += 1;
        } else {
            adjanceyList[word_str_view][link_word_str_view] = 1;
        }
    }
} // Flanky