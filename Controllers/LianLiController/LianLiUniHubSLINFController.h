/*-----------------------------------------*\
|  LianLiHubSLINFController.h                  |
|                                           |
|  Definitions and types for Lian Li SLINF120  |
|                                           |
|  Oliver P 04/26/2022                      |
|  Credit to Luca Lovisa for original work. |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

/*----------------------------------------------------------------------------*\
| Global definitions.                                                          |
\*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*\
| Definitions related to zone Sizes                                            |
\*----------------------------------------------------------------------------*/


enum
{
    UNIHUB_SLINF_CHANNEL_COUNT              = 0x04,   /* Channel count                   */
    UNIHUB_SLINF_CHAN_FANLED_COUNT          = 0x20,   /* Max-LED per channel count - 32  */
    UNIHUB_SLINF_CHAN_EDGELED_COUNT         = 0x30,   /* Max-LED per channel count - 48  */
    UNIHUB_SLINF_CHAN_LED_COUNT             = 0x50,   /* Max-LED per channel count - 80  */
};

/*----------------------------------------------------------------------------*\
| Definitions related to LED configuration.                                    |
\*----------------------------------------------------------------------------*/

// Used for sync'd mode between Fan and Edge

enum
{
    UNIHUB_SLINF_LED_MODE_RAINBOW            = 0x05,   /* Rainbow mode           - Calls Fan only */
    UNIHUB_SLINF_LED_MODE_RAINBOW_MORPH      = 0x35,   /* Rainbow Morph mode     - Calls Fan only */
    UNIHUB_SLINF_LED_MODE_STATIC_COLOR       = 0x01,   /* Static Color mode       */
    UNIHUB_SLINF_LED_MODE_BREATHING          = 0x02,   /* Breathing mode          */
    UNIHUB_SLINF_LED_MODE_BREATHING_RAINBOW  = 0x06,   /* Breathing Rainbow mode  */
    UNIHUB_SLINF_LED_MODE_RUNWAY             = 0x1A,   /* Runway mode             */
    UNIHUB_SLINF_LED_MODE_METEOR             = 0x19,   /* Meteor mode             */
    UNIHUB_SLINF_LED_MODE_WARNING            = 0x2D,   /* Warning mode           - Calls Fan only */
    UNIHUB_SLINF_LED_MODE_VOICE              = 0x2E,   /* Voice mode             - Calls Fan only */
    UNIHUB_SLINF_LED_MODE_MIXING             = 0x2F,   /* Mixing mode            - Calls Fan only */
    UNIHUB_SLINF_LED_MODE_STACK              = 0x30,   /* Stack mode             - Calls Fan only */
    UNIHUB_SLINF_LED_MODE_TIDE               = 0x31,   /* Tide mode              - Calls Fan only */
    UNIHUB_SLINF_LED_MODE_SCAN               = 0x1B,   /* Scan mode              - Calls Fan only */
    UNIHUB_SLINF_LED_MODE_DOOR               = 0x00,
    UNIHUB_SLINF_LED_MODE_HEART_BEAT         = 0x00,
    UNIHUB_SLINF_LED_MODE_HEART_BEAT_RUNAWAY = 0x00,
    UNIHUB_SLINF_LED_MODE_DISCO              = 0x00,
    UNIHUB_SLINF_LED_MODE_ELECTRIC_CURRENT   = 0x07,
    /*  09 is flickering 
        10 is rotating and changing colors from bottom
        11 is 9 + 10
        12 is nothing
        1B & 2E is scan?
        1C is another static
        1D is left right rotating
        1E is flicking a more patterned way
        2A is 1 12 123 tide
        2B is center to side side to center
        2C & 2D is static
    */
};

enum
{
    UNIHUB_SLINF_LED_SPEED_000              = 0x02,   /* Very slow speed         */
    UNIHUB_SLINF_LED_SPEED_025              = 0x01,   /* Rather slow speed       */
    UNIHUB_SLINF_LED_SPEED_050              = 0x00,   /* Medium speed            */
    UNIHUB_SLINF_LED_SPEED_075              = 0xFF,   /* Rather fast speed       */
    UNIHUB_SLINF_LED_SPEED_100              = 0xFE,   /* Very fast speed         */
};

enum
{
    UNIHUB_SLINF_LED_DIRECTION_LTR          = 0x00,   /* Left-to-Right direction */
    UNIHUB_SLINF_LED_DIRECTION_RTL          = 0x01,   /* Right-to-Left direction */
};

