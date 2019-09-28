#include "stdafx.h"
#include "LaughingGas.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"
#include "stdlib.hpp"
#include "PsxDisplay.hpp"
#include "PsxRender.hpp"
#include "Events.hpp"

ALIVE_VAR(1, 0x5BC214, int, gGasInstanceCount_5BC214, 0);
ALIVE_VAR(1, 0x5C1BA4, short, gLaughingGasOn_5C1BA4, FALSE);


LaughingGas* LaughingGas::ctor_432400(__int16 layer, int /*notUsed*/, Path_LaughingGas* pTlv, int tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x545000);

    gGasInstanceCount_5BC214++;

    field_4_typeId = Types::eLaughingGas_31;
    Path_LaughingGas_Data* pData = &field_48_tlv_data;
    pData->field_0_is_laughin_gas = pTlv->field_10_is_laughing_gas;
    pData->field_2_gas_id = pTlv->field_12_gas_id;

    pData->field_4_red_percent = pTlv->field_14_red_percent;
    pData->field_6_blue_percent = pTlv->field_18_blue_percent;
    pData->field_8_green_percent = pTlv->field_18_blue_percent;

    if (field_48_tlv_data.field_0_is_laughin_gas)
    {
        field_36_bGreen = 1;
    }
    else
    {
        field_36_bGreen = 0;
    }

    if (SwitchStates_Get_466020(field_48_tlv_data.field_2_gas_id))
    {
        field_34_bEnabled = 1;
        field_54_amount_on = FP_FromInteger(1);
    }
    else
    {
        field_34_bEnabled = 0;
        field_54_amount_on = FP_FromInteger(0);
    }

    field_24_tlvInfo = tlvInfo;
    field_58_layer = layer;

    gObjList_drawables_5C1124->Push_Back(this);
    field_6_flags.Set(BaseGameObject::eDrawable);

    field_28_rect.x = FP_GetExponent(FP_FromInteger(pTlv->field_8_top_left.field_2_y) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);
    field_28_rect.y = FP_GetExponent(PsxToPCX(FP_FromInteger(pTlv->field_8_top_left.field_0_x) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x));

    field_28_rect.w = FP_GetExponent(FP_FromInteger(pTlv->field_C_bottom_right.field_2_y) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);
    field_28_rect.h = FP_GetExponent(PsxToPCX(FP_FromInteger(pTlv->field_C_bottom_right.field_0_x) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x));
    
    field_31F8_h_count = (field_28_rect.h - field_28_rect.y) / 4;
    field_31FC_w_count = (field_28_rect.w - field_28_rect.x + 2) / 2;

    field_19C_pMem = static_cast<short*>(malloc_non_zero_4954F0(sizeof(short) * field_31FC_w_count * field_31F8_h_count));
    
    Init_432980();
    VUpdate();

    return this;
}

void LaughingGas::dtor_432B80()
{
    SetVTable(this, 0x545000);
    Path::TLV_Reset_4DB8E0(field_24_tlvInfo, -1, 0, 0);
    gObjList_drawables_5C1124->Remove_Item(this);
    gLaughingGasOn_5C1BA4 = FALSE;
    gGasInstanceCount_5BC214--;
    Mem_Free_495540(field_19C_pMem);
    BaseGameObject_dtor_4DBEC0();
}

BaseGameObject* LaughingGas::VDestructor(signed int flags)
{
    return vdtor_432670(flags);
}

void LaughingGas::VScreenChanged()
{
    vScreenChanged_432DE0();
}

void LaughingGas::VUpdate()
{
    vUpdate_432C40();
}

void LaughingGas::VRender(int** pOt)
{
    vRender_432D10(pOt);
}

void LaughingGas::vScreenChanged_432DE0()
{
    field_6_flags.Set(BaseGameObject::eDead);
}

const float dword_551C58[7] = { 1.0,  5.0,  10.0,  10.0,  5.0,  1.0,  0.0 };

