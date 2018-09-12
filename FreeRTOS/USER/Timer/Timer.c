#include "timer.h"
#include "led.h"

/*********************************************************************************
**********************************************************************************
* 文件名称: timer.c                                                              *
* 文件简述：timer定时                                                            *
* 创建日期：2018.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                       																						   * 
**********************************************************************************
*********************************************************************************/ 


/****************************************************************************
* 名    称: TIM2_Init(u16 auto_data,u16 fractional)
* 功    能：定时器2初始化
* 入口参数：auto_data: 自动重装值
*           fractional: 时钟预分频数
* 返回参数：无
* 说    明：定时器溢出时间计算方法:Tout=((auto_data+1)*(fractional+1))/Ft(us)  Ft定时器时钟
****************************************************************************/
void Tim2_Init(u16 auto_data,u16 fractional)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);      //使能TIM2时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = auto_data; 	     //自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=fractional;      //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器2更新中断
	TIM_Cmd(TIM2,ENABLE);                    //使能定时器2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;  //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM2, ENABLE);  									//使能TIM5	
}

//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
		CtrLed(5);
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}

/****************************************************************************
* 名    称: TIM5_Init(u16 auto_data,u16 fractional)
* 功    能：定时器2初始化
* 入口参数：auto_data: 自动重装值
*           fractional: 时钟预分频数
* 返回参数：无
* 说    明：定时器溢出时间计算方法:Tout=((auto_data+1)*(fractional+1))/Ft(us)  Ft定时器时钟
****************************************************************************/
void Tim5_Init(u16 arr,u16 psc)
{	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; 					           //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 					         //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	  //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 			      //根据指定的参数初始化TIMx的时间基数单位
 
	//TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); 					      //使能指定的TIM5中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  			      //TIM5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  	//先占优先级5级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  		    //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			      //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  							               //初始化NVIC寄存器

	//TIM_Cmd(TIM5, ENABLE);  									                //使能TIM5		
}
/*******************************************************************************
* 函数名  : Timer5_Start
* 描述    : Timer5开启并归零
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void Timer5_Start(void)
{
	TIM_SetCounter(TIM5, 0);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE ); 				//使能TIM2指定的中断
	TIM_Cmd(TIM5, ENABLE);															//开启定时中断
}
/*******************************************************************************
* 函数名  : Timer5_Stop
* 描述    : Timer5开启并归零
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void Timer5_Stop(void )
{
	TIM_Cmd(TIM5, DISABLE);//禁止TIM5 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , DISABLE);//关闭TIM5时钟
}
/*******************************************************************************
* 函数名  : TIM5_IRQHandler
* 描述    : 定时器5中断断服务函数
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM5更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIM5更新中断标志
    Timer5_Stop();
//		moblie_data_filter();		
	}
}



