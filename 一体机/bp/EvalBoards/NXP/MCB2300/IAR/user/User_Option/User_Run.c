#include "includes.h"
struct LMT_CONTRL_TYPE  gLMT_Power, gPC_power;
OS_EVENT *FRun;			//��Ϣ����
/////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void Config_printf(uint8 page_id)
{
    uint32 tmp;//,i;
    uint8 buf[40], *pdata, err;
    //struct BASE_MSG *pmsg;
    struct CLASS_INFO *pclass;
    //struct CLASS_OPION_TYPE *pCLASS_OPtion;
    //struct CLASS_TABLE_TYPE *pclass_table;
    //struct CLASS_TABLE_REC *pclass_Rec;
    //struct DEVICE_INFO *pdev;

    if (page_id == 0)
    {
        pclass = (struct CLASS_INFO *)Get_ClassInfo_Table(0);
        IP_printf("ѧУ��:%s", &pclass->school_name[0]);
        IP_printf("��ѧ¥:%s", &pclass->house_name[0]);
        IP_printf("������:%s", &pclass->class_name[0]);
        IP_printf("��������:%d", pclass->class_type);
        tmp = Get_ver();
        IP_printf("һ�������汾:%d.%d", tmp / 256, tmp & 0xff);
        Mem_Clr(buf, 10);
        Mem_Copy(buf, pclass->class_ID, 8);
        IP_printf("����ID:%s", buf);
        IP_printf("����IP=%d.%d.%d.%d", pclass->class_ip.IP[0], pclass->class_ip.IP[1], pclass->class_ip.IP[2], pclass->class_ip.IP[3]);
        IP_printf("����IP MASK=%d.%d.%d.%d", pclass->class_ip.Mask[0], pclass->class_ip.Mask[1], pclass->class_ip.Mask[2], pclass->class_ip.Mask[3]);
        IP_printf("����IP=%d.%d.%d.%d", pclass->Gateway_ip.IP[0], pclass->Gateway_ip.IP[1], pclass->Gateway_ip.IP[2], pclass->Gateway_ip.IP[3]);
        IP_printf("����IP MASK=%d.%d.%d.%d", pclass->Gateway_ip.Mask[0], pclass->Gateway_ip.Mask[1], pclass->Gateway_ip.Mask[2], pclass->Gateway_ip.Mask[3]);
        IP_printf("����IP=%d.%d.%d.%d", pclass->Ser_ip.IP[0], pclass->Ser_ip.IP[1], pclass->Ser_ip.IP[2], pclass->Ser_ip.IP[3]);
        IP_printf("����IP MASK=%d.%d.%d.%d", pclass->Ser_ip.Mask[0], pclass->Ser_ip.Mask[1], pclass->Ser_ip.Mask[2], pclass->Ser_ip.Mask[3]);
        IP_printf("��������ʱ��:%dʱ%d��", pclass->alm_on.hour, pclass->alm_on.min);
        IP_printf("��������ʱ��:%dʱ%d��", pclass->alm_off.hour, pclass->alm_off.min);
        IP_printf("��ǰ���¼���ID:%d", pclass->CurrsScameraPage);
        IP_printf("һ�����������:%d��(0�ֶ�1�α�2��ʱ)", pclass->OpenType);
        IP_printf("�༭��IP=%d.%d.%d.%d", pclass->BJCardIP[0], pclass->BJCardIP[1], pclass->BJCardIP[2], pclass->BJCardIP[3]);
        IP_printf("���뿨1_ IP=%d.%d.%d.%d", pclass->BMCard1IP[0], pclass->BMCard1IP[1], pclass->BMCard1IP[2], pclass->BMCard1IP[3]);
        IP_printf("���뿨2_ IP=%d.%d.%d.%d", pclass->BMCard2IP[0], pclass->BMCard2IP[1], pclass->BMCard2IP[2], pclass->BMCard2IP[3]);
        IP_printf(" ��ý�� IP=%d.%d.%d.%d", pclass->LMT_IP[0], pclass->LMT_IP[1], pclass->LMT_IP[2], pclass->LMT_IP[3]);
        IP_printf(" ��ʦPC IP=%d.%d.%d.%d", pclass->PC_IP[0], pclass->PC_IP[1], pclass->PC_IP[2], pclass->PC_IP[3]);
        IP_printf("У԰һ��ͨ����:%d =0��Ч=1 ��Ч", pclass->Card_Contrl);
        IP_printf("����������:%d =0��Ч=1 ��Ч", pclass->YuYinCtrl);

    }
    /*
    else
    	if (page_id==1)
    	{pdata=(uint8 *)Get_ClassInfo_Table(1);
    	 pmsg=(struct BASE_MSG *)(pdata+2);
    	 IP_printf("���ػ�����");
    	 IP_printf("���ػ�����-������Ϣ1:%d,%d",pmsg->device,pmsg->option);
    	 pmsg=(struct BASE_MSG *)(pdata+4);
    	 IP_printf("���ػ�����-������Ϣ2:%d,%d",pmsg->device,pmsg->option);
    	 pmsg=(struct BASE_MSG *)(pdata+6);
    	 IP_printf("���ػ�����-������Ϣ3:%d,%d",pmsg->device,pmsg->option);
    	 for (tmp=0;tmp<100;tmp++)
    	 	{ pCLASS_OPtion=(struct CLASS_OPION_TYPE *)(pdata+8+tmp*4);
    	         if ((pmsg->device==0)||(pmsg->option==0)) break;
    	          IP_printf("���ػ�����-���ػ���Ϣ%d:%d,%d��ʱ:%d",tmp,pCLASS_OPtion->msg.device,pCLASS_OPtion->msg.option,pCLASS_OPtion->del_time);
    	 	}
    	}
    else
    	if ((page_id>=2)&&(page_id<=9))
    		{IP_printf("�α��������:%d",page_id);
    	         pclass_table=(struct CLASS_TABLE_TYPE *)User_mem_allco(255);
    		 if (pclass_table==NULL)
    		 	{IP_printf("�����ڴ�ʧ��!");
    		        return ;
    		 	}
    		do{
    		err = AT24C16_Read(pclass_table->Class_Table_Rec, CLASS_TABLE_ADDRESS+page_id*256, 256);
    		if (i++ > 5)
    			{User_mem_free(pclass_table);
    			IP_printf("���α�AT24C512�ڴ�ʧ��!");
    			return ;
    		        }
    		}while(err == FALSE);
    		for (i=0;i<8;i++)
    			{
    			pclass_Rec=(struct CLASS_TABLE_REC *)(pclass_table->Class_Table_Rec+3+i*sizeof(struct CLASS_TABLE_REC));
    			if ((pclass_Rec->class_begin.hour==0)||(pclass_Rec->class_begin.hour>24)) break;
    			IP_printf("��¼:%d ����ʱ��:%d:%d  �ػ�ʱ��:%d:%d",i,
    				pclass_Rec->class_begin.hour,pclass_Rec->class_begin.min,
    				pclass_Rec->class_over.hour,pclass_Rec->class_over.min);
    			Mem_Clr(buf,20);
    			Mem_Copy(buf,pclass_Rec->class_name,16);
    			IP_printf("�γ���:%s",buf);
    			Mem_Clr(buf,20);
    			Mem_Copy(buf,pclass_Rec->teacher_name,10);
    			IP_printf("��ʦ��:%s",buf);
    			}
    		}
    else
    	if(page_id==16)
    		{
    		IP_printf("��ӡ�豸������Ϣ");
    		for (i=MSG_DEV_MAINBARD; i<255; i++)
    		{
    			pdev = (struct DEVICE_INFO*)Get_ClassInfo_Table(i);
    			if (pdev==NULL) break;

    			IP_printf("�豸������Ϣ �豸ID:%d�豸����:%d",pdev->dev_id,pdev->dev_type);
    			Mem_Clr(buf,40);
    			Mem_Copy(buf,pdev->dev_name,32);
    			IP_printf("�豸����:%s",buf);
    			Mem_Clr(buf,40);
    			Mem_Copy(buf,pdev->dev_type_name,32);
    			IP_printf("�豸��������:%s",buf);
    			IP_printf("VGA����IN:%d,OUT:%dLINK:%d",
    				pdev->VGA.VGAIn,pdev->VGA.VGAOut,pdev->VGA.VGALink);
    			IP_printf("Video����IN:%d,OUT:%dLINK:%d",
    				pdev->VIDEO.VideoIn,pdev->VIDEO.VideoOut,pdev->VIDEO.VideoLink);
    			IP_printf("��Ƶ����IN:%d,OUT:%dLINK:%d����:%d",
    				pdev->SOUND.SoundIn,pdev->SOUND.SoundOut,pdev->SOUND.SoundLink,pdev->SOUND.SoundVol);

    			IP_printf("IO��������:�����:%d,����:%d��Ƶ������%d",
    				pdev->IN.in_no,pdev->IN.in_type,pdev->IN.in_sound);
    			IP_printf("IO�������:�����:%d,����:%d��Ƶ������%d",
    				pdev->OUT.out_no,pdev->OUT.out_type,pdev->OUT.io_sound);
    			IP_printf("��Դ����--��Դ�䣬%d�˿ں�:%d",pdev->POWER.dev_id,pdev->POWER.power_no);
    			IP_printf("ͨѶ����--%d",pdev->COMM.comm_type);

    		}
    		}*/
}

