/** THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
 * BY HAND!!
 *
 * Generated by lcm-gen
 **/

#ifndef __lcm_sensor_msgs_LaserScan_hpp__
#define __lcm_sensor_msgs_LaserScan_hpp__

#include <lcm/lcm_coretypes.h>

#include <vector>
#include "Header.hpp"

namespace lcm_sensor_msgs
{

class LaserScan
{
    public:
        lcm_std_msgs::Header header;

        float      angle_min;

        float      angle_max;

        float      angle_increment;

        float      time_increment;

        float      scan_time;

        float      range_min;

        float      range_max;

        float      lidar_speed;

        int32_t    n_points;

        std::vector< float > ranges;

        std::vector< float > angles;

        std::vector< int16_t > intensities;

    public:
        LaserScan(): 
            angle_min(0.0f), 
            angle_max(0.0f), 
            angle_increment(0.0f), 
            time_increment(0.0f), 
            scan_time(0.0f), 
            range_min(0.0f), 
            range_max(0.0f), 
            lidar_speed(0.0f), 
            n_points(0)
        {}

    public:
        /**
         * Encode a message into binary form.
         *
         * @param buf The output buffer.
         * @param offset Encoding starts at thie byte offset into @p buf.
         * @param maxlen Maximum number of bytes to write.  This should generally be
         *  equal to getEncodedSize().
         * @return The number of bytes encoded, or <0 on error.
         */
        inline int encode(void *buf, int offset, int maxlen) const;

        /**
         * Check how many bytes are required to encode this message.
         */
        inline int getEncodedSize() const;

        /**
         * Decode a message from binary form into this instance.
         *
         * @param buf The buffer containing the encoded message.
         * @param offset The byte offset into @p buf where the encoded message starts.
         * @param maxlen The maximum number of bytes to read while decoding.
         * @return The number of bytes decoded, or <0 if an error occured.
         */
        inline int decode(const void *buf, int offset, int maxlen);

        /**
         * Retrieve the 64-bit fingerprint identifying the structure of the message.
         * Note that the fingerprint is the same for all instances of the same
         * message type, and is a fingerprint on the message type definition, not on
         * the message contents.
         */
        inline static int64_t getHash();

        /**
         * Returns "LaserScan"
         */
        inline static const char* getTypeName();

        // LCM support functions. Users should not call these
        inline int _encodeNoHash(void *buf, int offset, int maxlen) const;
        inline int _getEncodedSizeNoHash() const;
        inline int _decodeNoHash(const void *buf, int offset, int maxlen);
        inline static uint64_t _computeHash(const __lcm_hash_ptr *p);
};

int LaserScan::encode(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;
    int64_t hash = getHash();

    tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->_encodeNoHash(buf, offset + pos, maxlen - pos);
    if (tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int LaserScan::decode(const void *buf, int offset, int maxlen)
{
    int pos = 0, thislen;

    int64_t msg_hash;
    thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &msg_hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;
    if (msg_hash != getHash()) return -1;

    thislen = this->_decodeNoHash(buf, offset + pos, maxlen - pos);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int LaserScan::getEncodedSize() const
{
    return 8 + _getEncodedSizeNoHash();
}

int64_t LaserScan::getHash()
{
    static int64_t hash = static_cast<int64_t>(_computeHash(NULL));
    return hash;
}

const char* LaserScan::getTypeName()
{
    return "LaserScan";
}

int LaserScan::_encodeNoHash(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;

    tlen = this->header._encodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_encode_array(buf, offset + pos, maxlen - pos, &this->angle_min, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_encode_array(buf, offset + pos, maxlen - pos, &this->angle_max, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_encode_array(buf, offset + pos, maxlen - pos, &this->angle_increment, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_encode_array(buf, offset + pos, maxlen - pos, &this->time_increment, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_encode_array(buf, offset + pos, maxlen - pos, &this->scan_time, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_encode_array(buf, offset + pos, maxlen - pos, &this->range_min, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_encode_array(buf, offset + pos, maxlen - pos, &this->range_max, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_encode_array(buf, offset + pos, maxlen - pos, &this->lidar_speed, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &this->n_points, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    if(this->n_points > 0) {
        tlen = __float_encode_array(buf, offset + pos, maxlen - pos, &this->ranges[0], this->n_points);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    if(this->n_points > 0) {
        tlen = __float_encode_array(buf, offset + pos, maxlen - pos, &this->angles[0], this->n_points);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    if(this->n_points > 0) {
        tlen = __int16_t_encode_array(buf, offset + pos, maxlen - pos, &this->intensities[0], this->n_points);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    return pos;
}

int LaserScan::_decodeNoHash(const void *buf, int offset, int maxlen)
{
    int pos = 0, tlen;

    tlen = this->header._decodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_decode_array(buf, offset + pos, maxlen - pos, &this->angle_min, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_decode_array(buf, offset + pos, maxlen - pos, &this->angle_max, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_decode_array(buf, offset + pos, maxlen - pos, &this->angle_increment, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_decode_array(buf, offset + pos, maxlen - pos, &this->time_increment, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_decode_array(buf, offset + pos, maxlen - pos, &this->scan_time, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_decode_array(buf, offset + pos, maxlen - pos, &this->range_min, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_decode_array(buf, offset + pos, maxlen - pos, &this->range_max, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_decode_array(buf, offset + pos, maxlen - pos, &this->lidar_speed, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &this->n_points, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    if(this->n_points) {
        this->ranges.resize(this->n_points);
        tlen = __float_decode_array(buf, offset + pos, maxlen - pos, &this->ranges[0], this->n_points);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    if(this->n_points) {
        this->angles.resize(this->n_points);
        tlen = __float_decode_array(buf, offset + pos, maxlen - pos, &this->angles[0], this->n_points);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    if(this->n_points) {
        this->intensities.resize(this->n_points);
        tlen = __int16_t_decode_array(buf, offset + pos, maxlen - pos, &this->intensities[0], this->n_points);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    return pos;
}

int LaserScan::_getEncodedSizeNoHash() const
{
    int enc_size = 0;
    enc_size += this->header._getEncodedSizeNoHash();
    enc_size += __float_encoded_array_size(NULL, 1);
    enc_size += __float_encoded_array_size(NULL, 1);
    enc_size += __float_encoded_array_size(NULL, 1);
    enc_size += __float_encoded_array_size(NULL, 1);
    enc_size += __float_encoded_array_size(NULL, 1);
    enc_size += __float_encoded_array_size(NULL, 1);
    enc_size += __float_encoded_array_size(NULL, 1);
    enc_size += __float_encoded_array_size(NULL, 1);
    enc_size += __int32_t_encoded_array_size(NULL, 1);
    enc_size += __float_encoded_array_size(NULL, this->n_points);
    enc_size += __float_encoded_array_size(NULL, this->n_points);
    enc_size += __int16_t_encoded_array_size(NULL, this->n_points);
    return enc_size;
}

uint64_t LaserScan::_computeHash(const __lcm_hash_ptr *p)
{
    const __lcm_hash_ptr *fp;
    for(fp = p; fp != NULL; fp = fp->parent)
        if(fp->v == LaserScan::getHash)
            return 0;
    const __lcm_hash_ptr cp = { p, LaserScan::getHash };

    uint64_t hash = 0xeb135bc1992b06eaLL +
         lcm_std_msgs::Header::_computeHash(&cp);

    return (hash<<1) + ((hash>>63)&1);
}

}

#endif