#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <chrono>

using namespace std::chrono;
using std::string;

class Process {

public:

  friend class Manager;

  typedef enum { UNINITIALIZED, INITIALIZED, STOPPED, RUNNING } status_type;

  Process(std::string name) : _name(name), _status(UNINITIALIZED) {}
  virtual ~Process() = default;

  // Interface for derived classes
  virtual void init() = 0;
  virtual void start() = 0;
  virtual void update() = 0;
  virtual void stop() = 0;

  // Getters
  inline string name() { return _name; }
  inline status_type status() { return _status; }
  inline high_resolution_clock::duration period() { return _period; }
  inline high_resolution_clock::duration last_update() { return _last_update; }

private:

  string _name;
  status_type _status;
  high_resolution_clock::duration _period,
                                  _last_update;

};

#endif 