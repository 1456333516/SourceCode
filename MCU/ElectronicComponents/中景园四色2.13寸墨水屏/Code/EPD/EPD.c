#include "EPD.h"


/*******************************************************************
    函数说明:判忙函数
    入口参数:无
    说明:忙状态为1    
*******************************************************************/
void EPD_READBUSY(void)
{
  while(1)
  {
    if(EPD_ReadBusy==1)
    {
      break;
    }
  }
}

/*******************************************************************
    函数说明:硬件复位函数
    入口参数:无
    说明:在E-Paper进入Deepsleep状态后需要硬件复位  
*******************************************************************/
void EPD_HW_RESET(void)
{
  HAL_Delay(100);
  EPD_RES_Clr();
  HAL_Delay(10);
  EPD_RES_Set();
  HAL_Delay(10);
  EPD_READBUSY();
}

/*******************************************************************
    函数说明:更新函数
    入口参数:无  
    说明:更新显示内容到E-Paper    
*******************************************************************/
void EPD_Update(void)
{
  EPD_WR_REG(0x04);//Power ON
  EPD_READBUSY();

  EPD_WR_REG(0x12);//Dispaly Refresh
  EPD_WR_DATA8(0x00);
  EPD_READBUSY();
}

/*******************************************************************
    函数说明:休眠函数
    入口参数:无
    说明:屏幕进入低功耗模式    
*******************************************************************/
void EPD_DeepSleep(void)
{
  EPD_WR_REG(0x02);//Power OFF
  EPD_WR_DATA8(0x00);
  EPD_READBUSY();

  EPD_WR_REG(0x07);//Deep Sleep
  EPD_WR_DATA8(0xa5);
}

/*******************************************************************
    函数说明:初始化函数
    入口参数:无
    说明:调整E-Paper默认显示方向
*******************************************************************/
void EPD_Init(void)
{
  EPD_HW_RESET();
  
  EPD_WR_REG(0x4D);
  EPD_WR_DATA8(0x78);

  EPD_WR_REG(0x00);
  EPD_WR_DATA8(0x0F);
  EPD_WR_DATA8(0x09);
  
  EPD_WR_REG(0x01);
  EPD_WR_DATA8(0x07);
  EPD_WR_DATA8(0x00);
  EPD_WR_DATA8(0x22);
  EPD_WR_DATA8(0x78);
  EPD_WR_DATA8(0x0A);
  EPD_WR_DATA8(0x22);

  
  EPD_WR_REG(0x03);
  EPD_WR_DATA8(0x10);
  EPD_WR_DATA8(0x54);
  EPD_WR_DATA8(0x44);
  
  EPD_WR_REG(0x06);
  EPD_WR_DATA8(0x0F);
  EPD_WR_DATA8(0x0A);
  EPD_WR_DATA8(0x2F);
  EPD_WR_DATA8(0x25);
  EPD_WR_DATA8(0x22);
  EPD_WR_DATA8(0x2E);
  EPD_WR_DATA8(0x21);

  EPD_WR_REG(0x30);//frame rate from OTP
  EPD_WR_DATA8(0x02);
  
  EPD_WR_REG(0x41);
  EPD_WR_DATA8(0x00);

  EPD_WR_REG(0x50);
  EPD_WR_DATA8(0x37);
  
  EPD_WR_REG(0x60);
  EPD_WR_DATA8(0x02);
  EPD_WR_DATA8(0x02);
  
  EPD_WR_REG(0x61);
  EPD_WR_DATA8(Source_Bits/256);
  EPD_WR_DATA8(Source_Bits%256);
  EPD_WR_DATA8(Gate_Bits/256);
  EPD_WR_DATA8(Gate_Bits%256);

  EPD_WR_REG(0x65);
  EPD_WR_DATA8(0x00);
  EPD_WR_DATA8(0x00);
  EPD_WR_DATA8(0x00);
  EPD_WR_DATA8(0x00);

  EPD_WR_REG(0xE7);
  EPD_WR_DATA8(0x1C);
  
  EPD_WR_REG(0xE3);
  EPD_WR_DATA8(0x22);
  
  EPD_WR_REG(0xE0);
  EPD_WR_DATA8(0x00);
  
  EPD_WR_REG(0xB4);
  EPD_WR_DATA8(0xD0);
  EPD_WR_REG(0xB5);
  EPD_WR_DATA8(0x03);
  
  EPD_WR_REG(0xE9);
  EPD_WR_DATA8(0x01);

}
/*******************************************************************
    函数说明:填充函数 
    入口参数:无
    说明:E-Paper填充对应颜色
*******************************************************************/
void EPD_Display_Fill(uint8_t dat)
{
  uint16_t i;
  EPD_WR_REG(0x10);
  switch(dat)
  {
    case 0x00:
      dat=0x00;//BLACK  
    break;
    case 0x01:
      dat=0x55;//WHITE
    break;
    case 0x02:
      dat=0xAA;//YELLOW
    break;
    case 0x03:
      dat=0xFF;//RED
    break;
    default:
    break;
  }  
  for(i=0;i<8000;i++)
  {
    EPD_WR_DATA8(dat);
  }  
}
//颜色转换函数 
uint8_t Color_Conversion(uint8_t color)
{
  unsigned datas;
  switch(color)
  {
    case 0x00:
      datas=BLACK;  
      break;    
    case 0x01:
      datas=WHITE; 
      break;
    case 0x02:
      datas=YELLOW; 
      break;    
    case 0x03:
      datas=RED; 
      break;      
    default:
      break;      
  }
   return datas;
}
/*******************************************************************
    函数说明:数组数据更新到E-Paper
    入口参数:无
    说明:
*******************************************************************/
void EPD_Display(const uint8_t *image)
{
  uint8_t data_H1,data_H2,data_L1,data_L2,data,temp;
  uint16_t i,j,Width,Height;
  Width=(Source_Bits%4==0)?(Source_Bits/4):(Source_Bits/4+1);
  Height=EPD_H;
  EPD_WR_REG(0x10);
  for (j=0;j<Height;j++) 
  {
    for (i=0;i<Width;i++) 
    {
      temp=image[i+j*Width];
      data_H1=Color_Conversion(temp>>6&0x03)<<6;      
      data_H2=Color_Conversion(temp>>4&0x03)<<4;
      data_L1=Color_Conversion(temp>>2&0x03)<<2;
      data_L2=Color_Conversion(temp&0x03);
      data=data_H1|data_H2|data_L1|data_L2;
      EPD_WR_DATA8(data);
    }
  }
}



