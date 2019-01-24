#ifndef _ARBITRARY_ARRAY
#define _ARBITRARY_ARRAY

#define ARBITRARY_ARRAY_INITIAL_CAPACITY 10

typedef struct {
    int capacity,
        origin,
        end,
        element_size;
    char * buffer;
} ArbitraryArray;

/* Constructors / Destructors ************************************************/

ArbitraryArray * ArbitraryArray_new(int);
void ArbitraryArray_destroy(ArbitraryArray *);

/* Getters / Setters *********************************************************/

void ArbitraryArray_set_from_ptr(ArbitraryArray *, int, void *);
void * ArbitraryArray_get_ptr(const ArbitraryArray *, int);
int ArbitraryArray_size(const ArbitraryArray *);

/* Printing ******************************************************************/

void ArbitraryArray_print_debug_info(const ArbitraryArray *);

/* Operations ****************************************************************/

/* EXERCISES */

/*! Returns a string representation of the array, using a supplied to_string method for the element type.
 *  \param a The array
 *  \param element_to_string A method that creates a string representation of an element
 */
char * ArbitraryArray_to_string(const ArbitraryArray * a, char * (*element_to_string) (void *) );

#endif