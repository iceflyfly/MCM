#include<stdio.h>
#include<stdlib.h>
#include <time.h> //用到了time函数

#define DBD 50
#define N 2000    //吞吐量
#define BBL 2     //B/L
#define L   4     //车道数
#define STARTN 12  //起始第几行开始并道
#define B BBL*L   //收费站个数
#define CARMAXNUM 5000 //最大车量
#define CARSNEEDTIME 2//每3 t一个车
#define CARSBS 1//每t 3个车
struct table {
    int havecar;//有无车
    int bianjie;//是否是边界
    int qianjin;//某时刻是否前进
    int bindao;//是否向左并道
    int carnum;//对应的车号
    int weightchange;//改变权值了
};
  
struct car {
    int number;//车的编号 用于算吞吐量
    int wait_weight;//等待权重
    int isauto;//是自驾车 
};

int chuche[N];
struct table table[4*STARTN][B]; //B是8，L+2 其中一行用于进车，一行用于算吞吐量
struct car   car[CARMAXNUM];
int wait_car[1000];
int count_d = 0;

void move(int x,int y);//x y 为车的横纵坐标
void move_auto(int x,int y);//自驾 
void change_table();//改变表
int current_carnum = 0;
int a[20] = {0,1,2,3,4,4,5,5,6,6,7,7};
/*int b[31]= {0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0, 
0,0,0,0,0,0,0,0,0,0,1};//!!!!!!!!!!!*/
int b[4] = {1,1,1,1};
int main() {
    int autocarnum=0; 
    int area=0;
    srand((unsigned)time(0)); //用时间做种，每次产生随机数不一样
    int i, j;
    int k = 1;
     for(i = 0; i < 1000; i++)
    {
    	wait_car[i] = 0;
    }
    //初始化table表
    for (i = 0; i<(4 * STARTN ); i++) {
        for (j = 0; j<B; j++) {
            table[i][j].havecar = 0;
            table[i][j].bianjie = 0;
            table[i][j].qianjin = 0;
            table[i][j].bindao = 0;
            table[i][j].carnum = -1;
        }
    }
    //初始化car数组
    for (i = 0; i <CARMAXNUM; i++) {
        car[i].number = i;
        car[i].wait_weight = 0;
        car[i].isauto=0; 
    }
    //初始化出车数组
    for (i = 0; i < N; i++) {
        chuche[i] =-1;
    }
    //初始化边界
	for(i=STARTN+1;i<4*STARTN;i++) 
    	table[i][0].bianjie = 1;
    for(i=2*STARTN+1;i<4*STARTN;i++) 
    	table[i][1].bianjie = 1;
    for(i=3*STARTN;i<4*STARTN;i++) 
    	table[i][2].bianjie = 1;
	for(i=4*STARTN-1;i<4*STARTN;i++) 
  	    table[i][3].bianjie = 1;
    /*printf("\n\n");
    for (i = 0; i<(L + 2); i++) {
        for (j = 0; j<B; j++) {
            if (table[i][j].bianjie == 1)
                printf("%d %d \n", i, j);
        }
    }*/
    for(i=0;i<4*STARTN;i++)
		for(j=0;j<B;j++){
			if(table[i][j].bianjie == 0)
				area++;
		} 
    int t = 0,flag=0;

    while (!flag) {
        printf("t=%d\n",t);
        printf("area=%d\n",area);
        int he1 = 0, he2 = 0, he3 = 0, he4 = 0, he5 = 0;
		 for (i = 0; i < (4 * STARTN ); i++) {
	        for (j = 0; j < 4; j++) {
	            if(table[i][j].carnum >= 0){
	            	he1++;
	            }           
	    	}
		 for (j = 4; j < 8; j++) {
	            if(table[i][j].carnum >= 0){
	            	if(j == 4)  he2++;
	            	else if(j == 5)  he3++;
	            	else if(j == 6)  he4++;
	            	else if(j == 7)  he5++;
	            }           
	    	}
	    }
	    
	    if((((100*he1)/(area-16*STARTN))>DBD)||(((100*he2)/(4*STARTN))>DBD)||(((100*he3)/(4*STARTN))>DBD)||(((100*he4)/(4*STARTN))>DBD)||(((100*he5)/(4*STARTN))>DBD) )
          count_d++;
        printf("发生拥堵次数=%d   总车数=%d\n",count_d,he1+he2+he3+he4+he5);
        printf("autocarnum=%d\n",autocarnum);
        int tuntu = 0;
        for (i = 0; chuche[i] != -1; i++)
            tuntu++;
        printf("吞吐量为%d\n", tuntu);
        if (CARSNEEDTIME <1) {
            int pos = 0;
            for (i = 0; i < CARSBS;i++){
                pos =rand() % 12;
                if (table[0][a[pos]].havecar == 0) {//!!!!!!!!!!!!!!!!!
                    table[0][a[pos]].havecar = 1;
                    table[0][a[pos]].carnum = current_carnum;
                   // if(pos==7&&!autocarnum)
                   if(pos==7||pos == 6|| pos == 5|| pos ==4)
                  	   //car[current_carnum].isauto=b[rand()%31];                  	   
                  	   car[current_carnum].isauto=b[rand()%4];
                    if(car[current_carnum].isauto==1)
                    	autocarnum++;
                    current_carnum++;
                }
                else
                    i--;
                //srand(time(0));
            }

        }
        else {
            if(t%CARSNEEDTIME==0){

                int pos = rand() % 12;
                while (table[0][a[pos]].havecar) {
                    pos = rand() % 12;
                }
                if (table[0][a[pos]].havecar == 0) {
                    table[0][a[pos]].havecar = 1;
                    table[0][a[pos]].carnum = current_carnum;
                    current_carnum++;
                }
            }
        }


        if (t == 600)//循环10次退出
            flag = 1;
        for (i = 0; i < (4 * STARTN ); i++) {
            for (j = 0; j < B; j++) {
                if(table[i][j].havecar==1)//!!!!!!
                     if(car[table[i][j].carnum].isauto==0)
                     	move(i, j);
                     else
                        move_auto(i,j);
                        
            }
        }

        change_table();
        //计算吞吐量
            int isin = 0;
            int l1 = 4 * STARTN -1;
            for (j = 0; j < B; j++) {
                if (table[l1][j].carnum >= 0) {
                    for (i = 0; chuche[i] != -1; i++) {//遍历看出车是否重复
                        if (table[l1][j].carnum == chuche[i]) {
                            isin = 1;
                        }
                    }
                    if (isin == 0) {//如果不在加入数组中
                        for (i = 0; chuche[i] != -1; i++);
                        chuche[i] = table[l1][j].carnum;
                    }
                    table[l1][j].carnum = -1;
                    table[l1][j].havecar = 0;
                }
                isin = 0;
            }


        for (i = 0; i < (4 * STARTN  ); i++)
            for (j = 0; j < B; j++) {
                table[i][j].bindao = 0;
                table[i][j].qianjin = 0;
            }
        t++;
        /*
        if (t % 2 == 0) {
            for (j = 0; j < B; j++) {
                if (table[0][j].havecar == 0) {
                    table[0][j].havecar = 1;
                    table[0][j].carnum = current_carnum;
                    current_carnum++;
                }

            }
        }
        */

    }//end of while


 

    system("pause");
    return 0;
}