enum
{
    UNIHUB_SLINF_LED_BRIGHTNESS_000         = 0x08,   /* Very dark (off)         */
    UNIHUB_SLINF_LED_BRIGHTNESS_025         = 0x03,   /* Rather dark             */
    UNIHUB_SLINF_LED_BRIGHTNESS_050         = 0x02,   /* Medium bright           */
    UNIHUB_SLINF_LED_BRIGHTNESS_075         = 0x01,   /* Rather bright           */
    UNIHUB_SLINF_LED_BRIGHTNESS_100         = 0x00,   /* Very bright             */
};

enum
{
    UNIHUB_SLINF_LED_LIMITER                = 0x01    /* Limit the color white to 999999 as per manufacturer limits */
};


/*----------------------------------------------------------------------------*\
| Definitions related to packet configuration.                                 |
\*----------------------------------------------------------------------------*/

enum
{
    UNIHUB_SLINF_TRANSACTION_ID            = 0xE0, /* Command value to start all packets */
};

/*----------------------------------------------------------------------------*\
| Uni Hub SLINF controller.                                                       |
\*----------------------------------------------------------------------------*/

class LianLiUniHubSLINFController
{


public:
    LianLiUniHubSLINFController(hid_device* dev_handle, const char* path, unsigned short pid, std::string dev_name);
    ~LianLiUniHubSLINFController();

    std::string                 GetDeviceLocation();
    std::string                 GetFirmwareVersionString();
    std::string                 GetName();
    std::string                 GetSerialString();

    void                SetChannelMode
                                (
                                unsigned char   channel,
                                unsigned int    mode_value,
                                std::vector<RGBColor>        colors,    // Not a pointer because the copy gets resized
                                unsigned int    num_colors,
                                unsigned int    num_fans,
                                bool            upd_both_fan_edge,
                                unsigned int    brightness,
                                unsigned int    speed,
                                unsigned int    direction
                                );

    void                SetChannelLEDs
                                (
                                unsigned char   channel,
                                RGBColor *      colors,
                                unsigned int    num_colors,
                                float           brightness
                                );

    void                SendStartAction
                                (
                                unsigned char   channel,
                                unsigned int    num_fans
                                );

    void                SendColorData
                                (
                                unsigned char   channel,            // Zone index
                                unsigned int    fan_or_edge,        // 1 (Fan) or 0 (Edge) modifer to channel
                                unsigned int    num_leds,
                                unsigned char*  led_data            // Color data payload
                                );

    void                SendCommitAction
                                (
                                unsigned char   channel,            // Zone index
                                unsigned int    fan_or_edge,        // 1 (Fan) or 0 (Edge) modifer to channel
                                unsigned char   effect,
                                unsigned char   speed,
                                unsigned int    direction,
                                unsigned int    brightness
                                );

private:
    /* The Uni Hub requires colors in RBG order */
    struct Color
    {
        uint8_t r;
        uint8_t b;
        uint8_t g;
    };

    /* The values correspond to the definitions above */
    struct Channel
    {
        uint8_t  index;

        uint8_t  anyFanCountOffset;
        uint8_t  anyFanCount;

        uint16_t ledActionAddress;
        uint16_t ledCommitAddress;
        uint16_t ledModeAddress;
        uint16_t ledSpeedAddress;
        uint16_t ledDirectionAddress;
        uint16_t ledBrightnessAddress;

        Color    colors[UNIHUB_SLINF_CHAN_FANLED_COUNT];

        uint8_t  ledMode;
        uint8_t  ledSpeed;
        uint8_t  ledDirection;
        uint8_t  ledBrightness;

        uint16_t fanHubActionAddress;
        uint16_t fanHubCommitAddress;

        uint16_t fanPwmActionAddress;
        uint16_t fanPwmCommitAddress;
        uint16_t fanRpmActionAddress;

        uint16_t fanSpeed;
    };

private:
    hid_device*             dev;
    unsigned short          dev_pid;

    /*---------------------------------------------------------*\
    | Device-specific protocol settings                         |
    \*---------------------------------------------------------*/
    unsigned char           dev_transaction_id;
    unsigned char           dev_led_id;

    /*---------------------------------------------------------*\
    | Device information strings                                |
    \*---------------------------------------------------------*/
    std::string             firmware_version;
    std::string             location;
    std::string             name;
    device_type             type;

    /*---------------------------------------------------------*\
    | HID report index for request and response                 |
    \*---------------------------------------------------------*/
    unsigned char           report_index;
    unsigned char           response_index;

};
