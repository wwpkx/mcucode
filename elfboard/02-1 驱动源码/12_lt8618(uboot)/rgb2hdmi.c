/*
 * Copyright (C) 2020 Forlinx
 * bk iovt@outlook.com
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <linux/types.h>
#include <i2c.h>
#include <linux/compiler.h>
#include <rgb2hdmi.h>

#include <asm/arch/iomux.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/crm_regs.h>
#include <asm/arch/mx6-pins.h>
#include <asm/arch/sys_proto.h>
#include <asm/gpio.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/imx-common/boot_mode.h>
#include <asm/imx-common/mxc_i2c.h>

u8 HDMI_VIC=0x00;
u8 HDMI_Y=0x00;
enum {
    _16bit = 1,
    _20bit,
    _24_32bit
};
enum {
    _32KHz = 0,
    _44d1KHz,
    _48KHz,

    _88d2KHz,
    _96KHz,
    _176Khz,
    _196KHz
};
u16 Sample_Freq[] =
{
    0x30,   // 32K
    0x00,   // 44.1K
    0x20,   // 48K
    0x80,   // 88.2K
    0xa0,   // 96K
    0xc0,   // 176K
    0xe0    //  196K
};
u16 IIS_N[] =
{
    4096,   // 32K
    6272,   // 44.1K
    6144,   // 48K
    12544,  // 88.2K
    12288,  // 96K
    25088,  // 176K
    24576   // 196K
};

#define Sampling_rate   _48KHz
#define Sampling_Size   _24_32bit

u16 hfp,hs_width,hbp,h_act,h_tal,vfp,vs_width,vbp,v_act,v_tal;
// hfp, hs, hbp,hact,htotal,vfp, vs, vbp,vact,vtotal,
const struct video_timing video_800x600_60Hz  = {40, 128,  88, 800,   1056,  1,  4,  23, 600,   628};
const struct video_timing video_1280x720_60Hz = {110, 40, 220,1280,  1650,  5,  5,  20, 720,   750};
const struct video_timing video_1280x720_30Hz = {736, 40, 1244,1280,  3300, 5,  5,  20, 720,   750};

#define USE_DDRCLK  0


static u8 lt8618_reg_read(u8 reg)
{
    u8 buf = 0;
    I2C_SET_BUS(CONFIG_LT8618_BUS-1);
    if (i2c_read(CONFIG_LT8618_I2C_ADDR, reg, 1, &buf, 1))
        return -1;
    return buf;
}
static void lt8618_reg_write(u8 reg,u8 val)
{
    u8 buf = 0;
	buf = val;
    I2C_SET_BUS(CONFIG_LT8618_BUS-1);
    i2c_write(CONFIG_LT8618_I2C_ADDR, reg, 1, &buf , 1);
}

static u8 lt8618sx_video_check(void)
{
	u8 temp;
	mdelay(100);
	hfp = 0;
	hs_width = 0;
	hbp = 0;
	h_act = 0;
	h_tal = 0;
	vfp = 0;
	vs_width = 0;
	vbp = 0;
	v_act = 0;
	v_tal = 0;

	lt8618_reg_write( 0xff, 0x80 );
	lt8618_reg_write( 0x13, 0xf1 );//ttl video process reset
	lt8618_reg_write( 0x12, 0xfb );//video check reset
	mdelay(1);
	lt8618_reg_write( 0x12, 0xff );
	lt8618_reg_write( 0x13, 0xf9 );
	mdelay(100);
	lt8618_reg_write(0xff,0x82);
	lt8618_reg_write( 0x51, 0x00 );

	lt8618_reg_write(0xff,0x82); //video check
	 temp=lt8618_reg_read(0x70);  //hs vs polarity
//	 if(temp & 0x02)
	// {
//		printf("lt8618sx vs_pol is 1.\n");
	// }
	// else
	// {
	//	printk("lt8618sx vs_pol is 0.\n");
	// }
//	 if( temp & 0x01 )
	// {
//		printf("lt8618sx hs_pol is 1.\n");
	// }
	// else
	// {
	//	printk("lt8618sx hs_pol is 0.\n");
	// } 
	vs_width = lt8618_reg_read(0x71);
	hs_width = lt8618_reg_read(0x72);
	hs_width = ( (hs_width & 0x0f) << 8 ) + lt8618_reg_read(0x73);
	vbp = lt8618_reg_read(0x74);
	vfp = lt8618_reg_read(0x75);
	hbp = lt8618_reg_read(0x76);
	hbp = ( (hbp & 0x0f) << 8 ) + lt8618_reg_read(0x77);
	hfp = lt8618_reg_read(0x78);
	hfp = ( (hfp & 0x0f) << 8 ) + lt8618_reg_read(0x79);
	v_tal = lt8618_reg_read(0x7a);
	v_tal = ( v_tal << 8 ) + lt8618_reg_read(0x7b);
	h_tal = lt8618_reg_read(0x7c);
	h_tal = ( h_tal << 8 ) + lt8618_reg_read(0x7d);
	v_act = lt8618_reg_read(0x7e);
	v_act = ( v_act << 8 ) + lt8618_reg_read(0x7f);
	h_act = lt8618_reg_read(0x80);
	h_act = ( h_act << 8 ) + lt8618_reg_read(0x81);
   // printf("lt8618 init: hfp=%d,hs_width=%d,hbp=%d,h_act=%d,h_tal=%d\n",hfp,hs_width,hbp,h_act,h_tal);
   // printf("lt8618 init: vfp=%d,vs_width=%d,vbp=%d,v_act=%d,v_tal=%d\n",vfp,vs_width,vbp,v_act,v_tal);
	// printk("lt8618sx debug -----------------\n");
	// printk("lt8618sx hfp = %d.\n",hfp);
	// printk("lt8618sx hs_width = %d.\n",hs_width);
	// printk("lt8618sx hbp = %d.\n",hbp);
	// printk("lt8618sx h_act = %d.\n",h_act);
	// printk("lt8618sx h_tal = %d.\n",h_tal);

	// printk("lt8618sx vfp = %d.\n",vfp);
	// printk("lt8618sx vs_width = %d.\n",vs_width);
	// printk("lt8618sx vbp = %d.\n",vbp);
	// printk("lt8618sx v_act = %d.\n",v_act);
	// printk("lt8618sx v_tal = %d.\n",v_tal);
	// printk("lt8618sx debug -----------------\n");

}

static u8 lt8618_version_u2(void)
{
	u8 read_val;
	u8 j;
	u8 cali_done;
	u8 cali_val;
	u8 lock;
	u32 dclk;

	/* read dclk */
	lt8618_reg_write( 0xff, 0x82);
	lt8618_reg_write( 0x17, 0x80);
	mdelay(500);
	dclk=(((lt8618_reg_read(0x1d))&0x0f)<<8)+lt8618_reg_read(0x1e);
	dclk=(dclk<<8)+lt8618_reg_read(0x1f);
	// printk("lt8618sx ad ttl dclk = %d.\n",dclk);
