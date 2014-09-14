/*
 * buffer.c
 *
 *  Created on: 13 maj 2013
 *      Author: benjamin
 */

#include "utility.h"

namespace utility {

void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index) {
    buffer[(*index)++] = number >> 24;
    buffer[(*index)++] = number >> 16;
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}

void buffer_append_uint32(uint8_t* buffer, uint32_t number, int32_t *index) {
    buffer[(*index)++] = number >> 24;
    buffer[(*index)++] = number >> 16;
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}

void buffer_append_int16(uint8_t* buffer, int16_t number, int32_t *index) {
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}

void buffer_append_uint16(uint8_t* buffer, uint16_t number, int32_t *index) {
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}

int16_t buffer_get_int16(const uint8_t *buffer, int32_t *index) {
    int16_t res =	((uint8_t) buffer[*index]) << 8 |
                    ((uint8_t) buffer[*index + 1]);
    *index += 2;
    return res;
}

uint16_t buffer_get_uint16(const uint8_t *buffer, int32_t *index) {
    uint16_t res = 	((uint8_t) buffer[*index]) << 8 |
                    ((uint8_t) buffer[*index + 1]);
    *index += 2;
    return res;
}

int32_t buffer_get_int32(const uint8_t *buffer, int32_t *index) {
    int32_t res =	((uint8_t) buffer[*index]) << 24 |
                    ((uint8_t) buffer[*index + 1]) << 16 |
                    ((uint8_t) buffer[*index + 2]) << 8 |
                    ((uint8_t) buffer[*index + 3]);
    *index += 4;
    return res;
}

uint32_t buffer_get_uint32(const uint8_t *buffer, int32_t *index) {
    uint32_t res =	((uint8_t) buffer[*index]) << 24 |
                    ((uint8_t) buffer[*index + 1]) << 16 |
                    ((uint8_t) buffer[*index + 2]) << 8 |
                    ((uint8_t) buffer[*index + 3]);
    *index += 4;
    return res;
}

}
