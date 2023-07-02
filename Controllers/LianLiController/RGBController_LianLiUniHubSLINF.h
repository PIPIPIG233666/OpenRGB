/*-----------------------------------------*\
|  RGBController_LianLiUniHubSLINF.h           |
|                                           |
|  Generic RGB Interface for Lian Li Uni    |
|  Hub SLINF USB controller driver             |
|                                           |
|  Oliver P 04/26/2022                      |
|  Credit to Luca Lovisa for original work. |
\*-----------------------------------------*/

#pragma once

#include <cstdint>
#include <vector>

#include "LianLiUniHubSLINFController.h"
#include "RGBController.h"

class RGBController_LianLiUniHubSLINF : public RGBController
{
public:
    RGBController_LianLiUniHubSLINF(LianLiUniHubSLINFController* controller_ptr);
    ~RGBController_LianLiUniHubSLINF();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

    void SetCustomMode();

private:
    LianLiUniHubSLINFController* controller;
    bool                    initializedMode;
};