//	printf("lt8618 init: dclk=%d.\n",dclk);
	/* config pll */
	lt8618_reg_write( 0xff, 0x81);
	lt8618_reg_write( 0x23, 0x40 );
	lt8618_reg_write( 0x24, 0x62 );               //icp set
	lt8618_reg_write( 0x26, 0x55 );

	if(dclk<=25000)
	{
		lt8618_reg_write( 0x25, 0x00 );
		//lt8618_reg_write( 0x2c, 0xA8 );
		lt8618_reg_write( 0x2c, 0x94 );
		lt8618_reg_write( 0x2d, 0xaa );
	}
	else if((dclk>25000)&&(dclk<=50000))
	{
		lt8618_reg_write( 0x25, 0x00 );
		lt8618_reg_write( 0x2d, 0xaa );
		lt8618_reg_write( 0x2c, 0x94 );
	}
	else if((dclk>50000)&&(dclk<=100000))
	{
		lt8618_reg_write( 0x25, 0x01 );
		lt8618_reg_write( 0x2d, 0x99 );
		lt8618_reg_write( 0x2c, 0x94 );
		//printk("lt8618sx 50~100m.\n");
	}
	else //if(dclk>100000)
	{
		lt8618_reg_write( 0x25, 0x03 );
		lt8618_reg_write( 0x2d, 0x88 );
		lt8618_reg_write( 0x2c, 0x94 );
	}

	if( USE_DDRCLK )
	{
		read_val = lt8618_reg_read(0x2c) &0x7f;
		read_val=read_val*2|0x80;
		lt8618_reg_write( 0x2c, read_val );
		//printk("lt8618sx 0x812c = %x.\n",read_val);
		lt8618_reg_write( 0x4d, 0x05 );
		lt8618_reg_write( 0x27, 0x66 );//0x60 //ddr 0x66
		lt8618_reg_write( 0x28, 0x88 );
		printf("lt8618 init: PLL DDR.\n");			 
	}
	else
	{
		lt8618_reg_write( 0x4d, 0x01 );
		lt8618_reg_write( 0x27, 0x66 ); //0x06//0x60 //ddr 0x66
		//printk("lt8618sx 0x8127 = %x.\n",lt8618_reg_read(0x27));
		lt8618_reg_write( 0x28, 0x88 ); // 0x88
		//printf("lt8618 init: PLL SDR.\n");
	}
	/* add */
	/* as long as changing the resolution or changing the input clock,	You need to configure the following registers. */
	lt8618_reg_write( 0xff, 0x81 );
	read_val=lt8618_reg_read(0x2b);
	lt8618_reg_write( 0x2b, read_val&0xfd );// sw_en_txpll_cal_en
	read_val=lt8618_reg_read(0x2e);
	lt8618_reg_write( 0x2e, read_val&0xfe );//sw_en_txpll_iband_set
	lt8618_reg_write( 0xff, 0x82 );
	lt8618_reg_write( 0xde, 0x00 );
	lt8618_reg_write( 0xde, 0xc0 );		
	lt8618_reg_write( 0xff, 0x80 );
	lt8618_reg_write( 0x16, 0xf1 );
	lt8618_reg_write( 0x18, 0xdc );//txpll _sw_rst_n
	lt8618_reg_write( 0x18, 0xfc );
	lt8618_reg_write( 0x16, 0xf3 );	
	for(j=0;j<0x05;j++)
	{
		mdelay(10);
		lt8618_reg_write(0xff,0x80);	
		lt8618_reg_write(0x16,0xe3); /* pll lock logic reset */
		lt8618_reg_write(0x16,0xf3);
			
		lt8618_reg_write( 0xff, 0x82 );
		lock=0x80&lt8618_reg_read(0x15);	
		cali_val=lt8618_reg_read(0xea);	
		cali_done=0x80&lt8618_reg_read(0xeb);
		if(lock&&cali_done&&(cali_val!=0xff))
		{	
#if 1
			lt8618_reg_write( 0xff, 0x82 );
			// printk("lt8618sx 0x82ea = %x\n",lt8618_reg_read(0xea));	
			// printk("lt8618sx 0x82eb = %x\n",lt8618_reg_read(0xeb));	
			// printk("lt8618sx 0x82ec = %x\n",lt8618_reg_read(0xec));	
			// printk("lt8618sx 0x82ed = %x\n",lt8618_reg_read(0xed));	
			// printk("lt8618sx 0x82ee = %x\n",lt8618_reg_read(0xee));	
			// printk("lt8618sx 0x82ef = %x\n",lt8618_reg_read(0xef));	
#endif										
			// printk("lt8618sx TXPLL Lock\n");
			return 1;
		}
		else
		{
			lt8618_reg_write( 0xff, 0x80 );
			lt8618_reg_write( 0x16, 0xf1 );
			lt8618_reg_write( 0x18, 0xdc );//txpll _sw_rst_n
			lt8618_reg_write( 0x18, 0xfc );
			lt8618_reg_write( 0x16, 0xf3 );
			printf("lt8618sx TXPLL Reset\n");
		}
	}
	printf("lt8618sx TXPLL Unlock\n");

	return 0;
}


