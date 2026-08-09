#ifndef __STM8S_CONF_H
#define __STM8S_CONF_H

#include "stm8s.h"

/*
 * Default SPL module selection for this iolib.
 * Real projects can override this file by passing USER_INC_DIR in make.
 */
#include "stm8s_clk.h"
#include "stm8s_gpio.h"
#include "stm8s_i2c.h"

#ifndef STM8S903
#include "stm8s_tim4.h"
#endif

#ifdef USE_FULL_ASSERT
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
void assert_failed(uint8_t* file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif

#endif