void move(int x,int y)//
{
    if (table[x+1][y].bianjie != 1){//如果前方不是边界

        if (x != 0 && y != B - 1) {//不是出收费站也不是在最里道，可以并道
            if (table[x][y + 1].havecar == 0 && table[x + 1][y + 1].havecar == 0) {//如果左前方没有车左侧也没有车 可以并道
                table[x][y].bindao = 1;
            }
            else if (table[x ][y+1].havecar == 1 && table[x + 1][y + 1].havecar ==0&& car[table[x][y + 1].carnum].wait_weight <car[table[x][y].carnum].wait_weight) {//如果左前方无车但左侧有车需比较等待权值，权值大的先走
                //比左侧车权值大
                    table[x][y].bindao = 1;
            }
            else if (table[x + 1][y].havecar == 0 ) {//一维 如果无车就前进
                if(table[x][y-1].havecar&&car[table[x][y - 1].carnum].wait_weight <= car[table[x][y].carnum].wait_weight)//考虑右侧 只有等待权值大于等于右侧时前进
                    table[x][y].qianjin = 1;
                else if(table[x][y-1].havecar==0)
                    table[x][y].qianjin = 1;
            }
        }
        else if (table[x + 1][y].havecar == 0) {//一维 如果无车就前进
            if (table[x][y - 1].havecar&&car[table[x][y - 1].carnum].wait_weight <= car[table[x][y].carnum].wait_weight)//考虑右侧 只有等待权值大于等于右侧时前进
                table[x][y].qianjin = 1;
            else if (table[x][y - 1].havecar == 0)
                table[x][y].qianjin = 1;
        }
    }
    else {//是边界
        if (table[x][y + 1].havecar == 0 && table[x + 1][y + 1].havecar == 0)//如果左前方没有车左侧也没有车 可以并道
            table[x][y].bindao = 1;
        if (table[x][y + 1].havecar == 1 && table[x + 1][y + 1].havecar == 0) {//如果左前方无车但左侧有车需比较等待权值，权值大的先走
            if (car[table[x][y + 1].carnum].wait_weight < car[table[x][y].carnum].wait_weight)//比左侧车权值大
                table[x][y].bindao = 1;
        }
    }
    if (table[x][y].bindao == 1 || table[x][y].qianjin == 1)
        table[x][y].weightchange = 0;
        //car[table[x][y].carnum].wait_weight = 0;
    else
        table[x][y].weightchange = 1;
        //car[table[x][y].carnum].wait_weight++;
}
void move_auto(int x,int y)//
{
  
    if (table[x + 1][y].havecar == 0&&table[x + 2][y].havecar == 0) //一维 如果无车就前进
        table[x][y].qianjin = 1;
            
    if (table[x][y].qianjin ==1)
        table[x][y].weightchange = 0;
        //car[table[x][y].carnum].wait_weight = 0;
    else
        table[x][y].weightchange = 1;
        //car[table[x][y].carnum].wait_weight++;
}
void change_table() {
    int i, j;
    for (i = 0; i<(4 * STARTN); i++) {
        for (j = 0; j<B; j++) {
            if (table[i][j].bindao) {
                table[i][j].havecar =0;
                table[i + 1][j + 1].havecar = 1;
                table[i + 1][j+1].carnum = table[i][j].carnum;
                table[i][j].carnum = -1;
            }
            else if (table[i][j].qianjin) {
                table[i][j].havecar =0;
                table[i+1][j ].havecar = 1;
                table[i + 1][j].carnum = table[i][j].carnum;
                table[i][j].carnum = -1;
            }
            if (table[i][j].weightchange)
                car[table[i][j].carnum].wait_weight++;
        }
    }




    for (i = 0; i < (4 * STARTN ); i++) {
        for (j = 0; j < B; j++) {
            if(table[i][j].carnum == -1)
            printf("     ");
            else  printf("%5d", table[i][j].carnum);
        }
        printf("\n");
    }
    printf("-------------------------------------------------\n");
}

