#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>

#include <stdlib.h>
#include <stddef.h>
#include <math.h> 
#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include "imgui.h"
#include <time.h>
#define EDGE 0.015    //在这里定义像素点的边长 
#define PI 3.14159265358
extern double mediciner ;
/*
*	函数名：DrawDot 
*
*	用法：DrawDot(red,green,blue,x,y,size,n)
*
*	功能：在(x,y)处画一个边长为size的正方形。
* 
*	备注：当size很小时，视为一个点。它的颜色是(R,G,B)。 
*		  
*/ 
void DrawDot(double red,double green,double blue,double x,double y,double size)
{
	static int flag=0;
	switch (flag){
		case 0:{
			flag++;
			break;
		}
		case 1:{
			flag++;
			break;
		}
		case 2:{
			flag=0;
			break;
		}
	}
	static char *a[]={"a","b","c"};
	DefineColor(a[flag],red,green,blue);
	SetPenColor(a[flag]);
	

	
	StartFilledRegion(1);
	MovePen(x,y);
	DrawLine(size,0);
	DrawLine(0,size);
	DrawLine(-size,0);
	DrawLine(0,-size);
	EndFilledRegion();
	
}
/*	函数名：DrawPic
*
*	用法：DrawPic(name,x,y)
*
*	功能：以(x,y)为左上角画一个图片，这个图片的名字为name
* 
*	备注： 
*
*/ 
void DrawPic(char a[],double x,double y){
	
	freopen(a,"r",stdin);
	int i,j;
	int width,height;
	int num=0;
	int degree;
	int red_,green_,blue_;
	scanf("(%d,%d,%d)",&height,&width,&degree);
	for( i = 0 ; i < height ; i ++ )
	{
		for ( j = 0 ; j < width ; j ++ )
		{
			scanf("%d",&blue_);			
			scanf("%d",&green_);
			scanf("%d",&red_);
			DrawDot(red_/255.0,green_/255.0,blue_/255.0,x+j*EDGE,y-i*EDGE,EDGE);
		}
	}
	
}



void drawWatermelon(double x,double y,double r)
{
	r=r*2.5;
	SetPenColor("Green");
	StartFilledRegion(1);
	MovePen(x-1.2*r/2, y-1.2*r*pow(3, 1/2)/2);
	DrawArc(1.2*r, -120, 60);
	DrawLine(-1.2*r/2, 1.2*r*pow(3, 1/2)/2);
	DrawLine(-1.2*r/2, -1.2*r*pow(3, 1/2)/2);
	DrawLine(1.2*r, 0);
	EndFilledRegion();
	
	SetPenColor("Red");
	StartFilledRegion(1);
	MovePen(x-r/2, y-r*pow(3, 1/2)/2);
	DrawArc(r, -120, 60);
	DrawLine(-r/2, r*pow(3, 1/2)/2);
	DrawLine(-r/2, -r*pow(3, 1/2)/2);
	DrawLine(r, 0);
	EndFilledRegion();

	SetPenColor("Black");
	SetPenSize(2);
	MovePen(x-r/2, y-r*pow(3, 1/2)/2);
	DrawArc(r, -120, 60);
	MovePen(x-1.2*r/2, y-1.2*r*pow(3, 1/2)/2);
	DrawArc(1.2*r, -120, 60);
	DrawLine(-1.2*r/2, 1.2*r*pow(3, 1/2)/2);
	DrawLine(-1.2*r/2, -1.2*r*pow(3, 1/2)/2);
	
}

void drawApple(double x,double y ,double r)
{
	SetPenColor("Red");
	StartFilledRegion(0.8);
	MovePen(x+r, y);
	DrawArc(r, 0, 360);
	EndFilledRegion();
	
	SetPenColor("Black");
	SetPenSize(2);
	MovePen(x+r, y);
	DrawArc(r, 0, 360);
	MovePen(x/1.12, y+1.5*r-r*cos(20/180*PI));
	DrawArc(1.5*r, -110, 40);
    MovePen(x, y+1.5*r-r*cos(20/180*PI));
    DrawLine(0, r);
}

void drawBeer(double x,double y,double r)
{
	double dx = r*0.8;
	double dy = r*0.6;
	
	
    SetPenColor("Yellow");
	StartFilledRegion(1);
	MovePen(x+dx, y+dy);
	DrawLine(-2*dx, 0);
	DrawLine(0, -3*dy);
	DrawLine(2*dx, 0);
	DrawLine(0, 3*dy);
	EndFilledRegion();
	
	SetPenColor("Black");
	SetPenSize(2);
	MovePen(x+dx, y+dy);
	DrawLine(-2*dx, 0);
	DrawLine(0, -3*dy);
	DrawLine(2*dx, 0);
	DrawLine(0, 3*dy);
	DrawLine(0, dy/2);
	DrawLine(-2*dx, 0);
	DrawLine(0, -dy/2);
	DrawLine(0, dy);
	DrawLine(2*dx, 0);
	DrawLine(0, -dy);
	MovePen(x+dx, y+1.1*dy);
	DrawLine(dx, 0);
	DrawLine(0, -2.2*dy);
	DrawLine(-dx, 0);
	DrawLine(0, 0.2*dy);
	DrawLine(0.85*dx, 0);
	DrawLine(0, 1.8*dy);
	DrawLine(-0.85*dx, 0);
	
}

void drawHeart(double x,double y,double r)
{
	double dx = 0.015*r;
	double dy = 0.015*r;
	double a;
	
    SetPenColor("Red");
    StartFilledRegion(1);
	MovePen(x, y);
    for(a=0; a<=2*PI; a+=0.1){
    	DrawLine(2*dx*(sin(2*a)-sin(a)), 2*dy*(cos(a)-cos(2*a)));
	}
	EndFilledRegion();
	
	SetPenColor("Black");
	SetPenSize(2);
	MovePen(x, y);
    for(a=0; a<=2*PI; a+=0.1){
    	DrawLine(2*dx*(sin(2*a)-sin(a)), 2*dy*(cos(a)-cos(2*a)));
	}
}

