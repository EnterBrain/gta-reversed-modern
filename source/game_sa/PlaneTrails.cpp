#include "StdInc.h"

#include "PlaneTrails.h"

CPlaneTrail (&CPlaneTrails::aArray)[NUM_PLANE_TRAILS] = *(CPlaneTrail(*)[NUM_PLANE_TRAILS])0xC713A8;

void CPlaneTrails::InjectHooks() {
    RH_ScopedClass(CPlaneTrails);
    RH_ScopedCategoryGlobal();

    RH_ScopedInstall(Init, 0x717370);
    RH_ScopedInstall(Render, 0x7173A0);
    RH_ScopedInstall(RegisterPoint, 0x7174C0);
    RH_ScopedInstall(Update, 0x7174F0);
}

// 0x717370
void CPlaneTrails::Init() {
    for (int i = 0; i < NUM_PLANE_TRAILS; i++) {
        aArray[i].Init();
    }
}

// 0x7173A0
void CPlaneTrails::Render() {
    const CColourSet& c = CTimeCycle::m_CurrentColours;
    const float fMaxColor = (float)std::max(c.m_nSkyBottomBlue, c.m_nSkyBottomGreen, c.m_nSkyBottomRed) / 256.0f;

    const float fIntensity = std::min(1.0f - CWeather::CloudCoverage, 1.0f - CWeather::Foggyness, 1.0f - CWeather::Rain);

    const float fFinalIntensity = std::max(fMaxColor, fIntensity);

    if (fFinalIntensity > 0.0001f) {
        for (int i = 0; i < NUM_PLANE_TRAILS; i++) {
            aArray[i].Render(fFinalIntensity);
        }
    }
}

// 0x7174C0
void CPlaneTrails::RegisterPoint(CVector pos, uint32_t point) {
    aArray[point].RegisterPoint(pos);
}

// 0x7174F0
void CPlaneTrails::Update() {
    const uint32_t coronaBaseId = 101;

    aArray[0].Update(CVector(2590.0f, 2200.0f, 550.0f), CRGBA(255, 0, 0, 255), coronaBaseId, 0, 22, 7);
    aArray[1].Update(CVector(2000.0f, -2600.0f, 500.0f), CRGBA(255, 255, 128, 255), coronaBaseId + 1, 350, 21, 7);
    aArray[2].Update(CVector(2100.0f, 1300.0f, 600.0f), CRGBA(255, 255, 255, 255), coronaBaseId + 2, 200, 20, 6);
}