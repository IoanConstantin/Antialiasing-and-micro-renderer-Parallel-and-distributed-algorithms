#include "homework.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

int num_threads, resize_factor;

int flagcolor=-1;

FILE *intrare, *iesire;

void readInput(const char * fileName, image *img) {
   	intrare=fopen(fileName,"r");

	img->sir1=malloc(3*sizeof(unsigned char));
	fread(img->sir1, 3, 1, intrare);
	fscanf(intrare, "%d %d", &img->latime, &img->inaltime);
	fscanf(intrare, "%d\n", &img->max_value);

	char *ceva1=malloc(3*sizeof(char));
	strcpy(ceva1,(char *)img->sir1);
	if(strcmp(ceva1,"P6\n")==0) flagcolor=1;
	if(strcmp(ceva1,"P5\n")==0) flagcolor=0;
	free(ceva1);

	if(flagcolor==1)
	{
		img->sir2=malloc(img->latime*img->inaltime*3*sizeof(unsigned char));
   		fread(img->sir2, img->latime*img->inaltime*3, 1, intrare);
	}

	if(flagcolor==0)
	{
		img->sir2=malloc(img->latime*img->inaltime*sizeof(unsigned char));
   		fread(img->sir2, img->latime*img->inaltime, 1, intrare);
	}
	
   	fclose(intrare);
}

void writeData(const char * fileName, image *img) { 
	iesire=fopen(fileName,"w");

	fwrite(img->sir1,1,2,iesire);

	fprintf(iesire, "\n%d %d\n", img->latime,img->inaltime);
	fprintf(iesire, "%d\n", img->max_value);
	
	if(flagcolor==1)
		fwrite(img->sir2,1,img->inaltime*img->latime*3,iesire);

	if(flagcolor==0)
		fwrite(img->sir2,1,img->inaltime*img->latime,iesire);
	
	free(img->sir1);
	free(img->sir2);	

	fclose(iesire);
}