/* chip update */
static u8 lt8618_version_u3(void)
{
	u8 read_val;
	u8 j;
	u8 cali_done;
	u8 cali_val;
	u8 lock;
	u32 dclk;

	/* read dclk */
	lt8618_reg_write( 0xff, 0x82);
	lt8618_reg_write( 0x17, 0x80);
	mdelay(500);
	dclk=(((lt8618_reg_read(0x1d))&0x0f)<<8)+lt8618_reg_read(0x1e);
	dclk=(dclk<<8)+lt8618_reg_read(0x1f);
	// printk("lt8618sx ad ttl dclk = %d.\n",dclk);
//	printf("lt8618 init: dclk=%d.\n",dclk);
	/* config pll */
	lt8618_reg_write( 0xff, 0x81);
	lt8618_reg_write( 0x23, 0x40 );
	lt8618_reg_write( 0x24, 0x64 );               //icp set
	//lt8618_reg_write( 0x26, 0x55 );
	lt8618_reg_write( 0x25, 0x00 );
	lt8618_reg_write( 0x2c, 0x9e );

	if(dclk<=50000)
	{
		lt8618_reg_write( 0x2d, 0xaa );
	}
	else if((dclk>50000)&&(dclk<=100000))
	{
		lt8618_reg_write( 0x2d, 0x99 );
	}
	else //if(dclk>100000)
	{
		lt8618_reg_write( 0x2d, 0x88 );
	}

	lt8618_reg_write( 0x26, 0x55 );
	lt8618_reg_write( 0x27, 0x66 );
	lt8618_reg_write( 0x28, 0x88 );
	lt8618_reg_write( 0x29, 0x04 );

	/* add */
	/* as long as changing the resolution or changing the input clock,	You need to configure the following registers. */
	lt8618_reg_write( 0xff, 0x81 );
	read_val=lt8618_reg_read(0x2b);
	lt8618_reg_write( 0x2b, read_val&0xfd );// sw_en_txpll_cal_en
	read_val=lt8618_reg_read(0x2e);
	lt8618_reg_write( 0x2e, read_val&0xfe );//sw_en_txpll_iband_set
	lt8618_reg_write( 0xff, 0x82 );
	lt8618_reg_write( 0xde, 0x00 );
	lt8618_reg_write( 0xde, 0xc0 );		
	lt8618_reg_write( 0xff, 0x80 );
	lt8618_reg_write( 0x16, 0xf1 );
	lt8618_reg_write( 0x18, 0xdc );//txpll _sw_rst_n
	lt8618_reg_write( 0x18, 0xfc );
	lt8618_reg_write( 0x16, 0xf3 );	

	if( USE_DDRCLK )
	{
		read_val = lt8618_reg_read(0x2c) &0x7f;
		read_val=read_val*2|0x80;
		lt8618_reg_write( 0x2c, read_val );
		//printk("lt8618sx 0x812c = %x.\n",read_val);
		lt8618_reg_write( 0x4d, 0x05 );
		lt8618_reg_write( 0x27, 0x66 );//0x60 //ddr 0x66
		lt8618_reg_write( 0x28, 0x88 );
		printf("lt8618 init: PLL DDR.\n");			 
	}
	else
	{
		lt8618_reg_write( 0xff, 0x81);
		lt8618_reg_write( 0x4d, 0x01 );
		lt8618_reg_write( 0x27, 0x66 ); //0x06//0x60 //ddr 0x66
		lt8618_reg_write( 0x2a, 0x00 );
		lt8618_reg_write( 0x2a, 0x20 );
		//printk("lt8618sx 0x8127 = %x.\n",lt8618_reg_read(0x27));
		//lt8618_reg_write( 0x28, 0x88 ); // 0x88
		//printf("lt8618 init: PLL SDR.\n");
	}

	for(j=0;j<0x05;j++)
	{
		mdelay(10);
		lt8618_reg_write(0xff,0x80);	
		lt8618_reg_write(0x16,0xe3); /* pll lock logic reset */
		lt8618_reg_write(0x16,0xf3);
			
		lt8618_reg_write( 0xff, 0x82 );
		lock=0x80&lt8618_reg_read(0x15);	
		cali_val=lt8618_reg_read(0xea);	
		cali_done=0x80&lt8618_reg_read(0xeb);
		if(lock&&cali_done&&(cali_val!=0xff))
		{	
#if 1
			lt8618_reg_write( 0xff, 0x82 );
			// printk("lt8618sx 0x82ea = %x\n",lt8618_reg_read(0xea));	
			// printk("lt8618sx 0x82eb = %x\n",lt8618_reg_read(0xeb));	
			// printk("lt8618sx 0x82ec = %x\n",lt8618_reg_read(0xec));	
			// printk("lt8618sx 0x82ed = %x\n",lt8618_reg_read(0xed));	
			// printk("lt8618sx 0x82ee = %x\n",lt8618_reg_read(0xee));	
			// printk("lt8618sx 0x82ef = %x\n",lt8618_reg_read(0xef));	
#endif										
			// printk("lt8618sx TXPLL Lock\n");
			return 1;
		}
		else
		{
			lt8618_reg_write( 0xff, 0x80 );
			lt8618_reg_write( 0x16, 0xf1 );
			lt8618_reg_write( 0x18, 0xdc );//txpll _sw_rst_n
			lt8618_reg_write( 0x18, 0xfc );
			lt8618_reg_write( 0x16, 0xf3 );
			printf("lt8618sx TXPLL Reset\n");
		}
	}
	printf("lt8618sx TXPLL Unlock\n");

	return 0;
}

