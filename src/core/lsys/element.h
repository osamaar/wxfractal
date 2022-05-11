#pragma once

#include <string>
#include <memory>

namespace LSys {
    using ElementID = std::string;
    enum ElementType { Constant, Variable };

    template <typename T> struct Elementdata;

    struct ElementDataGeneric {
        // Here be dragons.
        template <typename T>
        T get_value() {
            return static_cast<Elementdata<T>*>(this)->value;
        }

        template <typename T>
        void set_value(T data) {
            static_cast<Elementdata<T>*>(this)->value = data;
        }
    };

    template <typename T>
    struct Elementdata : public ElementDataGeneric {
        T value;
        Elementdata(T value) : value(value) { }
    };

    struct Element {
        using DataPtr = std::unique_ptr<ElementDataGeneric>;
        ElementID id;
        ElementType type_;
        DataPtr data;

        // Element(Element &&element) = default;

        Element(ElementID id, ElementType type_=Variable) 
            : id(id), type_(type_), data(nullptr)
        {

        }

        template <typename T>
        Element(ElementID id, ElementType type_, T data) 
            : id(id), type_(type_)
        {
            this->data = std::move(std::unique_ptr<ElementDataGeneric>(new Elementdata<T>{data}));
        }

        template <typename T>
        static Element with_data(const ElementID &id, T data, ElementType type_=Variable) {
            return {id, type_, data};
        }
    };

}