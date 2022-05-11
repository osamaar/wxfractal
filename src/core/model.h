#pragma once

#include "../common/observer.h"
#include "lsys/lsys.h"
#include <string>

struct Model {
    struct LSysDataParam {
        int level;
        std::string axiom;
        std::string rules;
    };

    struct LSysVisParam {
        double rot_plus;
        double rot_minus;
        double scale;
        double forward_step;
        std::string forward_synonyms;
    };

    struct LSysParam {
        LSysDataParam data;
        LSysVisParam vis;
    };

    LSys::LSystem lsys;
    // LSysParam parameters;
    LSysVisParam param_vis;
    std::vector<std::string> forward_synonyms;
    Observable<LSysParam> change_notifier;

    Model()
        : lsys()
        , param_vis()
        , change_notifier()
    {

    }

    // void set_level(int n) {
    //     parameters.level = n;
    //     change_notifier.notify(parameters);
    // }

    void set_param(const LSysParam &param) {
        // lsys.reset_sequence();
        init_lsys();
        param_vis = param.vis;
        lsys.alphabet.at("F").data->set_value<double>(param.vis.forward_step);
        lsys.alphabet.at("+").data->set_value<double>(param.vis.rot_plus);
        lsys.alphabet.at("-").data->set_value<double>(param.vis.rot_minus);
        add_synonym_multi<double>("F", param.vis.forward_synonyms);
        lsys.level = param.data.level;
        lsys.set_axiom(param.data.axiom);
        lsys.add_ruleset(param.data.rules);
        change_notifier.notify(param);
    }

    void apply() {
        lsys.apply();
        LSysParam param;
        param.data.level = lsys.level;
        param.data.axiom = lsys.axiom_as_string();
        param.data.rules = lsys.ruleset_as_string();
        param.vis.rot_plus = param_vis.rot_plus;
        param.vis.rot_minus = param_vis.rot_minus;
        param.vis.scale = param_vis.scale;
        param.vis.forward_step = param_vis.forward_step;
        change_notifier.notify(param);
    }

    void init_lsys() {
        lsys.reset_all();
        lsys.add_element(LSys::Element::with_data<double>("F", 300., LSys::Variable));
        lsys.add_element(LSys::Element::with_data<double>("+", 60., LSys::Constant));
        lsys.add_element(LSys::Element::with_data<double>("-", -60., LSys::Constant));
        forward_synonyms.clear();
        forward_synonyms.push_back("F");
    }

    template <typename T>
    void add_synonym_multi(const std::string &existing, const std::string &synonyms) {
        auto tokens = LSys::tokenize(synonyms, " ");
        for (auto &&token: tokens) {
            lsys.add_synonym<T>(existing, token);
            forward_synonyms.push_back(token);
        }
    }

    bool is_forward_synonym(const std::string &str) {
        for (auto&& s : forward_synonyms) {
            if (s == str) {
                return true;
            }
        }
        return false;
    }
};