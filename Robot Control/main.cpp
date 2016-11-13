#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <iostream>
#include "spi_driver.h"

using namespace robot_control;
//using namespace std;

int main()
{
	char cmd[255];
	printf("Begin SPI Test\n");
	ControllerPort port;
	while(1)
	{
		printf("Input the Function name\n");;
		scanf("%s",cmd);
		fflush(stdin);
		if(strcmp(cmd,"exit")==0)
			break;
		if(strcmp(cmd,"mvf") == 0)
		{
			printf("Move Forward Command, Argument Format:\nspeed(0-255),distance(0-255)\n");
			unsigned char sp,dist;
			scanf("%hhd,%hhd",&sp,&dist);
			fflush(stdin);
			port.MoveForward(sp,dist);
		}
		else if(strcmp(cmd,"mvb") == 0)
		{
			printf("Move Back Command, Argument Format:\nspeed(0-255),distance(0-255)\n");
			unsigned char sp,dist;
			scanf("%hhd,%hhd",&sp,&dist);
			fflush(stdin);
			port.MoveBackward(sp,dist);
		}
		else if(strcmp(cmd,"mv")==0)
		{
			printf("Move Command, Argument Format:\ndirection(1:cw,2:ccw),speed(0-255),distance(0-255)\n");
			unsigned char d,sp,dist;
			scanf("%hhd,%hhd,%hhd",&d,&sp,&dist);
			fflush(stdin);
			port.Movement(d,sp,dist);
		}
		else if(strcmp(cmd,"head") == 0)
		{
			printf("Heading Command, Argument Format:\ndirection(0:cw,1:ccw),angle(0-360)\n");
			Deflection def;
			def.data = 0x0000;
			unsigned char d;
			unsigned short agl;
			scanf("%hhd,%hd",&d,&agl);
			fflush(stdin);
			//printf("D:%d,A:%d",d,agl);
			def.turn.direction = d;
			def.turn.angle1 = (unsigned char)((agl>>8)&0x01);
			def.turn.angle2 = (unsigned char)(agl&0xff);
			printf("Heading Value: 0x%x\n",def.data);
			port.Heading(def);
		}
		else if(strcmp(cmd,"ss") == 0)
		{
			printf("Speed Slope Command, Argument Format:\ncoeff(0-255)\n");
			unsigned char coef;
			scanf("%hhd",&coef);
			fflush(stdin);
			port.SpeedSlope(coef);
		}
		else if(strcmp(cmd,"stop")==0)
		{
			printf("Stop Command\n");
			port.Stop();
		}
		else if(strcmp(cmd,"lmsp")==0)
		{
			printf("Set the left motor speed Command, Argument Format:\nspeed(0-255)\n");
			unsigned char sp;
			scanf("%hhd",&sp);
			fflush(stdin);
			port.SetLMotorSpeed(sp);
		}
		else if(strcmp(cmd,"rmsp")==0)
		{
			printf("Set the right motor speed Command, Argument Format:\nspeed(0-255)\n");
			unsigned char sp;
			scanf("%hhd",&sp);
			fflush(stdin);
			port.SetRMotorSpeed(sp);
		}
		else if(strcmp(cmd,"qsr")==0)
		{
			printf("Quey Sensor Command, Argument Format:\nsensorid\n");
			unsigned short id;
			scanf("%hd",&id);
			fflush(stdin);
			int value = port.QuerySensor(id);
			printf("Returned Value:%d\n",value);
		}
		else if(strcmp(cmd,"loff")==0)
		{
			printf("Light OFF Command\n");
			port.LightOff();
		}
		else if(strcmp(cmd,"irdc")==0)
		{
			printf("Set the brightness of IR light Command, Argument Format:\nbrightness(0-255)\n");
			unsigned char dc;
			scanf("%hhd",&dc);
			fflush(stdin);
			port.SetIRLight(dc);
		}
		else if(strcmp(cmd,"wdc")==0)
		{
			printf("Set the brightness of White light Command, Argument Format:\nbrightness(0-255)\n");
			unsigned char dc;
			scanf("%hhd",&dc);
			fflush(stdin);
			port.SetWLight(dc);
		}
		else if(strcmp(cmd,"camstb")==0)
		{
			printf("Camera Stable Command\n");
			port.CamStable();
		}
		else if(strcmp(cmd,"camp") == 0)
		{
			printf("Pause the robot, Argument Format:\n 0: Resume 1: Pause\n");
			unsigned char p;
			scanf("%hhd",&p);
			fflush(stdin);
			port.CamPause(p);
		}
		else if(strcmp(cmd,"camh") == 0)
		{
			printf("Set the height of Camera Command, Argument Format:\nheight(0-1024)\n");
			unsigned short h;
			scanf("%hd",&h);
			fflush(stdin);
			port.CamHeight(h);
		}
		else if(strcmp(cmd,"camagl") == 0)
		{
			printf("Set the angle of Camera Command, Argument Format:\nangle(0-1024)\n");
			unsigned short agl;
			scanf("%hd",&agl);
			fflush(stdin);
			port.CamAngle(agl);
		}
		else if(strcmp(cmd,"camf") == 0)
		{
			printf("Camera Filter, Argument Format:\n 0: Disable 1: Enable\n");
			unsigned char f;
			scanf("%hhd",&f);
			fflush(stdin);
			port.CamFilter(f);
		}else{
			printf("Unknown command, available command: \n mvf,mvb,mv,head,ss,stop,lmsp,rmsp,qsr,loff,irdc,wdc,camstb,camp,camh,camagl,camf\n exit to exit");
		}
	}
	
}
