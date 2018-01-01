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
int car_pass[car_final];//��¼�Ѳ���L�ĳ���ȫ�����

typedef struct Car{
    int number;
    int wait_weight;
    int front;//ĳʱ���Ƿ�ǰ��
    int turn_left;//�Ƿ����󲢵�
    int move_done;
}Car;
Car table[table_w][table_l];


void car_move(int i, int j)//��ǰλ���г�
{
    if(table[i+1][j+1].number >= 0)//��ǰ����ǰ������Ϸ�
    {
        if(table[i+1][j+1].number == 0)//��ǰ����ǰ���޳�
        {
            if(table[i][j+1].number == 0 || ((table[i][j+1].number == 1)&&(table[i][j].wait_weight >= table[i][j+1].wait_weight)))
            {//��ǰ������޳���ǰ���ȴ�Ȩ�ظ���
                table[i][j].turn_left = 1;//��һʱ�̵�ǰ������ǰ���ƶ�
            }
        }
    }
    if((table[i][j].turn_left == 0)&&(table[i+1][j].number >= 0))//��ǰ��ǰ������Ϸ�
    {
        if(table[i+1][j].number == 0)//��ǰ��ǰ���޳�
        {
            if(j == 0 || table[i][j-1].number == -1)//��ǰ�����ڱ߽�ʱ
            {
                table[i][j].front = 1;
            }
            else if(table[i][j-1].number == 0 || ((table[i][j+1].number == 1)&&(table[i][j].wait_weight > table[i][j+1].wait_weight)))
            {//��ǰ���Ҳ��޳���ǰ���ȴ�Ȩ�ظ���
                table[i][j].front = 1;//��һʱ�̵�ǰ������ǰ���ƶ�
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

    //��ʼʱ��δ���κγ�ͨ��
    for(i = 0; i < car_final; i++)
    {
        car_pass[i] = 0;
    }

    //��ʼ��table
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
            if(i > rate_m*(j+1))//��ǰ���Ǳ߽�ǵĳ�
            {
                table[i][j].number = -1;
            }
        }
    }


    //while(car_pass[car_final-1]==0)//�����г�δȫ������L����ʱ
    while(time_total <= 10)//�����г�δȫ������L����ʱ
    {

        for(j = 0; j < table_l; j++)
        {
            for(i = table_w; i >= 0; i--)
            {
                if(table[i][j].number >0)//��ǰλ���г�
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
                if(table[i][j].move_done)//��ǰλ���г�
                {
                    if(table[i][j].turn_left == 1)
                    {
                         table[i][j].number = table[i-1][j-1].number;
                         table[i][j].wait_weight = 0;
                         table[i][j].front = 0;
                         table[i][j].turn_left = 0;
                         table[i][j].move_done = 0;
                         table[i-1][j-1].number = 0;//��ǰ������
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
                        table[i-1][j].number = 0;//��ǰ������
                        table[i-1][j].wait_weight = 0;
                        table[i-1][j].front = 0;
                        table[i-1][j].turn_left = 0;
                        table[i-1][j].move_done = 0;
                    }
                }
            }
        }

        //ÿ��һ�����ʱ�䷢һ�γ�,�����·�������Ϣд��table
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
                if(table[i][j].number >=0)//��ǰ���ںϷ�����
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
        //ʱ��Ƭ�ƽ�1
        time_total++;
    }
    return 0;
}

