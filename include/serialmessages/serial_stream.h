/**
    \file message_client.h
    \author Natesh Narain <nnaraindev@gmail.com>
*/
#ifndef SERIALMESSAGES_SERIAL_STREAM_H
#define SERIALMESSAGES_SERIAL_STREAM_H

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

    //! structure to do conversion between float and unsigned int
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

    //! structure to do conversion between double and unsigned int
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

    /**
        \param buffer pointer for a buffer for data
        \param max max buffer size
    */
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
    
    /**
        Reset serial stream. Serialization and deserialization begins at the start of buffer
    */
    void reset()
    {
        num_bytes_ = 0;
        read_idx_ = 0;
        checksum_ = 0;
    }

    /**
        \return number of bytes serialized
    */
    size_t size()
    {
        return num_bytes_;
    }
    
    /* Overloaded operators to serialize/deserialize types */

    /* Serialization */
    
    /**
        Serialize unsigned 8 bit integer

        @return this serial stream
    */
    SerialStream& operator<<(const uint8_t rhs)
    {
        buffer_[num_bytes_++] = rhs;
        checksum_ += rhs;
        return *this;
    }

    /**
        Serialize unsigned 16 bit integer

        @return this serial stream
    */
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

    /**
        Serialize unsigned 32 bit integer

        @return this serial stream
    */
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

    /**
        Serialize unsigned 64 bit integer

        @return this serial stream
    */
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

    /**
        Serialize signed 8 bit integer

        @return this serial stream
    */
    SerialStream& operator<<(const int8_t rhs)
    {
        buffer_[num_bytes_++] = rhs;
        checksum_ += rhs;
        return *this;
    }

    /**
        Serialize signed 16 bit integer

        @return this serial stream
    */
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

    /**
        Serialize signed 32 bit integer

        @return this serial stream
    */
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

    /**
        Serialize signed 64 bit integer

        @return this serial stream
    */
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

    /**
        Serialize float

        @return this serial stream
    */
    SerialStream& operator<<(float rhs)
    {
        FloatToUint float_to_uint;
        float_to_uint.dataf = rhs;

        *this << float_to_uint.datau;

        return *this;
    }

    /**
        Serialize double

        @return this serial stream
    */
    SerialStream& operator<<(double rhs)
    {
        DoubleToUint double_to_unit;
        double_to_unit.dataf = rhs;

        *this << double_to_unit.datau;

        return *this;
    }

    /**
        Serialize string

        @return this serial stream
    */
    SerialStream& operator<<(const char *str)
    {
        while(*str)
        {
            char c = *str++;
            buffer_[num_bytes_++] = c;
            checksum_ += c;
        }

        buffer_[num_bytes_++] = 0;

        return *this;
    }

    /* Deserialization */

    /**
        Deserialize unsigned 8 bit integer

        @return this serial stream
    */
    SerialStream& operator>>(uint8_t& rhs)
    {
        rhs = buffer_[read_idx_++];
        return *this;
    }

    /**
        Deserialize unsigned 16 bit integer

        @return this serial stream
    */
    SerialStream& operator>>(uint16_t& rhs)
    {
        rhs = 0;
        rhs |= ((buffer_[read_idx_++] & 0x00FF) << 8);
        rhs |= (buffer_[read_idx_++] & 0x00FF);

        return *this;
    }

    /**
        Deserialize unsigned 32 bit integer

        @return this serial stream
    */
    SerialStream& operator>>(uint32_t& rhs)
    {
        rhs = 0;
        rhs |= ((buffer_[read_idx_++] & 0x000000FF) << 24);
        rhs |= ((buffer_[read_idx_++] & 0x000000FF) << 16);
        rhs |= ((buffer_[read_idx_++] & 0x000000FF) << 8);
        rhs |= ((buffer_[read_idx_++] & 0x000000FF));

        return *this;
    }

    /**
        Deserialize unsigned 64 bit integer

        @return this serial stream
    */
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

    /**
        Deserialize float

        @return this serial stream
    */
    SerialStream& operator>>(float& rhs)
    {
        FloatToUint float_to_uint;
        
        *this >> float_to_uint.datau;
        rhs = float_to_uint.dataf;

        return *this;
    }

    /**
        Deserialize double

        @return this serial stream
    */
    SerialStream& operator>>(double& rhs)
    {
        DoubleToUint double_to_unit;
        
        *this >> double_to_unit.datau;
        rhs = double_to_unit.dataf;

        return *this;
    }

    /**
        Deserialize string

        @return this serial stream
    */
    SerialStream& operator>>(char * str)
    {
        while(buffer_[read_idx_])
        {
            *str++ = buffer_[read_idx_++];
        }

        *str++ = buffer_[read_idx_++];

        return *this;
    }

private:
    uint8_t* buffer_;   ///< Pointer to a buffer of data
    size_t length_max_; ///< Length of the buffer
    size_t num_bytes_;  ///< bytes in the serial stream
    size_t read_idx_;   ///< current index to read from
    uint8_t checksum_;  ///< checksum for stream stream
};
}

#endif // SERIALMESSAGES_SERIAL_STREAM_H
