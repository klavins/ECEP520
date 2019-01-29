#include <assert.h>
#include <stdexcept>
#include "double_array.h"

// Default constructor
DoubleArray::DoubleArray() {
    buffer = new double[INITIAL_CAPACITY]();
    capacity = INITIAL_CAPACITY;    
    origin = capacity / 2;
    end = origin;
}

// Range constructor
DoubleArray::DoubleArray(double a, double b, double step) : DoubleArray() {
    int i = 0;
    for ( double x=a; x<=b; x += step ) {
        set(i, x);
        i+=1;
    }
}

// Copy constructor: i.e DoubleArray b(a) where a is a DoubleArray
DoubleArray::DoubleArray(const DoubleArray& other) : DoubleArray() {
    *this = other;
}

// Assignment operator: i.e DoubleArray b = a 
DoubleArray& DoubleArray::operator=(const DoubleArray& other) {
    if ( this != &other) {
        delete[] buffer; // don't forget this or you'll get a memory leak!
        buffer = new double[other.capacity]();
        capacity = other.capacity;
        origin = other.origin;
        end = origin;
        for ( int i=0; i<other.size(); i++) {
            set(i,other.get(i));
        }
    }
    return *this;
}

// Destructor
DoubleArray::~DoubleArray() {
    delete[] buffer;
}

// Getters
double DoubleArray::get(int index) const {
    if (index < 0) {
        throw std::range_error("Negative index in array");
    }
    if ( index >= size() ) {
        return 0;
    } else {
        return buffer[index_to_offset(index)];
    }
}

int DoubleArray::size() const {
    return end - origin;
}

// Setters
void DoubleArray::set(int index, double value) {
    if (index < 0) {
        throw std::range_error("Negative index in array");
    }
    while ( out_of_buffer(index_to_offset(index) ) ) {
        extend_buffer();
    }
    buffer[index_to_offset(index)] = value;
    if ( index >= size() ) {
        end = index_to_offset(index+1);
    }
}

// Private methods

int DoubleArray::index_to_offset ( int index ) const {
    return index + origin;
}

/* Position of the element at buffer position 'offset' */
int DoubleArray::offset_to_index ( int offset ) const  {
    return offset - origin;
}

/* Non-zero if and only if offset lies ouside the buffer */
bool DoubleArray::out_of_buffer ( int offset ) const {
    return offset < 0 || offset >= capacity;
}

/* Makes a new buffer that is twice the size of the old buffer,
   copies the old information into the new buffer, and deletes
   the old buffer */
void DoubleArray::extend_buffer() {

    double * temp = new double[2 * capacity]();
    int new_origin = capacity - (end - origin)/2,
           new_end = new_origin + (end - origin);

    for ( int i=0; i<size(); i++ ) {
        temp[new_origin+i] = get(i);
    }

    delete[] buffer;
    buffer = temp;

    capacity = 2 * capacity;
    origin = new_origin;
    end = new_end;

    return;

}