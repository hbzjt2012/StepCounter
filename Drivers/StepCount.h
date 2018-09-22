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

#ifndef __STEPCOUNT_H
#define __STEPCOUNT_H
    
typedef signed short      DATATYPE;
#define DATASIZE    10
typedef struct __DATA {
    DATATYPE Data[DATASIZE];
    DATATYPE Max;
    DATATYPE MaxMark;
    DATATYPE Min;
    DATATYPE MinMark;
    DATATYPE Base;
    DATATYPE UpLimit;
    DATATYPE DownLimit;
}__DATA;

typedef struct _SensorData {
    __DATA X;
    __DATA Y;
    __DATA Z;
}SensorData;

void DataInit(SensorData *SData);
unsigned char DataSelect(SensorData *GMeter);
void CountStepInit(void);

#endif

/* [] END OF FILE */
