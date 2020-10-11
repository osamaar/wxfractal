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

        LSystem() : alphabet(), axiom(), rules(), level(0) { }

        // State peek_state();
        // State pop_state();
        // void push_state(State &&new_state) { state.push(new_state); }

        bool add_element(Element &&element);
        bool add_constants(const std::string &s);
        bool add_variables(const std::string &s);
        bool set_axiom(const std::string &a);
        bool add_rule(ElementID predecessor, const std::string &successor);

        void apply(int n);
        void reset();
        std::string as_string() const;
    };

    ////////////////////////////////////////////////////////////////////////////
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
        // a.substr()
        auto tokens = tokenize(a, " ");
        axiom.insert(axiom.end(), tokens.begin(), tokens.end());

        return true;
    }

    inline bool LSystem::add_rule(ElementID predecessor, const std::string &successor) {
        auto tokens = tokenize(successor, " ");
        rules[predecessor] = tokens;
        return true;
    }

    inline void LSystem::apply(int n) {
        level = n;
        reset();

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

    inline void LSystem::reset() {
        // state = {};
        sequence = axiom;
    }

    inline std::string LSystem::as_string() const {
        std::string str;
        for (auto &&id : sequence) {
            str.append(id);
            str.append(" ");
        }
        if (!str.empty()) {
            str.pop_back();
        }

        return str;
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
