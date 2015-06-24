/*
 * buffer.h
 *
 *  Created on: 13 maj 2013
 *      Author: benjamin
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdint.h>

namespace utility {

void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index);
void buffer_append_uint32(uint8_t* buffer, uint32_t number, int32_t *index);
void buffer_append_int16(uint8_t* buffer, int16_t number, int32_t *index);
void buffer_append_uint16(uint8_t* buffer, uint16_t number, int32_t *index);
void buffer_append_double16(uint8_t* buffer, double number, double scale, int32_t *index);
void buffer_append_double32(uint8_t* buffer, double number, double scale, int32_t *index);
int16_t buffer_get_int16(const uint8_t *buffer, int32_t *index);
uint16_t buffer_get_uint16(const uint8_t *buffer, int32_t *index);
int32_t buffer_get_int32(const uint8_t *buffer, int32_t *index);
uint32_t buffer_get_uint32(const uint8_t *buffer, int32_t *index);
double buffer_get_double16(const uint8_t *buffer, double scale, int32_t *index);
double buffer_get_double32(const uint8_t *buffer, double scale, int32_t *index);

}

#endif /* BUFFER_H_ */
