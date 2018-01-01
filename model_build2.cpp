#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define Lane 4
#define table_l 8
#define table_w 6
#define rate_m 1
#define time_limit 10
#define time_slot 10
#define car_final 8//table_l * time_limit / time_slot
using namespace std;

int car_num = 0;
int time_total = 0;
int car_pass[car_final];//记录已并入L的车的全部编号

typedef struct Car{
    int number;
    int wait_weight;
    int front;//某时刻是否前进
    int turn_left;//是否向左并道
    int move_done;
}Car;
Car table[table_w][table_l];


void car_move(int i, int j)//当前位置有车
{
    if(table[i+1][j+1].number >= 0)//当前车左前方区域合法
    {
        if(table[i+1][j+1].number == 0)//当前车左前方无车
        {
            if(table[i][j+1].number == 0 || ((table[i][j+1].number == 1)&&(table[i][j].wait_weight >= table[i][j+1].wait_weight)))
            {//当前车左侧无车或当前车等待权重更高
                table[i][j].turn_left = 1;//下一时刻当前车向左前方移动
            }
        }
    }
    if((table[i][j].turn_left == 0)&&(table[i+1][j].number >= 0))//当前车前方区域合法
    {
        if(table[i+1][j].number == 0)//当前车前方无车
        {
            if(j == 0 || table[i][j-1].number == -1)//当前车处于边界时
            {
                table[i][j].front = 1;
            }
            else if(table[i][j-1].number == 0 || ((table[i][j+1].number == 1)&&(table[i][j].wait_weight > table[i][j+1].wait_weight)))
            {//当前车右侧无车或当前车等待权重更高
                table[i][j].front = 1;//下一时刻当前车向左前方移动
            }
        }
    }
    if((table[i][j].turn_left == 0)&&(table[i][j].front == 0))
    {
         table[i][j].wait_weight++;
    }
}

int main()
{
    int i = 0, j = 0;

    //起始时刻未有任何车通过
    for(i = 0; i < car_final; i++)
    {
        car_pass[i] = 0;
    }

    //初始化table
    for(i = 0; i < table_w; i++)
    {
        for(j = 0; j < (table_l-Lane); j++)
        {
            table[i][j].wait_weight = 0;
            table[i][j].number = 0;
            table[i][j].front = 0;
            table[i][j].turn_left = 0;
            table[i][j].move_done = 0;
        }
    }
    for(i = 0; i < table_w; i++)
    {
        for(j = 0; j < (table_l-Lane); j++)
        {
            if(i > rate_m*(j+1))//当前车是边界角的车
            {
                table[i][j].number = -1;
            }
        }
    }


    //while(car_pass[car_final-1]==0)//当所有车未全部进入L区域时
    while(time_total <= 10)//当所有车未全部进入L区域时
    {

        for(j = 0; j < table_l; j++)
        {
            for(i = table_w; i >= 0; i--)
            {
                if(table[i][j].number >0)//当前位置有车
                {
                    car_move(i,j);
                    if(table[i][j].turn_left == 1)
                    {
                         table[i+1][j+1].move_done = 1;
                    }
                    else if(table[i][j].front == 1)
                    {
                        table[i+1][j].move_done = 1;
                    }
                }
            }
        }
        for(j = 0; j < table_l; j++)
        {
            for(i = table_w; i >= 0; i--)
            {
                if(table[i][j].move_done)//当前位置有车
                {
                    if(table[i][j].turn_left == 1)
                    {
                         table[i][j].number = table[i-1][j-1].number;
                         table[i][j].wait_weight = 0;
                         table[i][j].front = 0;
                         table[i][j].turn_left = 0;
                         table[i][j].move_done = 0;
                         table[i-1][j-1].number = 0;//当前格清零
                         table[i-1][j-1].wait_weight = 0;
                         table[i-1][j-1].front = 0;
                         table[i-1][j-1].turn_left = 0;
                         table[i-1][j-1].move_done = 0;
                    }
                    else if(table[i][j].front == 1)
                    {
                        table[i][j].number = table[i-1][j].number;
                        table[i][j].wait_weight = 0;
                        table[i][j].front = 0;
                        table[i][j].turn_left = 0;
                        table[i][j].move_done = 0;
                        table[i-1][j].number = 0;//当前格清零
                        table[i-1][j].wait_weight = 0;
                        table[i-1][j].front = 0;
                        table[i-1][j].turn_left = 0;
                        table[i-1][j].move_done = 0;
                    }
                }
            }
        }

        //每过一定间隔时间发一次车,并将新发车辆信息写入table
        int car_release = time_total % time_slot;
        if(!car_release)
        {
            for(i = 0; i < table_l; i++)
            {
                car_num++;
                table[0][i].number = car_num;

            }
        }

        for(i = 0; i < table_w; i++)
        {
            for(j = 0; j < table_l; j++)
            {
                if(table[i][j].number >=0)//当前车在合法区域
                {
                    cout<<table[i][j].number<<" ";
                }
                else
                {
                    cout<<"  ";
                }
            }
            cout<<endl;
        }
        cout<<"------------------------------------time:"<<time_total<<endl;
        //时间片推进1
        time_total++;
    }
    return 0;
}