////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void PC_init(uint16 Power_no)
{
    Mem_Set((uint8 *)&gPC_power, 0, sizeof(struct LMT_CONTRL_TYPE));
    gPC_power.status |= LMT_CONTRL_Set;
    gPC_power.power_no = Power_no;
}
////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint8 PC_Power_contrl(uint8 Device, uint8 option)
{

    if ((option != MSG_OPTION_POWEROPEN) &&
            (option != MSG_OPTION_POWERCLOSE)) return FALSE;
    if (Device != MSG_DEV_LMT) return FALSE;
    gPC_power.Device = Device;
    gPC_power.option = option;
    gLMT_Power.reptimes = 0;
    return TRUE;
}
////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void PC_Power_Check(void)
{
    //struct BASE_MSG msg;
    //uint8 *ip;
    if ((gPC_power.status & LMT_CONTRL_Set) == 0) return;

    ///////��ý�忪��/////////////////////////////////////
    if ((gPC_power.option == MSG_OPTION_POWEROPEN) &&
            ((gPC_power.status & LMT_CONTRL_PWon) == 0))
    {
        gPC_power.status &= ~LMT_CONTRL_PWoff;
        if (IO_in(IN_PC1POWER) == FALSE) //��ý���Ѿ�����
        {
            gPC_power.status |= LMT_CONTRL_PWon;
            gPC_power.status &= ~LMT_CONTRL_EXIT;
            return;
        }
        else
        {
            if (gPC_power.option_time++ < 10) return;
            gPC_power.option_time = 0;
            POWER_Set(gPC_power.power_no, gPC_power.option);
            IO_PC_Power(MSG_DEV_PC, gPC_power.option);
        }
    }
    ///////��ý��ػ�////////////////////////////////////
    if ((gPC_power.option == MSG_OPTION_POWERCLOSE) &&
            ((gPC_power.status & LMT_CONTRL_PWoff) == 0))
    {
        gPC_power.status &= ~LMT_CONTRL_PWon;

        if (IO_in(IN_PC1POWER))
        {
            gPC_power.status |= LMT_CONTRL_PWoff;
            POWER_Set(gPC_power.power_no, gPC_power.option);
            gPC_power.status &= ~LMT_CONTRL_EXIT;
            return;
        }
        else
        {
            if (gPC_power.option_time++ < 60) return;
            gPC_power.option_time = 0;
            IO_PC_Power(MSG_DEV_PC, gPC_power.option);
        }
    }

}
////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void LMT_init(uint16 Power_no)
{
    Mem_Set((uint8 *)&gLMT_Power, 0, sizeof(struct LMT_CONTRL_TYPE));
    gLMT_Power.status |= LMT_CONTRL_Set;
    gLMT_Power.power_no = Power_no;
}
////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void LMT_Delay_Power(void)
{
    if (gLMT_Power.option != MSG_OPTION_POWERCLOSE) return;
    gLMT_Power.reptimes = 0;

}
////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
uint8 LMT_Power_contrl(uint8 Device, uint8 option)
{

    if ((option != MSG_OPTION_POWEROPEN) &&
            (option != MSG_OPTION_POWERCLOSE)) return FALSE;
    if (Device != MSG_DEV_LMT) return FALSE;
    gLMT_Power.Device = Device;
    gLMT_Power.option = option;
    gLMT_Power.reptimes = 0;
    return TRUE;
}
////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void LMT_Power_Check(void)
{
    struct BASE_MSG msg;
    struct DEVICE_STATUS_TYPE *ps;
    //struct DEVICE_INFO *pdev;
    uint8 *ip;
    if (Check_FunctionMode(Device_FunctionMode_LB) == FALSE)  return;
    if ((gLMT_Power.status & LMT_CONTRL_Set) == 0) return;

    ///////��ý�忪��/////////////////////////////////////
    if ((gLMT_Power.option == MSG_OPTION_POWEROPEN) &&
            ((gLMT_Power.status & LMT_CONTRL_PWon) == 0))
    {
        gLMT_Power.status &= ~LMT_CONTRL_PWoff;
        if (IO_in(IN_PC2POWER) == FALSE) //��ý���Ѿ�����
        {
            gLMT_Power.status |= LMT_CONTRL_PWon;
            gLMT_Power.status &= ~LMT_CONTRL_EXIT;
            return;
        }
        else
        {
            if (gLMT_Power.option_time++ < 10) return;
            gLMT_Power.option_time = 0;
            POWER_Set(gLMT_Power.power_no, gLMT_Power.option);
            IO_PC_Power(MSG_DEV_LMT, gLMT_Power.option);
        }
    }
    ///////��ý��ػ�////////////////////////////////////
    if ((gLMT_Power.option == MSG_OPTION_POWERCLOSE) &&
            ((gLMT_Power.status & LMT_CONTRL_PWoff) == 0))
    {
        gLMT_Power.status &= ~LMT_CONTRL_PWon;
        ps = (struct DEVICE_STATUS_TYPE *)Get_Device_Status();
        if((ps->Current_REC == LMT_STATUS_Exit) || (ps->Current_REC == LMT_STATUS_NONE))
            gLMT_Power.status |= LMT_CONTRL_EXIT;

        if (IO_in(IN_PC2POWER))
        {
            gLMT_Power.status |= LMT_CONTRL_PWoff;
            POWER_Set(gLMT_Power.power_no, gLMT_Power.option);
            gLMT_Power.status &= ~LMT_CONTRL_EXIT;
            return;
        }
        else
        {
            if (gLMT_Power.option_time++ < 60) return;
            gLMT_Power.option_time = 0;
            if (gLMT_Power.status & LMT_CONTRL_EXIT)
            {
                IO_PC_Power(MSG_DEV_LMT, gLMT_Power.option);
            }
            else
            {
                if (gLMT_Power.reptimes++ < 2)
                {
                    msg.device = MSG_DEV_LMT;
                    msg.option = MSG_OPTION_LMT_ReqStatus;
                    ip = Get_DesIp(DEVICE_SUB_BJCard);
                    if (ARP_Rec_Find(ip) == NULL)
                    {
                        Arp_Request(ip);
                        return ;
                    }
                    UDP_Send_CMD((uint8 *)&msg, 2, ip);
                }
                else
                {
                    //��ʱ60S,ǿ�йػ�
                    IP_printf("��ʱ60S,ǿ�йػ�");
                    gLMT_Power.status |= LMT_CONTRL_EXIT;
                    gLMT_Power.reptimes = 0;

                }
            }
        }
    }

}
////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void Run_Send_Flag(void)
{
    OSSemPost(FRun);

}

