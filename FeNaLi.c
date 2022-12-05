#include <stdio.h>
#include <bmpio.h>
#include <math.h>
#include <string.h>
unsigned char Pic[2000][2000][3];
unsigned char Pic2[2000][2000][3];
unsigned int sum[500][500]={0};
unsigned char Average [500][500][7];
char Table [100][100];
char Tablevert [100][100];
int findMax(char Folder);
void crop(char NameR[],int num,char Letter);
void resize(char NameR[],int num,char Letter);
void keyword(int num,char letter,int Max,unsigned int sum[][500],int x); 
void Grid(char Address[],int line[],int column[],int size[]);
void croptable(char DataAddress[],char Address[],int line[],int column[],int size[]);
void cropRaw(int size[]);
void resizeRaw(int size[]);
void Detection(int size[]);
void detectWords(int size[],int line[],int column[],char Address[]);
void Vertical(int size[]);
char Letter(int Index);
int main ()
{
	int i,x,j,Max,k,l,height,width,line[50]={0},column[50]={0},size[2]={0},p=0;					//height and width represent the number of pictures in our entry data file
	char AddressC[30]={".\\DataSet\\A\\1.bmp"};
	char AddressRe[30]={".\\Cropped\\CroppedA1.bmp"};
	char RawData[30]={".\\RawData\\00_00.bmp"};
	char DataAddress[70];		
	char Folder='A',Command;
	printf("Plz enter the entry data's address: ");
	gets(DataAddress);
	for(i=0;i<=25;i++)
	{	
		Max=findMax(Folder);					
		for(j=1;j<=Max;j++)
		{
			crop(AddressC,j,Folder);
//			printf("%c%d  Cropped!     ",Folder,j);
			resize(AddressRe,j,Folder);
//			printf("Resized! \n");
			keyword(j,Folder,Max,sum,p);
		}
		if (Max!=0)
			p++;
//		printf("**%c**\n",Folder);
		Folder++;	
		
		memset(sum, 0, sizeof(int) * 500 * 500);
	}
	printf("\nDataSets got cropped and resized and the keywords are made \n");
	Grid(DataAddress,line,column,size); 							// finding borders in our entry data so we can crop and resize it later
	croptable(DataAddress,RawData,line,column,size);				//cropping and resizing the entry data and saving them for detection phase
	cropRaw(size);	
	printf("\nTable components got cropped ");
	resizeRaw(size);
	printf("\nTable components got Resized");
	printf("\nThe table got cropped and resized\n");
	printf("\nTable detected\n");
	Detection(size);
	Vertical(size);	
	detectWords(size,line,column,DataAddress);
}
void crop(char NameR[],int num,char Letter)			//Working
{	
//printf("%d \n", num);
	int h,w,i,j,up,down,right,left,found=0,x=0,y=0,count=0,exit,temp;
	if (num>=10)
	{
		strcpy(NameR,".\\DataSet\\A\\...bmp");
		NameR[10]=Letter;
		temp=num%10;
		NameR[13]='0'+temp;
		temp=num/10;
		NameR[12]='0'+temp;
	}
	else
	{
		strcpy(NameR,".\\DataSet\\A\\1.bmp");
		NameR[12]='0'+num;
		NameR[10]=Letter;
	}
		
//	printf("Worked");
	readBMP(NameR,&w,&h,Pic);
	char NameW[40]={".\\Cropped\\CroppedA0.bmp"};
	if (num>=10){
		strcpy(NameW,".\\Cropped\\CroppedA00.bmp");
		NameW[17]=Letter;
		temp=num%10;
		NameW[19]='0'+temp;
		num/=10;
		NameW[18]='0'+num;
//		puts(NameW);
	}
//	NameW[1000]={"CroppedA00.bmp"};
	else{
		NameW[17]=Letter;
		NameW[18]='0'+ num;
//		puts(NameW);
	}
	
//	puts(NameW);
	while (found==0)
	{	
		for (i=0;i<h && found!=1;i++)
		{
			for (j=0;j<w && found!=1;j++)
			{
				if (!(Pic[i][j][0]==255 && Pic[i][j][1]==255 && Pic[i][j][2]==255))
				{
					found=1;
					up=i;
				}
			}
		}
	}
//	printf("Upper edge found!  ");
	found=0;
	while (found==0)
	{	
		for (i=up;i<h && found!=1;i++)
		{
			count=0;
			for (j=0;j<w && found!=1;j++)
			{
				if (Pic[i][j][0]==255 && Pic[i][j][1]==255 && Pic[i][j][2]==255)
				{
					count++;
				}
			}
			if (count==w)
			{
				found=1;
				down=i-1;
			}
			else if(i==h-1)
			{
				down=i-1;
				found=1;
			}
				
		}
	}
//	printf("Bottom edge Found!  ");
	found=0;
	while (found==0)
	{	
		for (j=0;j<w && found!=1;j++)
		{
			for (i=up;i<down && found!=1;i++)
			{
				if (!(Pic[i][j][0]==255 && Pic[i][j][1]==255 && Pic[i][j][2]==255))
				{
					found=1;
					left=j;
				}
			}
		}
	}
//	printf("Left edge Found!  ");
	found=0;
	while (found==0)
	{	
		for (j=left;j<w && found!=1 ;j++)
		{
			count=0;
			for (i=up;i<down && found!=1;i++)
			{
				if (Pic[i][j][0]==255 && Pic[i][j][1]==255 && Pic[i][j][2]==255)
				{
					count++;
				}
			}
			if (count==down-up)
			{
				found=1;
				right=j-1;
			}
			else if(j==w-1)
			{
				right=j-1;
				found=1;
			}	
		}
	}
//	printf("Right edge Found!  \n");
//	printf("%d %d %d %d \n",u,d,l,r);
	for (i=up;i<=down;i++)
	{
		y=0;
		for (j=left;j<=right;j++)
		{
			Pic2[x][y][0]=Pic[i][j][0];
			Pic2[x][y][1]=Pic[i][j][1];
			Pic2[x][y][2]=Pic[i][j][2];
			y++;
		}
		x++;
	}
	w=right-left+1;
	h=down-up+1;
	saveBMP(Pic2,w,h,NameW);
//	return 0;
}
void resize(char NameR[],int num,char Letter)			//Working
{
	int width,height,i,j,k,l,temp;
	double cowidth,coheight,tempheight,tempwidth;
	char NameW[40]={".\\Resized\\Resized0..bmp"};
	if (num>=10)
	{
//		printf("Worked"); 
		strcpy(NameR,".\\Cropped\\Cropped....bmp");
		NameR[17]=Letter;
		temp=num%10;
		NameR[19]='0'+temp;
		temp=num/10;
		NameR[18]='0'+temp;
//		puts(NameR);
	}
	else {
		strcpy(NameR,".\\Cropped\\Cropped...bmp");
		NameR[18]='0'+num;
		NameR[17]=Letter;
//		puts(NameR);
	}
	if (num>=10)
	{
		strcpy(NameW,".\\Resized\\Resized00..bmp");
		NameW[17]=Letter;
		temp=num%10;
		NameW[19]='0'+temp;
		temp=num/10;
		NameW[18]='0'+temp;
	}
	else{
		strcpy(NameW,".\\Resized\\Resized0..bmp");
		NameW[18]='0'+num;
		NameW[17]=Letter;
	}
	readBMP(NameR,&width,&height, Pic);
	coheight=500.0/height;
	tempheight=height*coheight;
	cowidth=500.0/width;
	tempwidth=width*cowidth;

	for (i=0;i<=height;i++)
	{
		for (j=0;j<=width;j++)
		{
			for (k=i*coheight;k<(i+1)*coheight;k++)
			{				
				for(l=j*cowidth;l<(j+1)*cowidth;l++)
				{
					Pic2[k][l][0]=Pic[i][j][0];
					Pic2[k][l][1]=Pic[i][j][1];
					Pic2[k][l][2]=Pic[i][j][2];
				}
				
			}
		}
		
	}
	saveBMP(Pic2,tempwidth,tempheight, NameW);
}
void keyword(int num,char Letter,int Max,unsigned int sum[][500],int x)				//Working
{
	int i,j,k,temp,height,width;
	char Address[1000];
	char Saving[1000];
	strcpy(Address,".\\Resized\\Resized.0.bmp");
	Address[17]=Letter;
	if (num>=10)
	{
		strcpy(Address,".\\Resized\\Resized.0..bmp");
		temp=num%10;
		Address[19]='0'+temp;
		temp=num/10;
		Address[18]='0'+temp;
		Address[17]=Letter;
	}
	else
	{
		Address[18]='0'+num;
		Address[17]=Letter;
	}
	readBMP(Address,&width,&height,Pic);
	for (i=0;i<500;i++)
	{
		for (j=0;j<500;j++)
		{
			sum[i][j]+=Pic[i][j][0];	//	sum[i][j][1]+=Pic[i][j][1];		sum[i][j][2]+=Pic[i][j][2];
		if (num==Max)
			{
				sum[i][j]/=Max;	//	sum[i][j][1]/=Max;		sum[i][j][2]/=Max;
				Pic2[i][j][0]=sum[i][j];		Pic2[i][j][1]=sum[i][j];		Pic2[i][j][2]=sum[i][j];
				/*if (sum[i][j]==255)
					Average[i][j][x]=0;	*/
				Average[i][j][x]=sum[i][j];
				
			}
		}
	}
	

	if (num==Max)
	{
//		printf("%d \n",x);
		strcpy(Saving,".\\KeyWord\\KeyWordA.bmp");
		Saving[17]=Letter;
//		puts(Saving);
		saveBMP(Pic2,width,height,Saving);
//		printf("is it done?");
	}
		
/*	strcpy(Saving,".\\KeyWord\\KeyWordA.bmp");
	Saving[17]=Letter;
//	puts(Saving);
	saveBMP(Average,width,height,Saving);
	printf("is it done?");*/
	//	sum[0]+=Pic[j][k][0];	sum[1]+=Pic[j][k][1];	sum[2]+=Pic[j][k][2];
//				printf("\n %d   %d   %d \n",sum[1],sum[2],sum[3]);
}
int findMax(char Folder)		//Working
{
	if (Folder=='A'||Folder=='E')
			return 32;
		else if(Folder=='B')
			return 41;
		else if(Folder=='I')
			return 15;
		else if(Folder=='O')
			return 25;
		else if(Folder=='R')
			return 17;
		else if(Folder=='S')
			return 20;
		else
			return 0;
}
void Grid(char Address[],int line[],int column[],int size[])		//Working
{
		int h,w,i,j,x=0,flag=0,white=1,sum=0;
//		puts(Address);
		readBMP(Address,&w,&h,Pic);	
		for (i=0;i<h;i++)						//Drawing a line before the start of each line of letters so we can crop them later
		{
//			flag=0;
			for (j=0;j<w;j++)
			{
				if (Pic[i][j][0]!=255 && white==1)
				{
					line[x]=i;
					x++;	//flag=1;
					white=0;
					size[0]++;					//Represents the number of letter lines
				}
				if(Pic[i][j][0]==255)
					sum++;
			}
			if (sum>=w-1)
				white=1;		//We have reached white lines so we start looking for a non-white pixel again
			sum=0;	
		}
		x=0;
		for (j=0;j<w;j++)
		{
			for (i=0;i<h;i++)
			{
				if (Pic[i][j][0]!=255 && white==1)
				{
					column[x]=j;
					x++;
					white=0;
					size[1]++;					//Represents the number of letter columns
				}
				if(Pic[i][j][0]==255)
					sum++;
			}
			if (sum>=h-1)
				white=1;
			sum=0;				
		}
		
}
void croptable(char DataAddress[],char Address[],int line[],int column[],int size[]) 		//Working 		
{
	int k,l,i,j,x=0,y=0,width,height,coheight,cowidth,tempi,tempj;
//	printf("%d %d %d %d %d \n",line[0],line[1],line[2],line[3],line[4]);
//	printf("%d %d %d %d %d",column[0],column[1],column[2],column[3],column[4]);
//	printf("\n Lines: %d, Columns: %d ",size[0],size[1]);
	readBMP(DataAddress,&width,&height,Pic);
	line[size[0]]=height;	column[size[1]]=width;
	for(i=0;i<size[0] ;i++)
	{
		for (j=0;j<size[1] ;j++)
		{
			for (k=line[i];k<line[i+1];k++)
			{
				for(l=column[j];l<column[j+1];l++)
				{
					Pic2[x][y][0]=Pic[k][l][0];		Pic2[x][y][1]=Pic[k][l][1];		Pic2[x][y][2]=Pic[k][l][2];		
					y++;
				}
				x++; 
				y=0;
			}
			x=0;	y=0;
//			if (j<9)													// the reason for this pointless job is that white gab in column 9 -_____-
//			{
				tempj=j;		tempi=i;
				Address[14]=tempj%10+'0';	tempj/=10;
				Address[13]=tempj%10+'0';	
				Address[11]=tempi%10+'0';	tempi/=10;
				Address[10]=tempi%10+'0';
//				puts(Address);
				saveBMP(Pic2,column[j+1]-column[j],line[i+1]-line[i],Address);
/*			}
			if (j>9)
			{
				tempj=j;		tempi=i;
				tempj--;		
				Address[14]=tempj%10+'0';	tempj/=10;
				Address[13]=tempj%10+'0';	
				Address[11]=tempi%10+'0';	tempi/=10;
				Address[10]=tempi%10+'0';
//				puts(Address);
				saveBMP(Pic2,column[j+1]-column[j],line[i+1]-line[i],Address);
			}*/
			
			
//			Address[11]++;
		}
//		Address[10]++;
//		Address[11]='0';
	}
//	printf("\nCropped Table ");
	
}
void cropRaw(int size[])		//Working
{
	int h,w,i,j,up,down,right,left,found=0,x=0,y=0,count=0,exit,temp,i1,j1,tempi,tempj,temp1;
	char Address[35]={".\\RawData\\00_00.bmp"};
//	printf(" lines: %d, columns: %d ", size[0], size[1]);
	for (i1=0;i1<size[0];i1++)
	{
		temp1=size[1];
		if(size[1]>10)
			temp1--;												// -1 is for solving the problem that the columns were going to 12 while there were only 11... that white gap ffs (@_@)
		for (j1=0;j1<=temp1;j1++)			
		{
//			printf("Reading: ");
//			puts(Address);
			tempj=j1;		tempi=i1;
			Address[14]=tempj%10+'0';	tempj/=10;
			Address[13]=tempj%10+'0';	
			Address[11]=tempi%10+'0';	tempi/=10;
			Address[10]=tempi%10+'0';				//puts(Address);
			readBMP(Address,&w,&h,Pic);	
			while (found==0)
			{	
				for (i=0;i<h && found!=1;i++)
				{
					for (j=0;j<w && found!=1;j++)
					{
						if (!(Pic[i][j][0]==255 && Pic[i][j][1]==255 && Pic[i][j][2]==255))
						{
							found=1;							up=i;
						}
					}
				}
			}
		//	printf("Upper edge found!  ");
			found=0;
			while (found==0)
			{	
				for (i=up;i<h && found!=1;i++)
				{
					count=0;
					for (j=0;j<w && found!=1;j++)
					{
						if (Pic[i][j][0]==255 && Pic[i][j][1]==255 && Pic[i][j][2]==255)
						{
							count++;
						}
					}
					if (count==w)
					{
						found=1;						down=i-1;
					}
					else if(i==h-1)
					{
						down=i-1;						found=1;
					}
						
				}
			}
		//	printf("Bottom edge Found!  ");
			found=0;
			while (found==0)
			{	
				for (j=0;j<w && found!=1;j++)
				{
					for (i=up;i<down && found!=1;i++)
					{
						if (!(Pic[i][j][0]==255 && Pic[i][j][1]==255 && Pic[i][j][2]==255))
						{
							found=1;							left=j;
						}
					}
				}
			}
		//	printf("Left edge Found!  ");
			found=0;
			while (found==0)
			{	
				for (j=left;j<w && found!=1 ;j++)
				{
					count=0;
					for (i=up;i<down && found!=1;i++)
					{
						if (Pic[i][j][0]==255 && Pic[i][j][1]==255 && Pic[i][j][2]==255)
						{
							count++;
						}
					}
					if (count==down-up)
					{
						found=1;
						right=j-1;
					}
					else if(j==w-1)
					{
						right=j-1;
						found=1;
					}	
				}
			}
		//	printf("Right edge Found!  \n");
		//	printf("%d %d %d %d \n",u,d,l,r);
			for (i=up;i<=down;i++)
			{
				y=0;
				for (j=left;j<=right;j++)
				{
					Pic2[x][y][0]=Pic[i][j][0];
					Pic2[x][y][1]=Pic[i][j][1];
					Pic2[x][y][2]=Pic[i][j][2];
					y++;
				}
				x++;
			}
			w=right-left+1;
			h=down-up+1;
			saveBMP(Pic2,w,h,Address);
			found=0;	x=0;	y=0;	count=0;
		}
	}		
}
void resizeRaw(int size[])		//Working
{
	int h,w,i,j,u,d,r,l,f=0,x=0,y=0,count=0,exit,temp,i1,j1,width,height,k,tempi,tempj,temp1;
	double cowidth,coheight,tempheight,tempwidth;
	char Address[35]={".\\RawData\\00_00.bmp"};
//	printf(" lines: %d, columns: %d ", size[0], size[1]);
	for (i1=0;i1<size[0];i1++)
	{
		temp1=size[1];
		if(size[1]>10)
			temp1--;
		for (j1=0;j1<=temp1;j1++)										//-1 has the same reason cropRaw had (I mean i like it how these bugs happen alot and i solve them by counting stuff xD)
		{
//			printf("Reading: ");
//			puts(Address);
			tempj=j1;		tempi=i1;
			Address[14]=tempj%10+'0';	tempj/=10;
			Address[13]=tempj%10+'0';	
			Address[11]=tempi%10+'0';	tempi/=10;
			Address[10]=tempi%10+'0';	//			puts(Address);
			readBMP(Address,&width,&height,Pic);
//			printf("  Read \n");
			coheight=500.0/height;
			tempheight=height*coheight;
			cowidth=500.0/width;
			tempwidth=width*cowidth;
		
			for (i=0;i<=height;i++)
			{
				for (j=0;j<=width;j++)
				{
					for (k=i*coheight;k<(i+1)*coheight;k++)
					{				
						for(l=j*cowidth;l<(j+1)*cowidth;l++)
						{
							Pic2[k][l][0]=Pic[i][j][0];
							Pic2[k][l][1]=Pic[i][j][1];
							Pic2[k][l][2]=Pic[i][j][2];
						}
						
					}
				}
				
			}
			saveBMP(Pic2,tempwidth,tempheight,Address);
			Address[11]++;
		}
		Address[10]++;
		Address[11]='0';
	}
}
void Detection(int size[])					// Working
{
//	printf("%d,%d",size[0],size[1]);
	int k,l,i,x,j,width,height,Min=20000000,MinIndex=0,tempi,tempj,temp1;
	char Address[40]={".\\RawData\\00_00.bmp"};
	int Diff[7]={0},temp;	
	for(k=0;k<size[0];k++)
	{
		temp1=size[1];
		if(size[1]>10)
			temp1--;
		for(l=0;l<=temp1;l++)										// Sigh* i mentioned the reason for -1 above -____-
		{
				memset(Diff, 0, sizeof(int)  *7 );
				Min=20000000;
//				Address[11]=l+'0';
//				Address[10]=k+'0';
//				puts(Address);
				tempj=l;		tempi=k;
				Address[14]=tempj%10+'0';	tempj/=10;
				Address[13]=tempj%10+'0';	
				Address[11]=tempi%10+'0';	tempi/=10;
				Address[10]=tempi%10+'0';//				puts(Address);
				readBMP(Address,&width,&height,Pic);
				for (x=0;x<7;x++)
				{
					for(i=0;i<height;i++)
					{
						for (j=0;j<width;j++)
						{					
							if (Pic[i][j][0]<Average[i][j][x])
								temp=(Average[i][j][x]-Pic[i][j][0]);
							else
								temp=(Pic[i][j][0]-Average[i][j][x]);	
							Diff[x]+=temp;
			//				printf("\n  Average: %d Pic: %d",Average[i][j][x],Pic[i][j][0]);
							
						}
					}
			//		printf("\nMin: %d",Min);
			//		Diff[x]=abs(Diff[x]);
			//		printf("\n Diff: %d",Diff[x]);
					if (Diff[x] <= Min)
					{
						Min=Diff[x];	MinIndex=x;
					}		
				}			
			Table[k][l]=Letter(MinIndex);
			printf("%c ",Table[k][l]);							
		}
		printf("\n");
	}			
}
char Letter(int Index)				//Working 
{
	if (Index==0)
		return 'A';
	if (Index==1)
		return 'B';
	if (Index==2)
		return 'E';
	if (Index==3)
		return 'I';
	if (Index==4)
		return 'O';
	if (Index==5)
		return 'R';
	if (Index==6)
		return 'S';						
}
void detectWords(int size[],int line[],int column[],char Address[])
{
	// PS: chon function e RNG ro too C balad naboodam (rand fek konam maximum esho nemishod tayin konam) dasti 10 halat rang mokhtalef dadam :)
	int num,i,j,k,repeat[20]={0},height,width,length[20],lenghtT,lengthTV,lenghtres,place,x,y,color[10][3]={{0,150,25},{255,150,255},{100,250,50},{200,156,9},{253,123,10},{255,255,25},{255,150,255},{100,250,50},{200,156,9},{253,123,10}};
	readBMP (Address,&width,&height,Pic2);
	char word[20][12],*result,*result2,wordrev[20][12];
	FILE*Input;
	lenghtT=strlen(Table[0]);
	lengthTV=strlen(Tablevert[0]);
//	printf("Table lenght= %d \n",lenghtT);
	Input=fopen("dictionary.txt","r");
	fscanf(Input,"%d",&num);
//	printf("%d",num);
	for (i=0;i<num;i++)
	{
		fscanf(Input,"%s",word[i]);
//		printf("%s\n",word[i][3]);
		length[i]=strlen(word[i]);
//		printf("%d\n",length[i]);
	}
	for (i=0;i<size[0];i++)
	{
		for(j=0;j<num;j++)
		{	
//			printf("%s\n",Table[i]);
//			printf("%s\n",word[j]);
		//	result=strstr(Table[i],word[j]);
			result=strstr(Table[i],word[j]);
//			printf("%s\n",result);
			if (result!=NULL)
			{
				lenghtres=strlen(result);
				place=lenghtT-lenghtres;
//				printf("Place of word= %d \n",place);
//				printf("Found one  Repeat=%d\n",repeat[j]);
				repeat[j]++;
//				printf("Found one at line #%d \n",i+1);
//				printf("Coloring Line %d for the word %s \n",i,word[j]);
				for (x=line[i];x<line[i+1];x++)
				{
					for(y=column[place];y<column[place+length[j]];y++)
					{
						if(Pic2[x][y][0]!=255 && Pic2[x][y][1]!=255 && Pic2[x][y][2]!=255)
						{
							Pic2[x][y][0]=color[j][0];	Pic2[x][y][1]=color[j][1];	Pic2[x][y][2]=color[j][2];
						}
										
					}
				}
			}	
			result=strstr(Table[i],strrev(word[j]));
			if (result!=NULL)
			{
				repeat[j]++;
				lenghtres=strlen(result);
				place=lenghtT-lenghtres;
//				printf("Place of word= %d \n",place);
	//			printf("Found one at line #%d \n",i+1);
//				printf("Coloring Line %d for the word %s \n",i+1,word[j]);
				for (x=line[i];x<line[i+1];x++)
				{
					for(y=column[place];y<column[place+length[j]];y++)
					{
						
						if(Pic2[x][y][0]!=255&&Pic2[x][y][1]!=255&&Pic2[x][y][2]!=255)
						{
							Pic2[x][y][0]=color[j][0];	Pic2[x][y][1]=color[j][1];	Pic2[x][y][2]=color[j][2];
						}
										
					}
				}
//			strrev(word[j]);
//				printf("%c \n",word[j]);
			}
				
		}
		
	}
	for (i=0;i<=size[1];i++)
	{
		for(j=0;j<num;j++)
		{	
			result=strstr(Tablevert[i],word[j]);
			if (result!=NULL)
			{
				lenghtres=strlen(result);
				place=lengthTV-lenghtres;
//				printf("Place of word= %d \n",place);
				repeat[j]++;
				for (x=column[i];x<column[i+1];x++)
				{
					for(y=line[place];y<line[place+length[j]];y++)
					{
						if(Pic2[y][x][0]!=255&&Pic2[y][x][1]!=255&&Pic2[y][x][2]!=255)
						{
							Pic2[y][x][0]=color[j][0];	Pic2[y][x][1]=color[j][1];	Pic2[y][x][2]=color[j][2];
						}
										
					}
				}
			}
			result=strstr(Tablevert[i],strrev(word[j]));
			if (result!=NULL)
			{
				repeat[j]++;
				lenghtres=strlen(result);
				place=lenghtT-lenghtres;
//				printf("Place of word= %d \n",place);
				for (x=column[i];x<column[i+1];x++)
				{
					for(y=line[place];y<line[place+length[j]];y++)
					{
						if(Pic2[y][x][0]!=255&&Pic2[y][x][1]!=255&&Pic2[y][x][2]!=255)
						{
							Pic2[y][x][0]=color[j][0];	Pic2[y][x][1]=color[j][1];	Pic2[y][x][2]=color[j][2];
						}
										
					}
				}
			}
				
		}
		
	}
	saveBMP(Pic2,width,height,"Result.bmp");
	for (i=0;i<num;i++)
	{
		printf("Word %d: %s     ",i+1,strrev(word[i]));
		printf("Repeat times: %d \n",repeat[i]);
	}
	fclose(Input);
//	detectWordVerti(size,word,num,length)
	
}
void Vertical(int size[])
{
	int i,j;
	for (i=0;i<size[0];i++)
	{
		for (j=0;j<size[1];j++)
		{
			Tablevert[j][i]=Table[i][j];
		}
		
	}
/*	printf("*********************************\n");
	for (i=0;i<size[1];i++)
	{
		for (j=0;j<size[0];j++)
		{
			printf("%c ",Tablevert[i][j]);
		}
		printf("\n");
	}*/
}
