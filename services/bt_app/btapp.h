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
#ifndef __BTAPP_H__
#define __BTAPP_H__

#include "app_key.h"
#include "hfp_api.h"
#include "a2dp_api.h"
#include "app_a2dp_codecs.h"
#ifdef BT_HID_DEVICE
#include "app_bt_hid.h"
#endif
#if defined(BT_MAP_SUPPORT)
#include "map_api.h"
#endif

//#define __EARPHONE_STAY_BOTH_SCAN__

/* bt config */
#ifdef _SCO_BTPCM_CHANNEL_
#define SYNC_CONFIG_PATH (0<<8|1<<4|1<<0) /* all links use hci */
#else
#define SYNC_CONFIG_PATH (0<<8|0<<4|0<<0) /* all links use hci */
#endif
#define SYNC_CONFIG_MAX_BUFFER (0) /* (e)sco use Packet size */
#ifdef CVSD_BYPASS
#define SYNC_CONFIG_CVSD_BYPASS (1) /* use pcm over hci */
#else
#define SYNC_CONFIG_CVSD_BYPASS (0) /* use pcm over hci */
#endif
#define INQ_EXT_RSP_LEN 240
///a2dp app include
/* a2dp */
/* Default SBC codec configuration */
/* data type for the SBC Codec Information Element*/
/*****************************************************************************
**  Constants
*****************************************************************************/
/* the length of the SBC Media Payload header. */
#define A2D_SBC_MPL_HDR_LEN         1

/* the LOSC of SBC media codec capabilitiy */
#define A2D_SBC_INFO_LEN            6

/* for Codec Specific Information Element */
#if defined(A2DP_SCALABLE_ON)
#define A2D_SCALABLE_IE_SAMP_FREQ_MSK    0xFF    /* b7-b0 sampling frequency */
#endif

#if defined(A2DP_LHDC_ON)
#define A2D_LHDC__IE_SAMP_FREQ_MSK    0xFF    /* b7-b0 sampling frequency */
#endif


#define A2D_STREAM_SAMP_FREQ_MSK    0xFF    /* b7-b4 sampling frequency */

#define A2D_SBC_IE_SAMP_FREQ_MSK    0xF0    /* b7-b4 sampling frequency */

#define A2D_SBC_IE_SAMP_FREQ_16     0x80    /* b7:16  kHz */
#define A2D_SBC_IE_SAMP_FREQ_32     0x40    /* b6:32  kHz */
#define A2D_SBC_IE_SAMP_FREQ_44     0x20    /* b5:44.1kHz */
#define A2D_SBC_IE_SAMP_FREQ_48     0x10    /* b4:48  kHz */

#ifdef A2DP_SCALABLE_ON
#define A2D_SBC_IE_SAMP_FREQ_96     0x08    /* b4:48  kHz */
#endif

#ifdef A2DP_LHDC_ON
#ifndef A2D_SBC_IE_SAMP_FREQ_96
#define A2D_SBC_IE_SAMP_FREQ_96     0x08    /* b4:48  kHz */
#endif
#endif

#ifdef A2DP_LDAC_ON
#ifndef A2DP_LDAC_OCTET_NUMBER
#define A2DP_LDAC_OCTET_NUMBER                     (8)
#endif

#ifndef A2D_SBC_IE_SAMP_FREQ_96
#define A2D_SBC_IE_SAMP_FREQ_96     0x08    /* b4:96  kHz */
#endif
#ifndef A2D_SBC_IE_SAMP_FREQ_88
#define A2D_SBC_IE_SAMP_FREQ_88     0x04    /* b4:88.2  kHz */
#endif
#endif

#define A2D_SBC_IE_BIT_NUM_16       0x16
#define A2D_SBC_IE_BIT_NUM_24       0x24

#define A2D_SBC_IE_CH_MD_MSK        0x0F    /* b3-b0 channel mode */
#define A2D_SBC_IE_CH_MD_MONO       0x08    /* b3: mono */
#define A2D_SBC_IE_CH_MD_DUAL       0x04    /* b2: dual */
#define A2D_SBC_IE_CH_MD_STEREO     0x02    /* b1: stereo */
#define A2D_SBC_IE_CH_MD_JOINT      0x01    /* b0: joint stereo */

#define A2D_SBC_IE_BLOCKS_MSK       0xF0    /* b7-b4 number of blocks */
#define A2D_SBC_IE_BLOCKS_4         0x80    /* 4 blocks */
#define A2D_SBC_IE_BLOCKS_8         0x40    /* 8 blocks */
#define A2D_SBC_IE_BLOCKS_12        0x20    /* 12blocks */
#define A2D_SBC_IE_BLOCKS_16        0x10    /* 16blocks */

#define A2D_SBC_IE_SUBBAND_MSK      0x0C    /* b3-b2 number of subbands */
#define A2D_SBC_IE_SUBBAND_4        0x08    /* b3: 4 */
#define A2D_SBC_IE_SUBBAND_8        0x04    /* b2: 8 */

