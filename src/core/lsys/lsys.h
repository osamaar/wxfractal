#pragma once

#include <stack>
#include <cstdio>
#include <vector>
#include <map>
#include <string>
#include "element.h"

namespace LSys {
    using Sequence = std::vector<ElementID>;

    std::vector<std::string> tokenize(const std::string &s, const std::string &delim);

    struct State {
        double rotation = 0;
        double x = 0;
        double y = 0;
    };

    struct LSystem {
        std::map<ElementID, Element> alphabet;

        Sequence axiom;
        std::map<ElementID, Sequence> rules;
        // std::stack<State> state;
        Sequence sequence;
        int level;

        static std::string sequence_to_str(Sequence seq);

        LSystem() : alphabet(), axiom(), rules(), level(0) { }

        // State peek_state();
        // State pop_state();
        // void push_state(State &&new_state) { state.push(new_state); }

        bool add_element(Element &&element);
        bool add_constants(const std::string &s);
        bool add_variables(const std::string &s);
        bool set_axiom(const std::string &a);
        bool add_rule(ElementID predecessor, const std::string &successor);
        bool add_rule(const std::string &rule);
        bool add_ruleset(const std::string &ruleset);

        template <typename T>
        void add_synonym(const ElementID &existing, const ElementID &synonym);

        template <typename T>
        void add_synonym_multi(const ElementID &existing, const std::string &synonyms);

        void apply(int n);
        void apply();
        void reset_sequence();
        void reset_all();
        std::string sequence_as_string() const;
        std::string axiom_as_string() const;
        std::string ruleset_as_string() const;
    };

    ////////////////////////////////////////////////////////////////////////////
    inline std::string LSystem::sequence_to_str(Sequence seq) {
        std::string str;
        for (auto &&id : seq) {
            str.append(id);
            str.append(" ");
        }
        if (!str.empty()) {
            str.pop_back();
        }

        return str;
    }

    inline bool LSystem::add_element(Element &&element) {
        alphabet.emplace(element.id, std::move(element));
        return true;
    }

    inline bool LSystem::add_constants(const std::string &s) {
        auto tokens = tokenize(s, " ");

        for (auto &&token : tokens) {
            add_element(Element(token, Constant));
        }

        return true;
    }

    inline bool LSystem::add_variables(const std::string &s) {
        auto tokens = tokenize(s, " ");

        for (auto &&token : tokens) {
            add_element(Element(token, Variable));
        }

        return true;
    }

    inline bool LSystem::set_axiom(const std::string &a) {
        auto tokens = tokenize(a, " ");
        axiom.clear();
        axiom.insert(axiom.end(), tokens.begin(), tokens.end());

        return true;
    }

    inline bool LSystem::add_rule(ElementID predecessor, const std::string &successor) {
        auto tokens = tokenize(successor, " ");
        rules[predecessor] = tokens;
        return true;
    }

    inline bool LSystem::add_rule(const std::string &rule) {
        auto tokens = tokenize(rule, ":");

        if (tokens.size() != 2) {
            return false;
        }

        return add_rule(tokens[0], tokens[1].substr(1, tokens[1].size()-1));
    }

    inline bool LSystem::add_ruleset(const std::string &ruleset) {
        auto tokens = tokenize(ruleset, "\n");
        for (auto&& token : tokens) {
            add_rule(token);
        }
        return true;
        printf("rules:\n%s\n", ruleset_as_string().c_str());
    }

    inline void LSystem::apply(int n) {
        level = n;
        apply();
    }

    inline void LSystem::apply() {
        reset_sequence();
        int n = level;
        sequence = axiom;

        while (n > 0) {
            Sequence seqnew;
            
            for (auto &&var : sequence) {
                bool found = false;

                for (auto &&rule : rules) {
                    if (rule.first == var) {
                        seqnew.insert(seqnew.end(), rule.second.begin(), rule.second.end());
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    seqnew.push_back(var);
                }
            }
        
            sequence =  seqnew;
            n--;
        }

    }

    inline void LSystem::reset_sequence() {
        // state = {};
        sequence = axiom;
    }

    inline void LSystem::reset_all() {
        alphabet.clear();
        axiom.clear();
        rules.clear();
        sequence.clear();
        level = 0;
    }

    inline std::string LSystem::sequence_as_string() const {
        return sequence_to_str(sequence);
    }

    inline std::string LSystem::axiom_as_string() const {
        return sequence_to_str(axiom);
    }

    inline std::string LSystem::ruleset_as_string() const {
        std::string str;
        for (auto&& rule : rules) {
            str.append(rule.first);
            str.append(": ");
            str.append(sequence_to_str(rule.second));
            str.append("\n");
        }

        // if (str.size() > 2) {
        //     str.erase(str.end()-2, str.end());
        // }

        return str;
    }

    template <typename T>
    void LSystem::add_synonym(const ElementID &existing, const ElementID &synonym) {
        auto &elem = alphabet.at(existing);
        auto value = elem.data->get_value<T>();
        auto type_ = elem.type_;
        add_element(LSys::Element::with_data<T>(synonym, value, type_));
    }

    template <typename T>
    void LSystem::add_synonym_multi(const ElementID &existing, const std::string &synonyms) {
        auto tokens = tokenize(synonyms, " ");
        for (auto &&token: tokens) {
            add_synonym(existing, token);
        }
    }

    inline std::vector<std::string> tokenize(const std::string &str, const std::string &delim) {
        std::vector<std::string> tokens;
        size_t pos = 0;
        std::string s{str};
        std::string token;

        while ((pos = s.find(delim)) != std::string::npos) {
            tokens.push_back(s.substr(0, pos));
            // std::cout << token << std::endl;
            // s.erase(0, pos + delim.length());
            s = s.substr(pos + delim.length());
        }

        tokens.push_back(s);

        return tokens;
    }
    //////////////////////////////////////////////////////////////////////////////
}