////////////////////////////////////////////////////////////////////
//	�������ƣ�
//	�������ܣ�
//	��ڲ�����
//
//
//	���ڲ�������
////////////////////////////////////////////////////////////////////
void TASK_RUN(void *pdata)
{
    uint8 err;//,*p;
    uint8 Scan_io_Time, ReadWD;
    uint32 flag = 0, tmp;
    FRun = OSSemCreate(0);
    Mem_Set((uint8 *)&gLMT_Power, 0, sizeof(struct LMT_CONTRL_TYPE));
    Scan_io_Time = 0;
    ReadWD = 0;
    IO_FlagLingt_Flash();

    //uint8 *ip,buf[16];
    //Mem_Set(buf,0,16);
    //ip=Get_DesIp(DEVICE_SUB_BJCard);
    // if (ARP_Rec_Find(ip)==NULL)
    //  {
    //    Arp_Request(ip);
    //  }
    //          buf[0]=MSG_DEV_MAINBARD;
    //	  buf[1]=MSG_OPTION_SCAMERA_BackVAL;
    //          buf[3]=1;

    Device_Rec_Scan();
    for (;;)
    {
        OSSemPend(FRun, 0, &err);
        if (err !=  OS_NO_ERR) continue;
        flag = 0;

        IS752_Scan();
        //LMT_Power_Check();
        Device_DCS_Contrl();
        RF_CARD_Check();
        
        if (ReadWD++ > 60)
        {
            ReadWD = 0;
            DW_Read_WD();
            //UDP_Send_CMD(buf, 16,ip);
            //TB_Read_Csingle(); ����ͨ���ź���Ч
            //TB_Read_state();ͨ���ź�����
        }
        if (Scan_io_Time++ > 3)
        {
            // Device_Link_Scan();
            //Device_Vol_Scan();
            if (0 == IO_Scan()) //IOɨ��
            {
                flag |= 0x00;
            }
            else
            {
                flag |= 0x01;
                Alarm_Link(); //��������
            }


            //״̬�ı�
            if  (flag & STATUS_CHANGE)
                Send_State();

        }



    }


}



