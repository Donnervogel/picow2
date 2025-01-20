#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "btstack.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "btstack_resample.h"
//#include "btstack_hid.h"

// Define the pin to control pairing mode
#define PAIRING_CONTROL_PIN 2

#define BTSTACK_FILE__ "picow2.cpp"

// TinyUSB headers for USB communication
#include "tusb.h"
#include "class/audio/audio_device.h"

// Function prototypes
static void hci_packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);
static void a2dp_sink_packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t event_size);

// Initialize TinyUSB
static void init_usb(void) {
    tusb_init();
}

// Initialize GPIO for pairing control
static void init_pairing_control(void) {
    gpio_init(PAIRING_CONTROL_PIN);
    gpio_set_dir(PAIRING_CONTROL_PIN, GPIO_IN);
    gpio_pull_up(PAIRING_CONTROL_PIN); // Pull up internally
}

// Check if pairing mode should be enabled
static bool should_enable_pairing(void) {
    return gpio_get(PAIRING_CONTROL_PIN) == 0; // Pin pulled low
}

static int setup_demo(void) {
    // Initialize USB
    init_usb();

    // Initialize GPIO for pairing control
    init_pairing_control();

    // Initialize BTstack protocols
    l2cap_init();
    sdp_init();
#ifdef ENABLE_BLE
    sm_init(); // Initialize LE Security Manager
#endif
    a2dp_sink_init();
    avrcp_init();
    avrcp_controller_init();
    avrcp_target_init();

    gap_set_local_name("PicoW");
    gap_discoverable_control(1);
    gap_set_class_of_device(0x200404);
    gap_set_default_link_policy_settings(LM_LINK_POLICY_ENABLE_ROLE_SWITCH | LM_LINK_POLICY_ENABLE_SNIFF_MODE);
    gap_set_allow_role_switch(true);

    // Register HCI event handler
    static btstack_packet_callback_registration_t hci_event_callback_registration;
    hci_event_callback_registration.callback = &hci_packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    // Configure GAP (Generic Access Profile)
    if (should_enable_pairing()) {
        gap_discoverable_control(1); // Make the device discoverable
    } else {
        gap_discoverable_control(0); // Do not make the device discoverable
    }

    gap_set_class_of_device(0x200404); // Set Class of Device
    gap_set_default_link_policy_settings(LM_LINK_POLICY_ENABLE_ROLE_SWITCH | LM_LINK_POLICY_ENABLE_SNIFF_MODE);
    gap_set_allow_role_switch(true);

    // Register for A2DP and AVRCP events
    a2dp_sink_register_packet_handler(&a2dp_sink_packet_handler);
    // Define these handlers or include the appropriate headers
    // avrcp_register_packet_handler(&avrcp_packet_handler);
    // avrcp_controller_register_packet_handler(&avrcp_controller_packet_handler);
    // avrcp_target_register_packet_handler(&avrcp_target_packet_handler);
    // a2dp_sink_register_media_handler(&handle_l2cap_media_data_packet);

    // Define or include these variables
    // a2dp_sink_demo_stream_endpoint_t *stream_endpoint = &a2dp_sink_demo_stream_endpoint;
    // avdtp_stream_endpoint_t *local_stream_endpoint = a2dp_sink_create_stream_endpoint(AVDTP_AUDIO,
    //                                                                                   AVDTP_CODEC_SBC, media_sbc_codec_capabilities, sizeof(media_sbc_codec_capabilities),
    //                                                                                   stream_endpoint->media_sbc_codec_configuration, sizeof(stream_endpoint->media_sbc_codec_configuration));
    // memset(sdp_avdtp_sink_service_buffer, 0, sizeof(sdp_avdtp_sink_service_buffer));
    // a2dp_sink_create_sdp_record(sdp_avdtp_sink_service_buffer, sdp_create_service_record_handle(),
    //                             AVDTP_SINK_FEATURE_MASK_HEADPHONE, NULL, NULL);
    // btstack_assert(de_get_len(sdp_avdtp_sink_service_buffer) <= sizeof(sdp_avdtp_sink_service_buffer));
    // sdp_register_service(sdp_avdtp_sink_service_buffer);

    // Create AVRCP Controller service record and register it with SDP. We send Category 1 commands to the media player, e.g. play/pause
    // memset(sdp_avrcp_controller_service_buffer, 0, sizeof(sdp_avrcp_controller_service_buffer));
    // uint16_t controller_supported_features = 1 << AVRCP_CONTROLLER_SUPPORTED_FEATURE_CATEGORY_PLAYER_OR_RECORDER;
    return 0;
}

static void hci_packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size) {
    UNUSED(channel);
    UNUSED(size);
    if (packet_type != HCI_EVENT_PACKET) return;
    if (hci_event_packet_get_type(packet) == HCI_EVENT_PIN_CODE_REQUEST) {
        bd_addr_t address;
        printf("Pin code request - using '0000'\n");
        hci_event_pin_code_request_get_bd_addr(packet, address);
        gap_pin_code_response(address, "0000");
    }
}

static void a2dp_sink_packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t event_size) {
    UNUSED(channel);

    if (packet_type != AVDTP_MEDIA_TRANSPORT) return;

    // Check if there is space available in the endpoint's FIFO
    if (tud_audio_n_available(0) >= event_size) {
        // Write the audio data to the USB microphone interface
        tud_audio_n_write(0, packet, event_size);
    }
}

int btstack_main(int argc, const char *argv[]) {
    UNUSED(argc);
    (void)argv;

    setup_demo();

    // Turn on the Bluetooth stack
    printf("Starting BTstack ...\n");
    hci_power_control(HCI_POWER_ON);
    return 0;
}