#pragma once
class config
{
public:
	enum RGB_CHANNEL{
		CHANNEL_R = 1,
		CHANNEL_G = 2,
		CHANNEL_B = 4,
	};
	config();
	~config();
	void getRGBTherhold(RGB_CHANNEL ch,unsigned char * start, unsigned char* end);
	unsigned char setRGBTherhold(RGB_CHANNEL ch,unsigned char start, unsigned char end);
private:
	unsigned char rstart, rend, gstart, gend, bstart, bend;
};

