/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "StepCount.h"

SensorData GMeter;

void DataInit(SensorData *SData)
{
    SData->X.Max = -10;
    SData->X.Min = +10;
    SData->Y.Max = -10;
    SData->Y.Min = +10;
    SData->Z.Max = -10;
    SData->Z.Min = +10;
}

unsigned char DataSelect(SensorData *GMeter)
{
    DATATYPE tempX = 0, tempY = 0, tempZ = 0, tempMax;
    unsigned char flag = 0;
    tempX = GMeter->X.Max - GMeter->X.Min;
    tempY = GMeter->Y.Max - GMeter->Y.Min;
    tempZ = GMeter->Z.Max - GMeter->Z.Min;
    if(tempX > tempY) {
        if(tempX > tempZ) {
            flag = 1;
            tempMax = tempX;
        } else {
            flag = 3;
            tempMax = tempZ;
        }
    } else {
        if(tempY > tempZ) {
            flag = 2;
            tempMax = tempY;
        } else {
            flag = 3;
            tempMax = tempZ;
        }
    }
    if(tempMax > 50) {
        return flag;
    } else {
        return 0;
    }
}

void CountStepInit(void)
{
    DataInit(&GMeter);
}

/* [] END OF FILE */