#define A2D_SBC_IE_ALLOC_MD_MSK     0x03    /* b1-b0 allocation mode */
#define A2D_SBC_IE_ALLOC_MD_S       0x02    /* b1: SNR */
#define A2D_SBC_IE_ALLOC_MD_L       0x01    /* b0: loundess */

#define A2D_SBC_IE_MIN_BITPOOL      2
#define A2D_SBC_IE_MAX_BITPOOL      250

#ifdef __cplusplus
extern "C" {
#endif

//extern A2dpStream a2dp_stream;
//extern btif_avdtp_codec_t a2dp_avdtpcodec;
extern const unsigned char a2dp_codec_elements[];
//extern enum AUD_SAMPRATE_T a2dp_sample_rate;

//#define AVRCP_TRACK_CHANGED

#define AVRCP_KEY_NULL                  0
#define AVRCP_KEY_STOP                  1
#define AVRCP_KEY_PLAY                  2
#define AVRCP_KEY_PAUSE                 3
#define AVRCP_KEY_FORWARD               4
#define AVRCP_KEY_BACKWARD              5
#define AVRCP_KEY_VOLUME_UP             6
#define AVRCP_KEY_VOLUME_DOWN           7

#define HFP_KEY_ANSWER_CALL             8
#define HFP_KEY_HANGUP_CALL             9
#define HFP_KEY_REDIAL_LAST_CALL        10
#define HFP_KEY_CHANGE_TO_PHONE         11
#define HFP_KEY_ADD_TO_EARPHONE         12
#define HFP_KEY_MUTE                    13
#define HFP_KEY_CLEAR_MUTE              14
//3way calls oper
#define HFP_KEY_THREEWAY_HOLD_AND_ANSWER              15
#define HFP_KEY_THREEWAY_HANGUP_AND_ANSWER             16
#define HFP_KEY_THREEWAY_HOLD_REL_INCOMING            17
#define HFP_KEY_THREEWAY_HOLD_ADD_HELD_CALL           18

#define HFP_KEY_DUAL_HF_HANGUP_ANOTHER      19
#define HFP_KEY_DUAL_HF_HANGUP_CURR_ANSWER_ANOTHER  20
#define HFP_KEY_DUAL_HF_HOLD_CURR_ANSWER_ANOTHER    21
#define HFP_KEY_DUAL_HF_CHANGETOPHONE_ANSWER_ANOTHER  22
#define HFP_KEY_DUAL_HF_CHANGETOPHONE_ANOTHER_ADDTOEARPHONE 23
#define HFP_KEY_DUAL_HF_HANGUP_ANOTHER_ADDTOEARPHONE 24

//hsp
#define HSP_KEY_CKPD_CONTROL        21
#define HSP_KEY_ADD_TO_EARPHONE     22
#define HSP_KEY_CHANGE_TO_PHONE 23

typedef enum
{
    HFCALL_MACHINE_CURRENT_IDLE = 0,                                //0
    HFCALL_MACHINE_CURRENT_INCOMMING,                               //1
    HFCALL_MACHINE_CURRENT_OUTGOING,                                //2
    HFCALL_MACHINE_CURRENT_CALLING,                                 //3
    HFCALL_MACHINE_CURRENT_3WAY_INCOMMING,                          //4
    HFCALL_MACHINE_CURRENT_3WAY_HOLD_CALLING,                       //5
    HFCALL_MACHINE_CURRENT_IDLE_ANOTHER_IDLE,                       //6
    HFCALL_MACHINE_CURRENT_INCOMMING_ANOTHER_IDLE,                  //7
    HFCALL_MACHINE_CURRENT_OUTGOING_ANOTHER_IDLE,                   //8
    HFCALL_MACHINE_CURRENT_CALLING_ANOTHER_IDLE,                    //9
    HFCALL_MACHINE_CURRENT_3WAY_INCOMMING_ANOTHER_IDLE,             //10
    HFCALL_MACHINE_CURRENT_3WAY_HOLD_CALLING_ANOTHER_IDLE,          //11
    HFCALL_MACHINE_CURRENT_INCOMMING_ANOTHER_INCOMMING,             //12
    HFCALL_MACHINE_CURRENT_CALLING_ANOTHER_INCOMMING,               //13
    HFCALL_MACHINE_CURRENT_CALLING_ANOTHER_CHANGETOPHONE,           //14
    HFCALL_MACHINE_CURRENT_OUTGOING_ANOTHER_INCOMMING,              //15
    HFCALL_MACHINE_CURRENT_CALLING_ANOTHER_HOLD,                    //16
    HFCALL_MACHINE_NUM
} HFCALL_MACHINE_ENUM;

typedef enum
{
    HFCALL_NEXT_STA_NULL = 0,                                //0
    HFCALL_NEXT_STA_ANOTHER_ANSWER,                               //1
    HFCALL_NEXT_STA_ANOTHER_ADDTOEARPHONE,                               //2
    HFCALL_NEXT_STA_NUM
} HFCALL_NEXT_STA_ENUM;


#define APP_REPORT_SPEAKER_VOL_CMD  0x01
#define APP_CPKD_CMD                0x02
#define SPP_MAX_TX_PACKET_NUM   5


#ifndef BT_DEVICE_NUM
#if defined(IBRT_V2_MULTIPOINT)
#define BT_DEVICE_NUM 2
#elif defined(__BT_ONE_BRING_TWO__)
#define BT_DEVICE_NUM 2
#else
#define BT_DEVICE_NUM 1
#endif
#endif

enum BT_DEVICE_ID_T {
    BT_DEVICE_ID_1 = 0,
    BT_DEVICE_ID_2 = 1,
    BT_DEVICE_ID_3 = 2
};

#ifdef BT_SOURCE
#if defined(BT_MULTI_SOURCE)
#define BT_SOURCE_DEVICE_NUM 2
#else
#define BT_SOURCE_DEVICE_NUM 1
#endif
#endif

#ifndef BT_SOURCE_DEVICE_NUM
#define BT_SOURCE_DEVICE_NUM 0
#endif

#define BT_AVDTP_CP_VALUE_SIZE 10

struct BT_DEVICE_T {
    btif_a2dp_stream_t *btif_a2dp_stream;
    uint8_t a2dp_lhdc_llc;

    uint8_t channel_mode;
    uint8_t sample_rate;
    uint8_t sample_bit;



    a2dp_stream_t *a2dp_connected_stream;
    bt_bdaddr_t remote;


    uint8_t a2dp_conn_flag;
    uint8_t a2dp_streamming;
    uint8_t a2dp_play_pause_flag;
    uint8_t volume_report;
    btif_avdtp_codec_type_t codec_type;
    bool avdtp_cp;
    btif_avdtp_content_prot_t a2dp_avdtp_cp;
    uint8_t a2dp_avdtp_cp_security_data[BT_AVDTP_CP_VALUE_SIZE];

    btif_avrcp_channel_t *avrcp_channel;

    btif_hf_channel_t* hf_channel;
    btif_hf_call_setup_t hfchan_callSetup;
    btif_hf_call_active_t hfchan_call;
    btif_audio_state_t hf_audio_state;
    btif_hf_call_held_state hf_callheld;

    uint8_t hf_conn_flag;

#ifdef BT_HID_DEVICE
    hid_channel_t hid_channel;
#endif

#if defined (BT_MAP_SUPPORT)
    btif_map_session_handle_t map_session_handle;
#endif
};



/////app key handle include
void a2dp_handleKey(uint8_t a2dp_key);
void hfp_handle_key(uint8_t hfp_key);
void hsp_handle_key(uint8_t hsp_key);
void btapp_a2dp_report_speak_gain(void);

#ifdef __POWERKEY_CTRL_ONOFF_ONLY__
#define   BTAPP_FUNC_KEY            APP_KEY_CODE_FN1
#define   BTAPP_VOLUME_UP_KEY       APP_KEY_CODE_FN2
#define   BTAPP_VOLUME_DOWN_KEY     APP_KEY_CODE_FN3
#ifdef SUPPORT_SIRI
#define   BTAPP_RELEASE_KEY         APP_KEY_CODE_NONE
#endif
#else
#define   BTAPP_FUNC_KEY            APP_KEY_CODE_PWR
#define   BTAPP_VOLUME_UP_KEY       APP_KEY_CODE_FN1
#define   BTAPP_VOLUME_DOWN_KEY     APP_KEY_CODE_FN2
#ifdef SUPPORT_SIRI
#define   BTAPP_RELEASE_KEY         APP_KEY_CODE_NONE
#endif
#endif
void bt_key_init(void);
void bt_key_send(APP_KEY_STATUS *status);
void bt_key_handle(void);

void a2dp_callback(uint8_t device_id, a2dp_stream_t *Stream, const a2dp_callback_parms_t *Info);
void avrcp_init(void);

uint8_t a2dp_volume_get(enum BT_DEVICE_ID_T id);
bool avrcp_get_tg_play_status_play_pause(void);




/**
 * Convert BES BD_ADDR to virtual
 * BES Device ID
 */
bool a2dp_id_from_bdaddr(bt_bdaddr_t *bd_addr, uint8_t *id);

void bt_key_handle_func_click();
void bt_key_handle_func_doubleclick();
void bt_key_handle_func_longpress();


void bt_sbc_player_set_codec_type(uint8_t type);
uint8_t bt_sbc_player_get_codec_type(void);
uint8_t bt_sbc_player_get_sample_bit(void);
#if defined(A2DP_LDAC_ON)
int bt_ldac_player_get_channelmode(void);
int bt_get_ladc_sample_rate(void);
#endif

uint8_t app_bt_avrcp_get_volume_change_trans_id(uint8_t device_id);
void app_bt_avrcp_set_volume_change_trans_id(uint8_t device_id, uint8_t trans_id);
uint8_t app_bt_avrcp_get_ctl_trans_id(uint8_t device_id);
void app_bt_avrcp_set_ctl_trans_id(uint8_t device_id, uint8_t trans_id);



#ifdef __cplusplus
}
#endif

#endif /* __BTAPP_H__ */
