#include "stdafx.h"
#include "ROV_Control.h"

ROV_Control::ROV_Control(void)
{
//houmb
	m_temproll=0;
    m_tempitch=0;
	m_temyaw=0;
	m_temspeed=0;
	m_temdepth=0;
	m_camerapitch=0;//��̨����
	m_camerarotate=0;//��̨��ת
	m_camerazoom=0;//��ͷ�䱶
	m_camerafocus=0;//��ͷ�۽�
	m_leftlight=0;//��ͷ�۽�
	m_rightlight=0;//��ͷ�۽�

	m_left_forward_motor=0 ;
	m_left_backward_motor=0 ;
	m_right_forward_motor=0 ;
	m_right_backward_motor=0 ;
	m_left_H_motor=0;
	m_right_H_motor=0;

	m_need_reset=false;
	m_Mutex=::CreateMutex(NULL, FALSE, NULL);
}
ROV_Control::~ROV_Control()
{
	::CloseHandle(m_Mutex);
}
void ROV_Control::ROV_CMD_Init()
{
    CMD_RCtoROV.data_start[0] = 'S';
    CMD_RCtoROV.data_start[1] = 'T';
    CMD_RCtoROV.data_start[2] = 'R';
    CMD_RCtoROV.data_start[3] = 'C';
    CMD_RCtoROV.data_start[4] = 1;
    CMD_RCtoROV.data_start[5] = 0;
    CMD_RCtoROV.data_start[6] = 0;
    CMD_RCtoROV.data_start[7] = 0;
    CMD_RCtoROV.ATT.pitch = 0;
    CMD_RCtoROV.ATT.roll = 0;
    CMD_RCtoROV.ATT.yaw = 0;
    CMD_RCtoROV.ATT.forwar_backwar = 0;
    CMD_RCtoROV.ATT.left_right = 0;
    CMD_RCtoROV.ATT.up_down = 0;
    CMD_RCtoROV.ATT.operat_mode = 0;//Ĭ����ͣתģʽ
    CMD_RCtoROV.ATT.yewmode = 0;//(Options_New[0x1A][0]&0x01) + ((Options_New[0x1A][1]&0x01)<<1) ; //1;//��������   0:���ٿ���(-100%~+100%) 1:��̬����(-180~+180)
    CMD_RCtoROV.ATT.depth_aim = 0;//Ŀ����ȣ��߶ȣ�Ϊ0
    CMD_RCtoROV.camdata[0].focus_mode = 0;//�����Ծ���ֵ��ʽ����
    CMD_RCtoROV.ATT.Att_Calibration = 0;//��̬У׼
    CMD_RCtoROV.extraman.switching = 0;
    CMD_RCtoROV.ATT.reserved[0] = 0;
    CMD_RCtoROV.ATT.reserved[1] = 0;
    CMD_RCtoROV.ATT.reserved[2] = 0;
	//Options_New[0x32][0] = 0;
	CMD_RCtoROV.power = 0;//((Options_New[0x32][0]&0X01) + ((Options_New[0x32][1]&0X01)<<1) + ((Options_New[0x33][0]&0X01)<<2) + ((Options_New[0x33][1]&0X01)<<3));
	CMD_RCtoROV.OSD_Set &=~(1<<6) ;//�򿪸㹦��ģʽ//(Options_New[0x23][0]&0X01) + ((Options_New[0x22][0]&0X01)<<1) + ((Options_New[0x22][1]&0X01)<<2) + ((Options_New[0x23][1]&0X01)<<3) + ((Options_New[0x34][0]&0X01)<<6);
	CMD_RCtoROV.OSD_Set |=(1<<4);	//OSD_Set�ĵ�4λ �Զ��Խ�ģʽ���� �����Զ����Զ��Խ�ģʽ
	
	CMD_RCtoROV.Send_Cnt = 0;

    CMD_RCtoROV.data_end[0] = 'E';
    CMD_RCtoROV.data_end[1] = 'D';
    CMD_RCtoROV.data_end[2] = 'R';
    CMD_RCtoROV.data_end[3] = 'C';
    CMD_RCtoROV.data_end[4] = 1;
    CMD_RCtoROV.data_end[5] = 0;
    CMD_RCtoROV.data_end[6] = 0;
    CMD_RCtoROV.data_end[7] = 0;
}

void ROV_Control::Set_Reset_Flag(bool flag)
	{
		WaitForSingleObject(m_Mutex,INFINITE);
		m_need_reset=flag;
		ReleaseMutex(m_Mutex);
	}
bool ROV_Control::Get_Reset_Flag()
	{
		return m_need_reset;
	}


void ROV_Control::ROV_RC_CMD_Assemble()

