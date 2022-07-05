#pragma once
#include "sdk/classes/CUtlVector.h"
#include "utilities/structures/vec3d/vec3d.h"

class Entity;
class CGlowObjectManager
{
public:
    struct GlowObjectDefinition_t
    {
    public:
        int nextFreeSlot;

        // entity the object belongs to
        void* entity;

        Vec3D m_vGlowColor;
        float m_flGlowAlpha;

        bool glowAlphaCappedByRenderAlpha;
        float glowAlphaFunctionOfMaxVelocity;
        float glowAlphaMax;
        float glowPulseOverdrive;

        bool renderWhenOccluded;
        bool renderWhenUnoccluded;

        bool fullBloomRender;
        int fullBloomStencilTestValue;
        int glowStyle;
        int splitScreenSlot;

        static constexpr int END_OF_FREE_LIST = -1;
        static constexpr int ENTRY_IN_USE = -2;

        bool IsUnused()
        {
            return nextFreeSlot != ENTRY_IN_USE;
        }

        // helper function to set the glow color
        void SetColor(float* col)
        {
            m_vGlowColor.x = col[0];
            m_vGlowColor.y = col[1];
            m_vGlowColor.z = col[2];
            m_flGlowAlpha = col[3];

            renderWhenOccluded = true;
            renderWhenUnoccluded = false;
        }
    };

    CUtlVector<GlowObjectDefinition_t> glowObjects;
};