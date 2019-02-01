#ifndef DOUBLE_ARRAY
#define DOUBLE_ARRAY

#include <iostream>

class DoubleArray {

public:

    // Constructors
    DoubleArray(); // Default constructor
    DoubleArray(double a, double b, double step); // Range constructor
    DoubleArray(const DoubleArray& other); // Copy constructor

    // Assignment
    DoubleArray& operator=(const DoubleArray& other);

    // Destructor
    ~DoubleArray();

    // Getters
    double get(int index) const;
    int size() const;

    // Setters
    void set(int index, double value);

private:

    int capacity,
        origin,
        end;

    double * buffer;

    const int INITIAL_CAPACITY = 10;

    int index_to_offset(int index) const;
    int offset_to_index(int offset) const;
    bool out_of_buffer(int offset) const;
    void extend_buffer(void);

};

#endif