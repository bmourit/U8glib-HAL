/*

  u8g_arduino_no_en_parallel.c

  Universal 8bit Graphics Library

  Copyright (c) 2012, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  PIN_D0 8
  PIN_D1 9
  PIN_D2 10
  PIN_D3 11
  PIN_D4 4
  PIN_D5 5
  PIN_D6 6
  PIN_D7 7

  PIN_CS1 14
  PIN_CS2 15
  PIN_RW 16
  PIN_DI 17
  PIN_EN 18

  u8g_Init8Bit(u8g, dev, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, cs2, di, rw, reset)
  u8g_Init8Bit(u8g, dev,  8,    9, 10, 11,   4,   5,   6,   7, 18, 14, 15, 17, 16, U8G_PIN_NONE)
  Update for ATOMIC operation done (01 Jun 2013)

    U8G_ATOMIC_OR(ptr, val)
    U8G_ATOMIC_AND(ptr, val)
    U8G_ATOMIC_START();
    U8G_ATOMIC_END();

*/

#include "u8g.h"

#if defined(ARDUINO) && !defined(ARDUINO_ARCH_STM32) && !defined(ARDUINO_ARCH_MFL)

  #if ARDUINO < 100
    // #include <WProgram.h>
    #include <wiring_private.h>
    #include <pins_arduino.h>
  #else
    #include <Arduino.h>
  #endif

  // #define PIN_RESET

  #ifdef __PIC32MX
    // CHIPKIT PIC32
    typedef uint32_t u8g_data_t;
  #else
    typedef uint8_t u8g_data_t;
  #endif
  typedef u8g_data_t* u8g_ptr_t;
  static volatile u8g_ptr_t u8g_data_port[8];
  static u8g_data_t u8g_data_mask[8];

  static void u8g_com_arduino_no_en_parallel_init(u8g_t *u8g) {
    u8g_data_port[0] = (u8g_ptr_t)portOutputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D0]));
    u8g_data_mask[0] = (u8g_data_t)digitalPinToBitMask(u8g->pin_list[U8G_PI_D0]);
    u8g_data_port[1] = (u8g_ptr_t)portOutputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D1]));
    u8g_data_mask[1] = (u8g_data_t)digitalPinToBitMask(u8g->pin_list[U8G_PI_D1]);
    u8g_data_port[2] = (u8g_ptr_t)portOutputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D2]));
    u8g_data_mask[2] = (u8g_data_t)digitalPinToBitMask(u8g->pin_list[U8G_PI_D2]);
    u8g_data_port[3] = (u8g_ptr_t)portOutputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D3]));
    u8g_data_mask[3] = (u8g_data_t)digitalPinToBitMask(u8g->pin_list[U8G_PI_D3]);

    u8g_data_port[4] = (u8g_ptr_t)portOutputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D4]));
    u8g_data_mask[4] = (u8g_data_t)digitalPinToBitMask(u8g->pin_list[U8G_PI_D4]);
    u8g_data_port[5] = (u8g_ptr_t)portOutputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D5]));
    u8g_data_mask[5] = (u8g_data_t)digitalPinToBitMask(u8g->pin_list[U8G_PI_D5]);
    u8g_data_port[6] = (u8g_ptr_t)portOutputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D6]));
    u8g_data_mask[6] = (u8g_data_t)digitalPinToBitMask(u8g->pin_list[U8G_PI_D6]);
    u8g_data_port[7] = (u8g_ptr_t)portOutputRegister(digitalPinToPort(u8g->pin_list[U8G_PI_D7]));
    u8g_data_mask[7] = (u8g_data_t)digitalPinToBitMask(u8g->pin_list[U8G_PI_D7]);
  }

  // No atomic protcetion. This is done by caller
  static void u8g_com_arduino_no_en_write_data_pin(uint8_t pin, uint8_t val) {
    if (val != 0)
      *u8g_data_port[pin] |= u8g_data_mask[pin];
    else
      *u8g_data_port[pin] &= ~u8g_data_mask[pin];
  }

  void u8g_com_arduino_no_en_parallel_write(u8g_t *u8g, uint8_t val) {
    U8G_ATOMIC_START();
    u8g_com_arduino_no_en_write_data_pin( 0, val & 1 );
    val >>= 1;
    u8g_com_arduino_no_en_write_data_pin( 1, val & 1 );
    val >>= 1;
    u8g_com_arduino_no_en_write_data_pin( 2, val & 1 );
    val >>= 1;
    u8g_com_arduino_no_en_write_data_pin( 3, val & 1 );
    val >>= 1;

    u8g_com_arduino_no_en_write_data_pin( 4, val & 1 );
    val >>= 1;
    u8g_com_arduino_no_en_write_data_pin( 5, val & 1 );
    val >>= 1;
    u8g_com_arduino_no_en_write_data_pin( 6, val & 1 );
    val >>= 1;
    u8g_com_arduino_no_en_write_data_pin( 7, val & 1 );
    val >>= 1;
    U8G_ATOMIC_END();

    // EN cycle time must be 1 micro second, digitalWrite is slow enough to do this
    if (u8g->pin_list[U8G_PI_CS_STATE] == 1) {
      u8g_MicroDelay();
      u8g_com_arduino_digital_write(u8g, U8G_PI_CS1, HIGH);
      u8g_MicroDelay();
      u8g_com_arduino_digital_write(u8g, U8G_PI_CS1, LOW);
      u8g_MicroDelay();
    }
    else if (u8g->pin_list[U8G_PI_CS_STATE] == 2) {
      u8g_MicroDelay();
      u8g_com_arduino_digital_write(u8g, U8G_PI_CS2, HIGH);
      u8g_MicroDelay();
      u8g_com_arduino_digital_write(u8g, U8G_PI_CS2, LOW);
      u8g_MicroDelay();
    }
  }

  uint8_t u8g_com_arduino_no_en_parallel_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {
    switch (msg) {
      case U8G_COM_MSG_INIT:
        u8g_com_arduino_no_en_parallel_init(u8g);
        // setup the RW pin as output and force it to low
        if (u8g->pin_list[U8G_PI_RW] != U8G_PIN_NONE) {
          pinMode(u8g->pin_list[U8G_PI_RW], OUTPUT);
          u8g_com_arduino_digital_write(u8g, U8G_PI_RW, LOW);
        }
        // set all pins (except RW pin)
        u8g_com_arduino_assign_pin_output_high(u8g);
        break;
      case U8G_COM_MSG_STOP:
        break;

      case U8G_COM_MSG_CHIP_SELECT:
        /*
          0: nothing selected
          1: CS1 will be used as enable line
          2: CS2 will be used as enable line
          this will be used in the u8g_com_arduino_no_en_parallel_write() procedure
        */
        u8g->pin_list[U8G_PI_CS_STATE] = arg_val;
        break;
      case U8G_COM_MSG_WRITE_BYTE:
        u8g_com_arduino_no_en_parallel_write(u8g, arg_val);
        break;
      case U8G_COM_MSG_WRITE_SEQ: {
        register uint8_t *ptr = arg_ptr;
        while (arg_val > 0) {
          u8g_com_arduino_no_en_parallel_write(u8g, *ptr++);
          arg_val--;
        }
      }
      break;
      case U8G_COM_MSG_WRITE_SEQ_P: {
        register uint8_t *ptr = arg_ptr;
        while (arg_val > 0) {
          u8g_com_arduino_no_en_parallel_write(u8g, u8g_pgm_read(ptr));
          ptr++;
          arg_val--;
        }
      }
      break;
      case U8G_COM_MSG_ADDRESS:                   // define cmd (arg_val = 0) or data mode (arg_val = 1)
        u8g_com_arduino_digital_write(u8g, U8G_PI_DI, arg_val);
        break;
      case U8G_COM_MSG_RESET:
        if (u8g->pin_list[U8G_PI_RESET] != U8G_PIN_NONE)
          u8g_com_arduino_digital_write(u8g, U8G_PI_RESET, arg_val);
        break;
    }
    return 1;
  }

#else // if defined(ARDUINO) && !defined(ARDUINO_ARCH_STM32) && !defined(ARDUINO_ARCH_MFL)

  uint8_t u8g_com_arduino_no_en_parallel_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {
    return 1;
  }

#endif /* ARDUINO */
