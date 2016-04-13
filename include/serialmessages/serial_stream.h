
#ifndef SERIALMESSAGES_SERIAL_STREAM_H
#define SERIALMESSAGES_SERIAL_STREAM_H
#include <simplelogger/simplelogger.h>
#include <stdint.h>

namespace serialmessages
{
/**
    \class SerialStream
*/
class SerialStream
{
private:
    /* Floating point conversion structures */

    template<typename UintSz>
    struct FloatToUintT
    {
        union
        {
            float dataf;
            UintSz datau;
        };
    };
    typedef FloatToUintT<uint32_t> FloatToUint;

    template<typename UintSz>
    struct DoubleToUintT
    {
        union
        {
            double dataf;
            UintSz datau;
        };
    };
    typedef DoubleToUintT<uint64_t> DoubleToUint;

public:

    SerialStream(uint8_t * buffer, size_t max) : 
        buffer_(buffer), 
        length_max_(max),
        num_bytes_(0),
        read_idx_(0),
        checksum_(0)
    {
    }

    ~SerialStream()
    {
    }
    
    void reset()
    {
        num_bytes_ = 0;
        checksum_ = 0;
    }

    size_t size()
    {
        return num_bytes_;
    }
    
    /* Overloaded operators to serialize/deserialize types */

    /* Serialization */
    
    SerialStream& operator<<(const uint8_t rhs)
    {
        buffer_[num_bytes_++] = rhs;
        checksum_ += rhs;
        return *this;
    }

    SerialStream& operator<<(const uint16_t rhs)
    {
        for(int i = (sizeof(uint16_t) * 8) - 8; i >= 0; i -= 8)
        {
            uint8_t byte = (rhs & (0x00FF << i)) >> i;
            buffer_[num_bytes_++] = byte;
            checksum_ += byte;
        }

        return *this;
    }

    SerialStream& operator<<(const uint32_t rhs)
    {
        for(int i = (sizeof(uint32_t) * 8) - 8; i >= 0; i -= 8)
        {
            uint8_t byte = (rhs & (0x000000FF << i)) >> i;
            buffer_[num_bytes_++] = byte;
            checksum_ += byte;
        }
        return *this;
    }

    SerialStream& operator<<(const uint64_t rhs)
    {
        for(int i = (sizeof(uint64_t) * 8) - 8; i >= 0; i -= 8)
        {
            uint8_t byte = (rhs & (0x00000000000000FF << i)) >> i;
            buffer_[num_bytes_++] = byte;
            checksum_ += byte;
        }
        return *this;    
    }

    SerialStream& operator<<(const int8_t rhs)
    {
        buffer_[num_bytes_++] = rhs;
        checksum_ += rhs;
        return *this;
    }

    SerialStream& operator<<(const int16_t rhs)
    {
        for(int i = (sizeof(int16_t) * 8) - 8; i >= 0; i -= 8)
        {
            uint8_t byte = (rhs & (0x00FF << i)) >> i;
            buffer_[num_bytes_++] = byte;
            checksum_ += byte;
        }

        return *this;
    }

    SerialStream& operator<<(const int32_t rhs)
    {
        for(int i = (sizeof(int32_t) * 8) - 8; i >= 0; i -= 8)
        {
            uint8_t byte = (rhs & (0x000000FF << i)) >> i;
            buffer_[num_bytes_++] = byte;
            checksum_ += byte;
        }
        return *this;
    }

    SerialStream& operator<<(const int64_t rhs)
    {
        for(int i = (sizeof(int64_t) * 8) - 8; i >= 0; i -= 8)
        {
            uint8_t byte = (rhs & (0x00000000000000FF << i)) >> i;
            buffer_[num_bytes_++] = byte;
            checksum_ += byte;
        }
        return *this;    
    }

    SerialStream& operator<<(float rhs)
    {
        FloatToUint float_to_uint;
        float_to_uint.dataf = rhs;

        *this << float_to_uint.datau;

        return *this;
    }

    SerialStream& operator<<(double rhs)
    {
        DoubleToUint double_to_unit;
        double_to_unit.dataf = rhs;

        *this << double_to_unit.datau;

        return *this;
    }

    /* Deserialization */

    SerialStream& operator>>(uint8_t& rhs)
    {
        rhs = buffer_[read_idx_++];
        return *this;
    }

    SerialStream& operator>>(uint16_t& rhs)
    {
        rhs = 0;
        rhs |= ((buffer_[read_idx_++] & 0x00FF) << 8);
        rhs |= (buffer_[read_idx_++] & 0x00FF);

        return *this;
    }

    SerialStream& operator>>(uint32_t& rhs)
    {
        rhs = 0;
        rhs |= ((buffer_[read_idx_++] & 0x000000FF) << 24);
        rhs |= ((buffer_[read_idx_++] & 0x000000FF) << 16);
        rhs |= ((buffer_[read_idx_++] & 0x000000FF) << 8);
        rhs |= ((buffer_[read_idx_++] & 0x000000FF));

        return *this;
    }

    SerialStream& operator>>(uint64_t& rhs)
    {
        rhs = 0;
        rhs |= (((uint64_t)buffer_[read_idx_++] & 0x00000000000000FF) << 56);
        rhs |= (((uint64_t)buffer_[read_idx_++] & 0x00000000000000FF) << 48);
        rhs |= (((uint64_t)buffer_[read_idx_++] & 0x00000000000000FF) << 40);
        rhs |= (((uint64_t)buffer_[read_idx_++] & 0x00000000000000FF) << 32);
        rhs |= (((uint64_t)buffer_[read_idx_++] & 0x00000000000000FF) << 24);
        rhs |= (((uint64_t)buffer_[read_idx_++] & 0x00000000000000FF) << 16);
        rhs |= (((uint64_t)buffer_[read_idx_++] & 0x00000000000000FF) << 8);
        rhs |= (((uint64_t)buffer_[read_idx_++] & 0x00000000000000FF));

        return *this;
    }

    SerialStream& operator>>(float& rhs)
    {
        FloatToUint float_to_uint;
        
        *this >> float_to_uint.datau;
        rhs = float_to_uint.dataf;

        return *this;
    }

    SerialStream& operator>>(double& rhs)
    {
        DoubleToUint double_to_unit;
        
        *this >> double_to_unit.datau;
        rhs = double_to_unit.dataf;

        return *this;
    }

private:
    uint8_t* buffer_; ///< Pointer to a buffer of data
    size_t length_max_;   ///< Length of the buffer
    size_t num_bytes_; ///< bytes in the serial stream
    size_t read_idx_;
    uint8_t checksum_;
};
}

#endif // SERIALMESSAGES_SERIAL_STREAM_H
