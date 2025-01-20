#ifndef TUSB_CONFIG_H
#define TUSB_CONFIG_H

// Common Configuration
#define CFG_TUSB_MCU             OPT_MCU_RP2040
#define CFG_TUSB_RHPORT0_MODE    (OPT_MODE_DEVICE)
#define CFG_TUSB_OS              OPT_OS_NONE
#define CFG_TUSB_DEBUG           0

// Device Configuration
#define CFG_TUD_ENDPOINT0_SIZE   64
#define CFG_TUD_AUDIO_ENABLE_EP_OUT 1

// Class Configuration
#define CFG_TUD_CDC              1
#define CFG_TUD_MSC              1
#define CFG_TUD_HID              1
#define CFG_TUD_MIDI             0
#define CFG_TUD_VENDOR           0
#define CFG_TUD_AUDIO            1  // Enable Audio class

// Audio Configuration
#define CFG_TUD_AUDIO_ENABLE_EP_IN  1  // Enable Endpoint IN for Audio
#define CFG_TUD_AUDIO_FUNC_1_DESC_LEN  TUD_AUDIO_MIC_ONE_CH_DESC_LEN
#define CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_TX  1
#define CFG_TUD_AUDIO_FUNC_1_N_BYTES_PER_SAMPLE_TX  2
#define CFG_TUD_AUDIO_FUNC_1_TX_FIFO_SIZE  64
#define CFG_TUD_AUDIO_FUNC_1_EP_IN_SZ_MAX  64  // Define the biggest EP IN size
#define CFG_TUD_AUDIO_FUNC_1_CTRL_BUF_SZ  64  // Define the audio class control request buffer size
#define CFG_TUD_AUDIO_FUNC_1_N_AS_INT  1  // Define the number of Standard AS Interface Descriptors

// CDC FIFO Configuration
#define CFG_TUD_CDC_RX_BUFSIZE   64
#define CFG_TUD_CDC_TX_BUFSIZE   64

// MSC FIFO Configuration
#define CFG_TUD_MSC_BUFSIZE      512

#endif // TUSB_CONFIG_H