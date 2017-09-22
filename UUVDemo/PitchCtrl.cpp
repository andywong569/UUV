#include "stdafx.h"
#include "PitchCtrl.h"


/** 
*  @brief             PID�㷨 
*  @param[in]         ��ֵ 
*  @param[in,out]     pid�ṹ��
*  @return            pid������
*  @remarks      
*  @see          
*/  
float PIDCalc(float Error, struct PID *PID_x)
{
	float dError;
	float d_out, out;
	float temp;

	dError = Error - PID_x->LastError;               // diretative

	d_out = PID_x->P * dError                                           //P
		+ PID_x->I * Error                                            //I
		+ PID_x->D * (Error - 2 * PID_x->LastError + PID_x->PrevError); //D

	out = d_out + PID_x->Last_u;
	PID_x->PrevError = PID_x->LastError;
	PID_x->LastError = Error;
	PID_x->Last_u = out;
	temp = out;
	return temp;
}


/** ��������巢���ݵĽṹ�� */ 
Result result = { 0 };


/** 
*  @brief       �ں�pid����Ľ�� 
*  @param[in]   fx��ǰ�������ٶ�   
*  @param[in]   depth��pid��������
*  @param[in]   roll��pid����ĺ��
*  @param[in]   pitch��pid����ĸ���
*  @param[in]   yaw��pid����ĺ����
*  @return      ��
*  @remarks      
*  @see          
*/  
void ControlTask(float fx, float depth, float roll, float pitch, float yaw)
{
	result.vLeftFront =  depth - roll - pitch;
	result.vLeftBack =  depth - roll + pitch;
	result.vRightBack =  depth + roll + pitch;
	result.vRightFront =  depth + roll - pitch;

	result.hLeft = fx + yaw;
	result.hRight = fx - yaw;
}


/** 
*  @brief       ������������� 
*  @param[in]   index�����������ֵ 
*  @param[in]   speed���ٶ�ֵ
*  @return      ��
*  @remarks      
*  @see          
*/  
void SendMotorSpeed( UUVMOTORINDEX index, int speed )
{
	unsigned char move_pattrn = 0xff;
	if(!m_pUUV->UUV_Set(UUV_WORK_PATTERN_SET, &move_pattrn))
	{
		printf("���õ���ģʽʧ�ܣ������� %d", m_pUUV->UUV_GetErrno());
		return;
	}

	UUVMOTOR_T motor;
	motor.motorindex = index;
	motor.motorspeed = speed;
	if(!m_pUUV->UUV_Set(UUV_MOTOR_SPEED_SET, &motor))
	{
		printf("���õ��%x���󣬴����� %d", motor.motorindex, m_pUUV->UUV_GetErrno());
	}
}


/** ������ȡ�����������������pid�ṹ�� */ 
struct PID pid_depth, pid_roll, pid_pitch, pid_yaw;


/** �����û������Ŀ��ṹ�� */ 
Target target = { 0 };


/** 
*  @brief       �û�����Ŀ��ֵ���Ժ���
*  @return      ��
*  @remarks      
*  @see          
*/ 
void PitchCtrlTest()
{
	printf("���������Ŀ��ֵ����ʽΪ���ٶȡ��������������ȡ�����\n");
	scanf("%f%f%f%f%f", &target.speed, &target.roll, &target.pitch, &target.depth, &target.yaw);
	printf("Ŀ��ֵΪ��%f, %f, %f, %f, %f\n", target.speed, target.roll, target.pitch, target.depth, target.yaw);

	pid_depth.P = 1000;
	pid_depth.I = 4;
	pid_roll.P = 35;
	pid_roll.I = 0.25;
	pid_roll.D = 0.2;
	pid_pitch.P = 40;
	pid_pitch.I = 0.3;
	pid_pitch.D = 0.2;
	pid_yaw.P = 20;
	pid_yaw.I = 0.4;
	pid_yaw.D = 0.1;
	//	PID�㷨
	printf("PID ������...\n");
	while(true)
	{
		float tmpRoll = PIDCalc(target.roll - puuv_status->ATT.roll*0.1, &pid_roll);
		float tmpPitch = PIDCalc(target.pitch - puuv_status->ATT.pitch*0.1, &pid_pitch);
		float tmpDepth = PIDCalc(target.depth - puuv_status->depth*0.01, &pid_depth);
		float tmpYaw = PIDCalc(target.yaw - puuv_status->ATT.yaw*0.1, &pid_yaw);

		ControlTask(target.speed, tmpDepth, tmpRoll, tmpPitch, tmpYaw);

		//	��ӡ���
		printf("\n��ǰ��%d", result.vLeftFront);
		printf("\t���%d", result.vLeftBack);
		printf("\t�Һ�%d", result.vRightBack);
		printf("\t��ǰ��%d", result.vRightFront);
		printf("\tˮƽ��%d", result.hLeft);
		printf("\tˮƽ�ң�%d", result.hRight);

		//	��������
		SendMotorSpeed(V_L_F_MOTORINDEX, result.vLeftFront);
		SendMotorSpeed(V_L_B_MOTORINDEX, result.vLeftBack);
		SendMotorSpeed(V_R_B_MOTORINDEX, result.vRightBack);
		SendMotorSpeed(V_R_F_MOTORINDEX, result.vRightFront);
		SendMotorSpeed(H_L_MOTORINDEX, result.hLeft);
		SendMotorSpeed(H_R_MOTORINDEX, result.hRight);
	}
}