void lt8618_init(void)
{
	lt8618_probe();
	/* 1. read chip id */
	u8 id[3];
	lt8618_reg_write( 0xff, 0x80); //每次i2c通信要向80ee 中写入01特殊命令
	lt8618_reg_write( 0xee, 0x01);
	id[0]=lt8618_reg_read(0x00);
	id[1]=lt8618_reg_read(0x01);
	id[2]=lt8618_reg_read(0x02);
	printf("lt8618 init: id=%x %x %x.\n",id[0],id[1],id[2]);

	/* 2. config reset */
	lt8618_reg_write( 0xff, 0x80);
	lt8618_reg_write( 0x11, 0x00);
	lt8618_reg_write( 0x13, 0xf1);
	lt8618_reg_write( 0x13, 0xf9);

	/* 3. config TTL mode input analog */
	lt8618_reg_write( 0xff, 0x81);
	lt8618_reg_write( 0x02, 0x66);
	lt8618_reg_write( 0x0a, 0x06);
	lt8618_reg_write( 0x15, 0x06);	
	lt8618_reg_write( 0x4e, 0x00);	
	lt8618_reg_write( 0xff,0x82);// ring
	lt8618_reg_write( 0x1b,0x77);
	lt8618_reg_write( 0x1c,0xec); 

	/* 4. config TTL mode input digtal RGB888 */
	//printk("lt8618sx input mode = RGB888.\n");
	lt8618_reg_write( 0xff,0x82);
	lt8618_reg_write( 0x45,0x70); //RGB channel swap
	if(USE_DDRCLK == 1)
	{
		lt8618_reg_write( 0x4f,0x80); //0x80;  0xc0: invert dclk 
	}
	else
	{
		lt8618_reg_write( 0x4f,0x00); //0x00;  0x40: invert dclk fix:00-c0
	}
	lt8618_reg_write( 0x50,0x00);
	lt8618_reg_write( 0x51,0x00);

	/* 5. config PLL */
	if(id[2]==0xe1)
	{
	 lt8618_version_u2();
	 //printk("lt8618sx chip u2c.\n");
	}
	else if (id[2]==0xe2)
	{
	lt8618_version_u3();
	}

	/* 5+. config audio */
	// IIS Input
	lt8618_reg_write( 0xff, 0x82 );   // register bank
	lt8618_reg_write( 0xd6, Tx_Out_Mode|0x0e );   // bit7 = 0 : DVI output; bit7 = 1: HDMI output

	lt8618_reg_write( 0xd7, 0x04 );   

	lt8618_reg_write( 0xff, 0x84 );   // register bank
	lt8618_reg_write( 0x06, 0x08 );
	lt8618_reg_write( 0x07, 0x10 );
	lt8618_reg_write( 0x10, 0x15 );
	lt8618_reg_write( 0x12, 0x60 );
	lt8618_reg_write( 0x0f, 0x0b + Sample_Freq[_48KHz] );
	lt8618_reg_write( 0x34, 0xd4 );   //CTS_N / 2; 32bit(24bit)
	//	lt8618_reg_write( 0x34, 0xd5 );	//CTS_N / 4; 16bit
	lt8618_reg_write( 0x35, (u8)( IIS_N[_32KHz] / 0x10000 ) );
	lt8618_reg_write( 0x36, (u8)( ( IIS_N[_32KHz] & 0x00FFFF ) / 0x100 ) );
	lt8618_reg_write( 0x37, (u8)( IIS_N[_32KHz] & 0x0000FF ) );
	lt8618_reg_write( 0x3c, 0x21 );   // Null packet enable

	lt8618sx_video_check();
	/* 6. config CSC */
	HDMI_Y=0;
	lt8618_reg_write( 0xff,0x82);	
	lt8618_reg_write( 0xb9,0x00); //No csc
	/* 7. config hdmi tx digital */
	//AVI
	u8	AVI_PB0	   = 0x00;
	u8	AVI_PB1	   = 0x00;
	u8	AVI_PB2	   = 0x00;
	AVI_PB1 = ( HDMI_Y << 5 ) + 0x10;
	AVI_PB2 = 0x2A;  // 16:9  //	AVI_PB2 = 0x19;// 4:3
	AVI_PB0 = ( ( AVI_PB1 + AVI_PB2 + HDMI_VIC ) <= 0x6f ) ? ( 0x6f - AVI_PB1 - AVI_PB2 - HDMI_VIC ) : ( 0x16f - AVI_PB1 - AVI_PB2 - HDMI_VIC );
	lt8618_reg_write( 0xff, 0x84 );
	lt8618_reg_write( 0x43, AVI_PB0 );    //AVI_PB0
	lt8618_reg_write( 0x44, AVI_PB1 );    //AVI_PB1
	lt8618_reg_write( 0x45, AVI_PB2 );    //AVI_PB2
	lt8618_reg_write( 0x47, HDMI_VIC );   //AVI_PB4
	lt8618_reg_write( 0x10, 0x32 );       //data iland
	lt8618_reg_write( 0x12, 0x64 );       //act_h_blank
	//VS_IF, 4k 30hz need send VS_IF packet. Please refer to hdmi1.4 spec 8.2.3
	if( HDMI_VIC == 95 )
	{
		//LT8618SXB_I2C_Write_Byte(0xff,0x84);
		lt8618_reg_write( 0x3d, 0x2a );   //UD1 infoframe enable
		lt8618_reg_write( 0x74, 0x81 );
		lt8618_reg_write( 0x75, 0x01 );
		lt8618_reg_write( 0x76, 0x05 );
		lt8618_reg_write( 0x77, 0x49 );
		lt8618_reg_write( 0x78, 0x03 );
		lt8618_reg_write( 0x79, 0x0c );
		lt8618_reg_write( 0x7a, 0x00 );
		lt8618_reg_write( 0x7b, 0x20 );
		lt8618_reg_write( 0x7c, 0x01 );
	}else
	{
		//LT8618SXB_I2C_Write_Byte(0xff,0x84);
		lt8618_reg_write( 0x3d, 0x0a ); //UD1 infoframe disable
	}
	//AVI_audio
	lt8618_reg_write( 0xff, 0x84 );
	lt8618_reg_write( 0xb2, 0x84 );
	lt8618_reg_write( 0xb3, 0x01 );
	lt8618_reg_write( 0xb4, 0x0a );
	lt8618_reg_write( 0xb5, 0x60 - ( ( ( Sampling_rate + 1 ) << 2 ) + Sampling_Size ) );  //checksum
	lt8618_reg_write( 0xb6, 0x11 );                                                       //AVI_PB0//LPCM
	lt8618_reg_write( 0xb7, ( ( Sampling_rate + 1 ) << 2 ) + Sampling_Size );             //AVI_PB1//32KHz 24bit(32bit)

	/* 8. config hdmi tx phy */
	lt8618_reg_write(0xff,0x81);
	lt8618_reg_write(0x30,0xff);//0xea
	lt8618_reg_write(0x31,0x44);//DC: 0x44, AC:0x73
	lt8618_reg_write(0x32,0x4a);
	lt8618_reg_write(0x33,0x0b);
	lt8618_reg_write(0x34,0x00);//d0 pre emphasis
	lt8618_reg_write(0x35,0x00);//d1 pre emphasis
	lt8618_reg_write(0x36,0x00);//d2 pre emphasis
	lt8618_reg_write(0x37,0x44);
	lt8618_reg_write(0x3f,0x0f);
	lt8618_reg_write(0x40,0x80);//clk swing
	lt8618_reg_write(0x41,0x80);//d0 swing
	lt8618_reg_write(0x42,0x80);//d1 swing
	lt8618_reg_write(0x43,0x80); //d2 swing
	lt8618_reg_write(0x44,0x0a);

	/* 9. enable */ 
	lt8618_reg_write(0xff,0x81);
	lt8618_reg_write(0x30,0xea);
	
	lt8618_reg_write(0xff,0x82);
	// printk("lt8618sx 0x8215 = %x.\n",lt8618_reg_read(0x15));
	// printk("lt8618sx 0x824f = %x.\n",lt8618_reg_read(0x4f));
	// printk("lt8618sx 0x82b9 = %x.\n",lt8618_reg_read(0xb9));
	//printf("lt8618sx enable output.\n");
}

