/* size limits on read() */


// The size_t type is used for storing values used to measure size in bytes.
//
// The ssize_t type is a signed verion of size_t.


// Macro: ssize_t SSIZE_MAX
// The largest value that can fit in an object of type ssize_t. Effectively, this is
// the limit on the number of bytes that can be read or written in a single operation.
//
// This macro is defined in all POSIX systems because this limit is never configurable.

if (len > SSIZE_MAX){
        len = SSIZE_MAX;
}
