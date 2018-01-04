#ifndef MAIN_H
#define MAIN_H

#include "stm32f4xx.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "userconfig.h"
#include "setup.h"
#include "delay.h"
#include "motorcontrol.h"
#include "encoder.h"
#include "sensor.h"
#include "button.h"
#include "model.h"
#include "view.h"
#include "controller.h"
#include "search.h"
#include "speedrun.h"
#include "flash.h"
#include "test.h"

extern int32_t option;

void systick(void);

#endif
