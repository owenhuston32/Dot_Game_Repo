#pragma once
#include <vector>
#include <functional>
#include <algorithm>

enum class Screen {
    MainMenu,
    Gameplay,
    PauseMenu,
    Inventory,
    Settings,
    Bindings,
};

class GameState {
public:
    GameState() = default;

    bool isEmpty() const { return _screenStack.empty(); }

    Screen top() const {
        if (_screenStack.empty())
            return Screen::MainMenu; // default
        return _screenStack.back();
    }

    void push(Screen s) {
        _screenStack.push_back(s);
        notify();
    }

    void pop() {
        if (!_screenStack.empty()) {
            _screenStack.pop_back();
            notify();
        }
    }

    void clear() {
        if (!_screenStack.empty()) {
            _screenStack.clear();
            notify();
        }
    }

    void clearAndPush(Screen s) {
        _screenStack.clear();
        _screenStack.push_back(s);
        notify();
    }

    // Register a listener which will be called when the stack changes.
    // The listener receives the new top Screen as an argument.
    // Returns a listener id which can be used to remove the listener.
    size_t addListener(const std::function<void(Screen)>& cb) {
        size_t id = ++_lastId;
        _listeners.emplace_back(id, cb);
        return id;
    }

    void removeListener(size_t id) {
        _listeners.erase(std::remove_if(_listeners.begin(), _listeners.end(), [&](const auto& p) {
            return p.first == id;
        }), _listeners.end());
    }

private:
    void notify() {
        Screen current = top();
        for (const auto& p : _listeners) {
            const auto& cb = p.second;
            if (cb) cb(current);
        }
    }

    std::vector<Screen> _screenStack;
    std::vector<std::pair<size_t, std::function<void(Screen)>>> _listeners;
    size_t _lastId = 0;
};
