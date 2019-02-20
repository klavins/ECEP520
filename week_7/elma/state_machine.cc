#include "elma.h"

namespace elma {

    StateMachine& StateMachine::set_initial(State& s) {
        _initial = &s;
        return *this;
    }

    StateMachine& StateMachine::add_transition(string event_name, State& from, State& to) {
        _transitions.push_back(Transition(event_name, from, to));
        return *this;
    }

    void StateMachine::init() {
        for (auto transition : _transitions ) {
            watch(transition.event_name(), [this, transition](const Event& e) {
                if ( _current == &transition.from() ) {
                    _current->exit();
                    _current = &transition.to();
                    _current->entry();
                }
            });
        }
    }

    void StateMachine::start() {
        _current = _initial;
        _current->entry();
    }

    void StateMachine::update() {
        _current->during();
    }

    void StateMachine::stop() {}

};