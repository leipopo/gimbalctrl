#ifndef TOF_H
#define TOF_H

#define TOF_RX_BUF_NUM 94u

#define TOF_FRAME_LENGTH 47u
typedef struct LidarPointStructDef
{
	uint16_t distance;//距离
	uint8_t intensity;//置信度
	
}Point_Data;
void tofinit();
extern float distance;
#endif // !