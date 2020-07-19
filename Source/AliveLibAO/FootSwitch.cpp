#include "stdafx_ao.h"
#include "Function.hpp"
#include "FootSwitch.hpp"
#include "ResourceManager.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "BaseAliveGameObject.hpp"

START_NS_AO

FootSwitch* FootSwitch::ctor_4887F0(Path_FootSwitch* pTlv, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BD648);
    field_4_typeId = Types::eFootSwitch36;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2010, 1, 0);
    Animation_Init_417FD0(744, 68, 15, ppRes, 1);

    field_10_anim.field_C_layer = 25;

    field_EA_id = pTlv->field_18_id;
    if (pTlv->field_1A_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }

    field_E8_state = States::eWaitForStepOnMe_0;
    field_EC_action = pTlv->field_1C_action;
    field_EE_trigger_by = pTlv->field_1E_trigger_by;

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x + 12);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);

    SwitchStates_Set(field_EA_id, 0);

    field_E4_tlvInfo = tlvInfo;
    return this;
}

BaseGameObject* FootSwitch::VDestructor(signed int flags)
{
    return vdtor_488C10(flags);
}

FootSwitch* FootSwitch::vdtor_488C10(signed int flags)
{
    dtor_4889E0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* FootSwitch::dtor_4889E0()
{
    SetVTable(this, 0x4BD648);
    if (field_F0_pStoodOnMe)
    {
        field_F0_pStoodOnMe->field_C_refCount--;
        field_F0_pStoodOnMe = nullptr;
    }
    gMap_507BA8.TLV_Reset_446870(field_E4_tlvInfo, -1, 0, 0);
    return dtor_417D10();
}

void FootSwitch::VScreenChanged()
{
    VScreenChanged_4889D0();
}

void FootSwitch::VScreenChanged_4889D0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

END_NS_AO