{
	
    if(CMD_RCtoROV.ATT.operat_mode == 0)//ֹͣģʽʱ
    {
    	if(STAT_ROVtoRC.depth<0) CMD_RCtoROV.ATT.depth_aim = 0;
    		else 
        CMD_RCtoROV.ATT.depth_aim = STAT_ROVtoRC.depth; //���ȡ��ǰֵ
		if((CMD_RCtoROV.ATT.yewmode&0x01)==0x01) 		//��̬���ƺ���
        	CMD_RCtoROV.ATT.yaw = STAT_ROVtoRC.ATT.yaw; //����ȡ��ǰֵ
        else CMD_RCtoROV.ATT.yaw = 0;					//���ٿ��ƺ���  ����Ϊ0
        CMD_RCtoROV.ATT.pitch =STAT_ROVtoRC.ATT.pitch;//����ȡ��ǰֵ
        CMD_RCtoROV.ATT.roll = STAT_ROVtoRC.ATT.roll;//���ȡ��ǰֵ
        //CMD_RCtoROV.camdata[0].elevation = STAT_ROVtoRC.camdata[0].elevation;//��̨
        //CMD_RCtoROV.camdata[0].rotate = STAT_ROVtoRC.camdata[0].rotate;//��̨
        //CMD_RCtoROV.camdata[0].focus = STAT_ROVtoRC.camdata[0].focus;//��ͷ
        //CMD_RCtoROV.camdata[0].zoom = STAT_ROVtoRC.camdata[0].zoom;//��ͷ
        //CMD_RCtoROV.camdata[0].leftlight_lum = STAT_ROVtoRC.camdata[0].leftlight_lum;//�ƹ�
        //CMD_RCtoROV.camdata[0].rightlight_lum = STAT_ROVtoRC.camdata[0].rightlight_lum;//�ƹ�
	}
    else if(CMD_RCtoROV.ATT.operat_mode == 2)
    {

		//ROLL

						CMD_RCtoROV .ATT .roll =m_temproll;
		//PITCH	
						CMD_RCtoROV .ATT .pitch =m_tempitch;
					
		//Heading
						CMD_RCtoROV .ATT .yaw =m_temyaw;

		//Speed
						CMD_RCtoROV .ATT .forwar_backwar =m_temspeed;
		//Depth
						CMD_RCtoROV .ATT .depth_aim  =m_temdepth;
				//������һ��������ǰ������ ʱ�� �����˶�ģʽΪ�ֶ�ģʽ		
				if(((CMD_RCtoROV.ATT.forwar_backwar>15)||(CMD_RCtoROV.ATT.forwar_backwar<-15))&&((CMD_RCtoROV.ATT.pitch<=-50)||(CMD_RCtoROV.ATT.pitch>=50)))
					{			
						CMD_RCtoROV.ATT.operat_mode = 1;
						CMD_RCtoROV.ATT.depth_aim = STAT_ROVtoRC.depth; //���ȡ��ǰֵ,��ʱ���ָ��û�ã�ȡ��ǰ���Ϊ ���붨��ģʽʹ��
					}
				else //��û�и���ʱǰ���˶�������Ϊ����ģʽ
					{						
						CMD_RCtoROV.ATT.operat_mode = 2;	
					
						
						if(CMD_RCtoROV.ATT.depth_aim- STAT_ROVtoRC.depth>Depth_Offset) //Ŀ����ȳ���ʵ�����
						{
								CMD_RCtoROV.ATT.depth_aim = STAT_ROVtoRC.depth+Depth_Offset;
						}
						else if(CMD_RCtoROV.ATT.depth_aim- STAT_ROVtoRC.depth<(Depth_Offset*(-1)))
						{
							CMD_RCtoROV.ATT.depth_aim = STAT_ROVtoRC.depth-Depth_Offset;
						}
					}
					
				if(CMD_RCtoROV.ATT.depth_aim < 0)
				{
          			CMD_RCtoROV.ATT.depth_aim = 0;
				}
          		else if(CMD_RCtoROV.ATT.depth_aim > 32000)
				{
            		CMD_RCtoROV.ATT.depth_aim = 32000;

				}

			//leftlight_lum/rightlight_lum//�ƹ� 

			




			  //�������
    }
	else if(0xff==CMD_RCtoROV.ATT.operat_mode)//����ģʽ
			  {
				CMD_RCtoROV.motor .l_f_v_motor =m_left_forward_motor ;
				CMD_RCtoROV.motor.l_b_v_motor =m_left_backward_motor ;
				CMD_RCtoROV.motor.r_f_v_motor =m_right_forward_motor ;
				CMD_RCtoROV.motor.r_b_v_motor =m_right_backward_motor ;
				CMD_RCtoROV.motor.l_h_motor =m_left_H_motor;
				CMD_RCtoROV.motor.r_h_motor =m_right_H_motor;

    //    CMD_RCtoROV.camdata[0].rightlight_lum = CMD_RCtoROV.camdata[0].leftlight_lum;//�ƹ�
      
    }
	 CMD_RCtoROV.camdata[0].leftlight_lum = m_leftlight ;   
	 CMD_RCtoROV.camdata[0].rightlight_lum  =m_rightlight  ;  
	 CMD_RCtoROV.camdata[0].focus =m_camerafocus   ;  
	 CMD_RCtoROV.camdata[0].zoom =m_camerazoom   ;  
     CMD_RCtoROV.camdata[0].rotate =m_camerarotate   ;  
	 CMD_RCtoROV.camdata[0].elevation  =m_camerapitch   ;  

	 if(Get_Reset_Flag())//��Ҫ������
	 {
		 CMD_RCtoROV .power|=(1<<15);
		Set_Reset_Flag(false);
	 }
	 else
	 {
		  CMD_RCtoROV .power&=~(1<<15);
	 }
	CMD_RCtoROV.Send_Cnt++;
    CMD_RCtoROV.crc16 = Tcp_Crc16_Check((unsigned char *)&CMD_RCtoROV + 8, sizeof(ROV_CMD) - 18);//crcУ��ֵ��������ʼ�ͽ�β�ı�־
	
}