#define I2C_PAD_CTRL    (PAD_CTL_PKE | PAD_CTL_PUE |            \
	PAD_CTL_PUS_100K_UP | PAD_CTL_SPEED_MED |               \
	PAD_CTL_DSE_40ohm | PAD_CTL_HYS |			\
	PAD_CTL_ODE)
#define PC MUX_PAD_CTRL(I2C_PAD_CTRL)
static struct i2c_pads_info i2c_pad_info2 = {
       .scl = {
               .i2c_mode =  MX6_PAD_UART5_TX_DATA__I2C2_SCL | PC,
               .gpio_mode = MX6_PAD_UART5_TX_DATA__GPIO1_IO30 | PC,
               .gp = IMX_GPIO_NR(1, 30),
       },
       .sda = {
               .i2c_mode = MX6_PAD_UART5_RX_DATA__I2C2_SDA | PC,
               .gpio_mode = MX6_PAD_UART5_RX_DATA__GPIO1_IO31 | PC,
               .gp = IMX_GPIO_NR(1, 31),
       },
};
int lt8618_probe(void)
{

	setup_i2c(1, CONFIG_SYS_I2C_SPEED, 0x7f, &i2c_pad_info2);
    i2c_set_bus_num(CONFIG_LT8618_BUS-1);
   if (i2c_probe(CONFIG_LT8618_I2C_ADDR)) {
        printf("Can't find lt8618! \n");
        return -1;
    }
    return 0;
}

