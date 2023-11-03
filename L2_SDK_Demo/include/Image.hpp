/** THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
 * BY HAND!!
 *
 * Generated by lcm-gen
 **/

#ifndef __lcm_sensor_msgs_Image_hpp__
#define __lcm_sensor_msgs_Image_hpp__

#include <lcm/lcm_coretypes.h>

#include <string>
#include <vector>
#include "lcm_std_msgs/Header.hpp"

namespace lcm_sensor_msgs
{

class Image
{
    public:
        lcm_std_msgs::Header header;

        int32_t    height;

        int32_t    width;

        std::string encoding;

        int8_t     is_bigendian;

        int32_t    step;

        int32_t    n_data;

        std::vector< uint8_t > data;

    public:
        Image(): 
            height(0), 
            width(0), 
            is_bigendian(0), 
            step(0), 
            n_data(0)
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
         * Returns "Image"
         */
        inline static const char* getTypeName();

        // LCM support functions. Users should not call these
        inline int _encodeNoHash(void *buf, int offset, int maxlen) const;
        inline int _getEncodedSizeNoHash() const;
        inline int _decodeNoHash(const void *buf, int offset, int maxlen);
        inline static uint64_t _computeHash(const __lcm_hash_ptr *p);
};

int Image::encode(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;
    int64_t hash = getHash();

    tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->_encodeNoHash(buf, offset + pos, maxlen - pos);
    if (tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int Image::decode(const void *buf, int offset, int maxlen)
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

int Image::getEncodedSize() const
{
    return 8 + _getEncodedSizeNoHash();
}

int64_t Image::getHash()
{
    static int64_t hash = static_cast<int64_t>(_computeHash(NULL));
    return hash;
}

const char* Image::getTypeName()
{
    return "Image";
}

int Image::_encodeNoHash(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;

    tlen = this->header._encodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &this->height, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &this->width, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    char* encoding_cstr = const_cast<char*>(this->encoding.c_str());
    tlen = __string_encode_array(
        buf, offset + pos, maxlen - pos, &encoding_cstr, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->is_bigendian, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &this->step, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &this->n_data, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    if(this->n_data > 0) {
        tlen = __byte_encode_array(buf, offset + pos, maxlen - pos, &this->data[0], this->n_data);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    return pos;
}

int Image::_decodeNoHash(const void *buf, int offset, int maxlen)
{
    int pos = 0, tlen;

    tlen = this->header._decodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &this->height, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &this->width, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    int32_t __encoding_len__;
    tlen = __int32_t_decode_array(
        buf, offset + pos, maxlen - pos, &__encoding_len__, 1);
    if(tlen < 0) return tlen; else pos += tlen;
    if(__encoding_len__ > maxlen - pos) return -1;
    this->encoding.assign(
        static_cast<const char*>(buf) + offset + pos, __encoding_len__ - 1);
    pos += __encoding_len__;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->is_bigendian, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &this->step, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &this->n_data, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    if(this->n_data) {
        this->data.resize(this->n_data);
        tlen = __byte_decode_array(buf, offset + pos, maxlen - pos, &this->data[0], this->n_data);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    return pos;
}

int Image::_getEncodedSizeNoHash() const
{
    int enc_size = 0;
    enc_size += this->header._getEncodedSizeNoHash();
    enc_size += __int32_t_encoded_array_size(NULL, 1);
    enc_size += __int32_t_encoded_array_size(NULL, 1);
    enc_size += this->encoding.size() + 4 + 1;
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    enc_size += __int32_t_encoded_array_size(NULL, 1);
    enc_size += __int32_t_encoded_array_size(NULL, 1);
    enc_size += __byte_encoded_array_size(NULL, this->n_data);
    return enc_size;
}

uint64_t Image::_computeHash(const __lcm_hash_ptr *p)
{
    const __lcm_hash_ptr *fp;
    for(fp = p; fp != NULL; fp = fp->parent)
        if(fp->v == Image::getHash)
            return 0;
    const __lcm_hash_ptr cp = { p, Image::getHash };

    uint64_t hash = 0x6ea4a4eed3495c8eLL +
         lcm_std_msgs::Header::_computeHash(&cp);

    return (hash<<1) + ((hash>>63)&1);
}

}

#endif