void resize(image *in, image * out) {
	out->sir1=malloc(3*sizeof(unsigned char));

	memcpy(out->sir1,in->sir1,2);

	out->inaltime=(int)in->inaltime/resize_factor;
	out->latime=(int)in->latime/resize_factor;
	out->max_value=in->max_value;
	
	if(flagcolor==1) 
		out->sir2=malloc(out->inaltime*out->latime*3*sizeof(unsigned char));
	
	if(flagcolor==0) 
		out->sir2=malloc(out->inaltime*out->latime*sizeof(unsigned char));

	if(flagcolor==1)
	{
		int z=0;
		
		unsigned char **red=malloc(in->inaltime*sizeof(unsigned char *));
		for(int i=0;i<in->inaltime;i++)
			red[i]=calloc(in->latime,sizeof(unsigned char));

		unsigned char **green=malloc(in->inaltime*sizeof(unsigned char *));
		for(int i=0;i<in->inaltime;i++)
			green[i]=calloc(in->latime,sizeof(unsigned char));

		unsigned char **blue=malloc(in->inaltime*sizeof(unsigned char *));
		for(int i=0;i<in->inaltime;i++)
			blue[i]=calloc(in->latime,sizeof(unsigned char));

		unsigned char **new_red=malloc(in->inaltime*sizeof(unsigned char *));
		for(int i=0;i<in->inaltime;i++)
			new_red[i]=calloc(in->latime,sizeof(unsigned char));

		unsigned char **new_green=malloc(in->inaltime*sizeof(unsigned char *));
		for(int i=0;i<in->inaltime;i++)
			new_green[i]=calloc(in->latime,sizeof(unsigned char));

		unsigned char **new_blue=malloc(in->inaltime*sizeof(unsigned char *));
		for(int i=0;i<in->inaltime;i++)
			new_blue[i]=calloc(in->latime,sizeof(unsigned char));

		for(int i=0;i<in->inaltime;i++)
		{
			for(int j=0;j<in->latime;j++)
			{
				red[i][j]=in->sir2[3*z];
				green[i][j]=in->sir2[(3*z)+1];
				blue[i][j]=in->sir2[(3*z)+2];

		red[i][j]=red[i][j]-((int) (red[i][j]/(in->max_value+1)))*(in->max_value+1);
		green[i][j]=green[i][j]-((int) (green[i][j]/(in->max_value+1)))*(in->max_value+1);
	blue[i][j]=blue[i][j]-((int) (blue[i][j]/(in->max_value+1)))*(in->max_value+1);				
			
				z++;
			}
		}

		if(resize_factor%2==0)
		{
			int k=0;

			for(int i=resize_factor;i<((int) (in->inaltime/resize_factor))*resize_factor+resize_factor;i=i+resize_factor)
			{
				for(int j=resize_factor;j<((int) (in->latime/resize_factor))*resize_factor+resize_factor;j=j+resize_factor)
				{
					int q1=0;
					int q2=0;
					int q3=0;
					for(int a=1;a<=resize_factor;a++)
					{
						for(int b=1;b<=resize_factor;b++)
						{
		q1=q1+(int) red[i-a][j-b];
		q2=q2+(int) green[i-a][j-b];
		q3=q3+(int) blue[i-a][j-b];
						}
					}
		new_red[(i/resize_factor)-1][(j/resize_factor)-1]=
		(unsigned char) (q1/(resize_factor*resize_factor));
		out->sir2[k]=new_red[(i/resize_factor)-1][(j/resize_factor)-1];
		k++;

		new_green[(i/resize_factor)-1][(j/resize_factor)-1]=
		(unsigned char) (q2/(resize_factor*resize_factor));
		out->sir2[k]=new_green[(i/resize_factor)-1][(j/resize_factor)-1];
		k++;

		new_blue[(i/resize_factor)-1][(j/resize_factor)-1]=
		(unsigned char) (q3/(resize_factor*resize_factor));
		out->sir2[k]=new_blue[(i/resize_factor)-1][(j/resize_factor)-1];
		k++;
				}
			}
		}

		if(resize_factor==3)
		{
			int k=0;
			for(int i=resize_factor;i<=in->inaltime;i=i+resize_factor)
			{
				for(int j=resize_factor;j<=in->latime;j=j+resize_factor)
				{
new_red[(i/resize_factor)-1][(j/resize_factor)-1]=
(unsigned char) 
((int) 
((
((int) red[i-3][j-3])+
((int) red[i-3][j-2])*2+
((int) red[i-3][j-1])+
((int) red[i-2][j-3])*2+
((int) red[i-2][j-2])*4+
((int) red[i-2][j-1])*2+
((int) red[i-1][j-3])+
((int) red[i-1][j-2])*2+
((int) red[i-1][j-1])
)/16));
out->sir2[k]=new_red[(i/resize_factor)-1][(j/resize_factor)-1];
k++;

new_green[(i/resize_factor)-1][(j/resize_factor)-1]=
(unsigned char) 
((int)
((
((int) green[i-3][j-3])+
((int) green[i-3][j-2])*2+
((int) green[i-3][j-1])+
((int) green[i-2][j-3])*2+
((int) green[i-2][j-2])*4+
((int) green[i-2][j-1])*2+
((int) green[i-1][j-3])+
((int) green[i-1][j-2])*2+
((int) green[i-1][j-1])
)/16));
out->sir2[k]=new_green[(i/resize_factor)-1][(j/resize_factor)-1];
k++;

new_blue[(i/resize_factor)-1][(j/resize_factor)-1]=
(unsigned char) 
((int)
((
((int) blue[i-3][j-3])+
((int) blue[i-3][j-2])*2+
((int) blue[i-3][j-1])+
((int) blue[i-2][j-3])*2+
((int) blue[i-2][j-2])*4+
((int) blue[i-2][j-1])*2+
((int) blue[i-1][j-3])+
((int) blue[i-1][j-2])*2+
((int) blue[i-1][j-1])
)/16));
out->sir2[k]=new_blue[(i/resize_factor)-1][(j/resize_factor)-1];
k++;
				}
			}
		}

		for(int i=0;i<in->inaltime;i++)
			free(red[i]);

		for(int i=0;i<in->inaltime;i++)
			free(green[i]);

		for(int i=0;i<in->inaltime;i++)
			free(blue[i]);

		for(int i=0;i<in->inaltime;i++)
			free(new_red[i]);

		for(int i=0;i<in->inaltime;i++)
			free(new_green[i]);

		for(int i=0;i<in->inaltime;i++)
			free(new_blue[i]);

		free(red);
		free(green);
		free(blue);
		free(new_red);
		free(new_green);
		free(new_blue);
	}
	
	if(flagcolor==0)
	{
		int z=0;

		unsigned char **whiteblack=malloc(in->inaltime*sizeof(unsigned char *));
		for(int i=0;i<in->inaltime;i++)
			whiteblack[i]=calloc(in->latime,sizeof(unsigned char));
	
	unsigned char **new_whiteblack=malloc(out->inaltime*sizeof(unsigned char *));
		for(int i=0;i<out->inaltime;i++)
			new_whiteblack[i]=calloc(out->latime,sizeof(unsigned char));
				
		for(int i=0;i<in->inaltime;i++)
		{
			for(int j=0;j<in->latime;j++)
			{
				whiteblack[i][j]=in->sir2[z];

		whiteblack[i][j]=whiteblack[i][j]-((int) (whiteblack[i][j]/(in->max_value+1)))*(in->max_value+1);
				
				z++;
			}
		}
	
		if(resize_factor%2==0)
		{
			int k=0;
			for(int i=resize_factor;i<((int) (in->inaltime/resize_factor))*resize_factor+resize_factor;i=i+resize_factor)
			{
				for(int j=resize_factor;j<((int) (in->latime/resize_factor))*resize_factor+resize_factor;j=j+resize_factor)
				{
					int q=0;
					for(int a=1;a<=resize_factor;a++)
					{
						for(int b=1;b<=resize_factor;b++)
						{
							q=q+(int) whiteblack[i-a][j-b];
						}
					}
		new_whiteblack[(i/resize_factor)-1][(j/resize_factor)-1]=
		(unsigned char) (q/(resize_factor*resize_factor));
		out->sir2[k]=new_whiteblack[(i/resize_factor)-1][(j/resize_factor)-1];
		k++;
				}
			}
		}		
	
		if(resize_factor==3)
		{
			int k=0;
			for(int i=resize_factor;i<=in->inaltime;i=i+resize_factor)
			{
				for(int j=resize_factor;j<=in->latime;j=j+resize_factor)
				{
new_whiteblack[(i/resize_factor)-1][(j/resize_factor)-1]=
(unsigned char) 
((
((int) whiteblack[i-3][j-3])+
((int) whiteblack[i-3][j-2])*2+
((int) whiteblack[i-3][j-1])+
((int) whiteblack[i-2][j-3])*2+
((int) whiteblack[i-2][j-2])*4+
((int) whiteblack[i-2][j-1])*2+
((int) whiteblack[i-1][j-3])+
((int) whiteblack[i-1][j-2])*2+
((int) whiteblack[i-1][j-1])
)/16);
out->sir2[k]=new_whiteblack[(i/resize_factor)-1][(j/resize_factor)-1];
k++;
				}
			}
		}

		for(int i=0;i<in->inaltime;i++)
			free(whiteblack[i]);
		
		for(int i=0;i<out->inaltime;i++)
			free(new_whiteblack[i]);

		free(whiteblack);
		free(new_whiteblack);
	}
}
