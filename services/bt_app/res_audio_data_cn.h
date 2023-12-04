/*
 * Copyright (c) 2021 Bestechnic (Shanghai) Co., Ltd. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __RES_AUDIO_DATA_CN_H
#define __RES_AUDIO_DATA_CN_H

const uint8_t CN_POWER_ON [] = {
#include "res/cn/SOUND_POWER_ON.txt"
};

const uint8_t CN_POWER_OFF [] = {
#include "res/cn/SOUND_POWER_OFF.txt"
};

const uint8_t CN_SOUND_ZERO[] = { 
#include "res/cn/SOUND_ZERO.txt"
};

const uint8_t CN_SOUND_ONE[] = { 
#include "res/cn/SOUND_ONE.txt"
};

const uint8_t CN_SOUND_TWO[] = { 
#include "res/cn/SOUND_TWO.txt"
};

const uint8_t CN_SOUND_THREE[] = { 
#include "res/cn/SOUND_THREE.txt"
};

const uint8_t CN_SOUND_FOUR[] = { 
#include "res/cn/SOUND_FOUR.txt"
};

const uint8_t CN_SOUND_FIVE[] = { 
#include "res/cn/SOUND_FIVE.txt"
};

const uint8_t CN_SOUND_SIX[] = { 
#include "res/cn/SOUND_SIX.txt"
};

const uint8_t CN_SOUND_SEVEN[] = { 
#include "res/cn/SOUND_SEVEN.txt"
};

const uint8_t CN_SOUND_EIGHT[] = { 
#include "res/cn/SOUND_EIGHT.txt"
};

const uint8_t CN_SOUND_NINE[] = { 
#include "res/cn/SOUND_NINE.txt"
};

const uint8_t CN_BT_PAIR_ENABLE[] = {
#include "res/cn/SOUND_PAIR_ENABLE.txt"
};

const uint8_t CN_BT_PAIRING [] = {
#include "res/cn/SOUND_PAIRING.txt"
};

const uint8_t CN_BT_PAIRING_FAIL[] = {
#include "res/cn/SOUND_PAIRING_FAIL.txt"
};

const uint8_t CN_BT_PAIRING_SUCCESS[] = { 
#include "res/cn/SOUND_PAIRING_SUCCESS.txt"
};

const uint8_t CN_BT_REFUSE[] = { 
#include "res/cn/SOUND_REFUSE.txt"
};

const uint8_t CN_BT_OVER[] = { 
#include "res/cn/SOUND_OVER.txt"
};

const uint8_t CN_BT_ANSWER[] = { 
#include "res/cn/SOUND_ANSWER.txt"
};


const uint8_t CN_BT_HUNG_UP[] = { 
#include "res/cn/SOUND_HUNG_UP.txt"
};

const uint8_t CN_BT_INCOMING_CALL [] = {
#include "res/cn/SOUND_INCOMING_CALL.txt"
};

const uint8_t CN_BT_CONNECTED [] = {
#include "res/cn/SOUND_CONNECTED.txt"    
};

const uint8_t CN_BT_DIS_CONNECT [] = {
#include "res/cn/SOUND_DIS_CONNECT.txt"
};

const uint8_t CN_CHARGE_PLEASE[] = {
#include "res/cn/SOUND_CHARGE_PLEASE.txt"
};

const uint8_t CN_CHARGE_FINISH[] = {
#include "res/cn/SOUND_CHARGE_FINISH.txt"
};

const uint8_t CN_LANGUAGE_SWITCH[] = { 
#include "res/cn/SOUND_LANGUAGE_SWITCH.txt"
};

const uint8_t CN_BT_WARNING[] = { 
#include "res/cn/SOUND_WARNING.txt"
};

const uint8_t CN_BT_ALEXA_START[] = { 

};

const uint8_t CN_BT_ALEXA_STOP[] = { 

};

const uint8_t CN_BT_GSOUND_MIC_OPEN[] = {

};

const uint8_t CN_BT_GSOUND_MIC_CLOSE[] = {

};

const uint8_t CN_BT_GSOUND_NC[] = {

};

#ifdef __INTERACTION__
const uint8_t CN_BT_FINDME[] = {
#include "res/cn/SOUND_FINDME.txt"
};
#endif

/*doesn't have chinese version sound mute */
const uint8_t CN_BT_MUTE[] = {
#include "res/SOUND_MUTE.txt"
};

#endif