void LaughingGas::Init_432980()
{
    int yCount = 0;
    if (field_31F8_h_count > 0)
    {
        float v1 = 0.0f;
        float* pElem_ptr = &field_1A0_x_data[0].unknown2;
        do
        {
            float v4 = (float)yCount / (float)field_31F8_h_count;
            float * pDst1 = (pElem_ptr - 5);
            v1 = 1.0f - v4;
            float val1 = 1.0f;

            for (float pSrc1 : dword_551C58)
            {
                const float dstVal1 = val1 * pSrc1;
                pDst1 += 4;
                *(float *)(pDst1 - 4) = dstVal1;
                val1 = val1 * v4;
            }
            
            float v9 = 1.0f;
            float* v10 = pElem_ptr;
            for (int i = 0; i < 6; i++)
            {
                const float v12 = v9 * *v10;
                --v10;
                v10[1] = v12;
                v9 = v9 * v1;
            }

            ++yCount;
            pElem_ptr += 7;
        } while (yCount < field_31F8_h_count);
    }

    int xCount = 0;
    if (field_31FC_w_count > 0)
    {
        float* v14 = (float *)field_24D0_y_data;
        float v2 = 0.0f;
        do
        {
            const float v15 = (float)xCount / (float)field_31FC_w_count;
            float* v17 = v14 - 5;
            v2 = 1.0f - v15;
            float v18 = 1.0f;
            for (float v16 : dword_551C58)
            {
                const float v19 = v18 * v16;
                ++v17;
                *(v17 - 1) = v19;
                v18 = v18 * v15;
            }

            float v20 = 1.0f;
            float* v21 = v14;
            for (int i = 0; i < 6; i++)
            {
                const float v23 = v20 * *v21;
                --v21;
                v21[1] = v23;
                v20 = v20 * v2;
            }
            ++xCount;
            v14 += 7;
        } while (xCount < field_31FC_w_count);
    }

    float* p66_array_2 = &field_10C_ary6[0][0];
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            *p66_array_2 = rand() * 6.28f * 0.00003051850947599719f;
            p66_array_2++;
        }
    }

    field_5C_prim.mPrimHeader.rgb_code.code_or_pad = PrimTypeCodes::eGas;
    field_5C_prim.x = field_28_rect.x;
    field_5C_prim.y = field_28_rect.y;
    field_5C_prim.w = field_28_rect.w;
    field_5C_prim.h = field_28_rect.h;
    field_5C_prim.pData = field_19C_pMem;

}

LaughingGas* LaughingGas::vdtor_432670(signed int flags)
{
    dtor_432B80();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void LaughingGas::vRender_432D10(int** pOt)
{
    if (field_54_amount_on > FP_FromDouble(0.1) || !field_36_bGreen)
    {
        if (field_19C_pMem)
        {
            if (sbDisplayRenderFrame_55EF8C)
            {
                DoRender_432740();
            }
            OrderingTable_Add_4F8AA0(&pOt[field_58_layer], &field_5C_prim.mPrimHeader);
        }
        pScreenManager_5BB5F4->InvalidateRect_40EC10(0, 0, gPsxDisplay_5C1130.field_0_width, gPsxDisplay_5C1130.field_2_height);
    }
}

void LaughingGas::DoRender_432740()
{
    float local_array[6];

    short*  memPtr = field_19C_pMem;
    int rgb_base = (1 << sRedShift_C215C4) + (1 << sGreenShift_C1D180);
    if (!field_36_bGreen)
    {
        rgb_base = (1 << sBlueShift_C19140) + (1 << sRedShift_C215C4) + (1 << sGreenShift_C1D180);
    }

    for (int xCount = 0; xCount < field_31FC_w_count; ++xCount)
    {
        for (int p = 0; p < 6; p++)
        {
            local_array[p] = Calc_X_4326F0(&field_7C[p][0], xCount);
        }

        for (int yCount = 0; yCount < field_31F8_h_count; ++memPtr)
        {
            float yValue = Calc_Y_4326A0(local_array, yCount);
            if (yValue > 0.0f)
            {
                if (yValue >= 3.0f)
                {
                    if (yValue > 31.0f)
                    {
                        yValue = 31.0f;
                    }
                }
                else
                {
                    yValue = 3.0f;
                }
            }
            else
            {
                yValue = 0.0f;
            }

            *memPtr = static_cast<WORD>(rgb_base * (static_cast<BYTE>(yValue) & 30));
            ++yCount;
        }
    }
}

__int16 LaughingGas::CounterOver_432DA0()
{
    return field_54_amount_on >= FP_FromDouble(0.3);
}

void LaughingGas::vUpdate_432C40()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (SwitchStates_Get_466020(field_48_tlv_data.field_2_gas_id))
    {
        field_34_bEnabled = 1;
    }
    else
    {
        field_34_bEnabled = 0;
    }

    if (field_36_bGreen)
    {
        if (field_34_bEnabled)
        {
            if (field_54_amount_on < FP_FromInteger(1))
            {
                field_54_amount_on += FP_FromDouble(0.01);
            }
        }
        else
        {
            if (field_54_amount_on > FP_FromInteger(0))
            {
                field_54_amount_on -= FP_FromDouble(0.01);
            }
        }
    }

    if (field_36_bGreen)
    {
        gLaughingGasOn_5C1BA4 = 1;
        if (CounterOver_432DA0())
        {
            sub_4328A0();
            return;
        }
    }

    gLaughingGasOn_5C1BA4 = 0;
    sub_4328A0();
}

float LaughingGas::Calc_Y_4326A0(float* a2, int yIndex)
{
    float result = 0.0;
    float* v4 = a2 + 1;
    for (float& v5 : field_1A0_x_data[yIndex].array_4)
    {
        result += v5 * *v4;
        v4++;
    }
    return result;
}

float LaughingGas::Calc_X_4326F0(float* a2, int xIndex)
{
    float result = 0.0;
    float* v4 = a2 + 1;
    for (float& v5 : field_24D0_y_data[xIndex].array_4)
    {
        result += v5 * *v4;
        v4++;
    }
    return result;
}

void LaughingGas::sub_4328A0()
{
    NOT_IMPLEMENTED();
}