void drawBubble(double x,double y,double r)
{
	SetPenColor("CREAM");
	StartFilledRegion(1);
	MovePen(x+r, y);
	DrawArc(r, 0, 360);
	EndFilledRegion();
}

void drawCandy(double x,double y,double r)
{
	int i;
	r=r/2.5;
	SetPenColor("Red");
	for(i=0; i<3; i++){
	    StartFilledRegion(1);
	    MovePen(x+i*r+r, y-i*pow(3, 0.5)*r);
	    DrawArc(r, 0, 360);
	    EndFilledRegion();
	}
	
	SetPenColor("Black");
	SetPenSize(2); 
	for(i=0; i<3; i++){
	    MovePen(x+i*r+r, y-i*pow(3, 0.5)*r);
	    DrawArc(r, 0, 360);
	}
	SetPenSize(3);
	MovePen(x-r/2, y+pow(3, 0.5)*r/2);
	DrawLine(-r/2, pow(3, 0.5)*r/2);
	MovePen(x+r*5/2, y-pow(3, 0.5)*r*5/2);
	DrawLine(r, -pow(3, 0.5)*r);
} 

void drawRice(double x,double y,double r)
{
	SetPenColor("BLACK");
	StartFilledRegion(1);
	MovePen(x+0.4*r, y);
	DrawLine(-0.8*r, 0);
	DrawLine(0, -0.5*r);
	DrawLine(0.8*r, 0);
	DrawLine(0, 0.5*r);
	EndFilledRegion();
	
	SetPenSize(2);
	MovePen(x, y+r);
	DrawLine(-0.5*pow(3, 0.5)*r, -1.5*r);
	DrawLine(pow(3, 0.5)*r, 0);
	DrawLine(-0.5*pow(3, 0.5)*r, 1.5*r);
}

void drawMilk(double x,double y,double r)
{
	r=r/1.5;
	SetPenColor("Blue");
	StartFilledRegion(0.8);
	MovePen(x+r, y+1.5*r);
	DrawLine(-2*r, 0);
	DrawLine(0, -3*r);
	DrawLine(2*r, 0);
	DrawLine(0, 3*r);
	EndFilledRegion();
	
	
	SetPenColor("Black");
	SetPenSize(2);
	MovePen(x+r, y+1.5*r);
	DrawLine(-2*r, 0);
	DrawLine(0, -3*r);
	DrawLine(2*r, 0);
	DrawLine(0, 3*r);
	DrawLine(r/pow(2, 0.5), r/pow(2, 0.5));
	DrawLine(r/pow(3, 0.5), -r/pow(3, 0.5));
	DrawLine(0, -3*r);
	DrawLine(-r/pow(3, 0.5)-r/pow(2, 0.5), r/pow(3, 0.5)-r/pow(2, 0.5));
	MovePen(x+r, y+1.5*r);
	DrawLine(r/pow(3, 0.5)+r/pow(2, 0.5), -r/pow(3, 0.5)+r/pow(2, 0.5));
	MovePen(x-r, y+1.5*r);
	DrawLine(r/pow(2, 0.5), r/pow(2, 0.5));
	DrawLine(2*r, 0);
	DrawLine(0, 0.5*r);
	DrawLine(-2*r, 0);
	DrawLine(0, -0.5*r);
}

void drawMedicine(double x, double y, int kind){
	double dx = 0.08, dy = 0.1;

	if(kind==1){
		SetPenColor("Brown");
		StartFilledRegion(0.8);
		MovePen(x+0.9*dx,y+dy);
		DrawLine(0, 2*dy);
		DrawLine(-2*0.9*dx, 0);
		DrawLine(0, -2*dy);
		DrawLine(2*0.9*dx, 0);
		EndFilledRegion();
	
		SetPenColor("Blue");
		StartFilledRegion(0.85);
		MovePen(x+2*dx,y-dy);
		DrawLine(dx, -dy);
		DrawLine(-6*dx, 0);
		DrawLine(dx, dy);
		DrawLine(4*dx, 0);
		EndFilledRegion();
	
		SetPenColor("Black");
		SetPenSize(2);
		MovePen(x+0.9*dx, y);
		DrawLine(0, 2*dy);
		DrawLine(-2*dx, 0);
		DrawLine(0, -2*dy);
		DrawLine(-2*dx, -2*dy);
		DrawLine(6*dx, 0);
		DrawLine(-2*dx, 2*dy);
	}
	else{
		SetPenColor("Brown");
		StartFilledRegion(0.8);
		MovePen(x+0.9*mediciner/2,y+mediciner*pow(3,1/2)/2+dy);
		DrawLine(0, 2*dy);
		DrawLine(-0.9*mediciner, 0);
		DrawLine(0, -2*dy);
		DrawLine(0.9*mediciner, 0);
		EndFilledRegion();
	
		SetPenColor("Red");
		StartFilledRegion(0.85);
		MovePen(x-mediciner,y-0.7*dy);
		DrawArc(mediciner, 180, 180);
		EndFilledRegion();
	
		SetPenColor("Black");
		SetPenSize(2);
		MovePen(x-mediciner/2, y+mediciner*pow(3,1/2)/2);
		DrawArc(mediciner, 120, 300);
		DrawLine(0, 2*dy);
		DrawLine(-mediciner, 0);
		DrawLine(0, -2*dy);
	}
	
	
	
	
	
	
}
