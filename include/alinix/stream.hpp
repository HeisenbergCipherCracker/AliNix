#ifndef __ALINIX_KERNEL_STREAM_HPP
#define __ALINIX_KERNEL_STREAM_HPP

#ifndef __cplusplus
#warning "Including stream.hpp in C is not recommended"
#endif

#include <alinix/types.h>

class Stream
{
public:
    /**
     * Create a new instance of the stream class
    */
    Stream();
    /**
     * Delete the stream and free all the memory it has used
    */
    virtual ~Stream();

    /**
     * Read a byte from this stream
    */
    virtual char Read();
    /**
     * Write a byte to this stream buffer
    */
    virtual void Write(char byte);
    /**
     * How many bytes can currently be read?
    */
    virtual int Available();
};

#endif // __ALINIX_KERNEL_STREAM_HPP