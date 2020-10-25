#pragma once

#include <vector>
#include <functional>

template <typename T> class Observer;

template <typename T>
class Observable {
public:
    Observable();
    void add_observer(Observer<T> &observer);
    void remove_observer(Observer<T> &observer);
    void notify();
    void notify(T data);
    T get_data();
    void set_data(T data);
    int observer_count() { return m_observers.size(); }
private:
    std::vector<Observer<T>*> m_observers;
    T m_data;
};

template <typename T>
class Observer {
public:
    Observer(std::function<void(T)> callback);

    template <typename Cls>
    Observer(void (Cls::*f)(T), Cls *instance);

    void notify(T data);
private:
    std::function<void(T)> m_callback;
};


template <typename T>
Observable<T>::Observable() : m_observers(), m_data() { }

template <typename T>
void Observable<T>::add_observer(Observer<T> &observer) {
    m_observers.push_back(&observer);
}

template <typename T>
void Observable<T>::remove_observer(Observer<T> &observer) {
    for (auto it = m_observers.begin(); it != m_observers.end(); ++it) {
        if (*it == &observer) {
            m_observers.erase(it);
            break;
        }
    }
}

template <typename T>
void Observable<T>::notify() {
    for (auto &&observer: m_observers) {
        observer->notify(m_data);
    }
}

template <typename T>
void Observable<T>::notify(T data) {
    set_data(data);
    notify();
}

template <typename T>
T Observable<T>::get_data() {
    return m_data;
}

template <typename T>
void Observable<T>::set_data(T data) {
    m_data = data;
}

template <typename T>
Observer<T>::Observer(std::function<void(T)> callback) : m_callback(callback) { }

template <typename T>
template <typename Cls>
Observer<T>:: Observer(void (Cls::*f)(T), Cls *instance) {
    using namespace std::placeholders;
    auto cb = std::bind(f, instance, _1);
    m_callback = std::function<void(T)>(cb);
}

template <typename T>
void Observer<T>::notify(T data) {
    m_callback(data);
}
