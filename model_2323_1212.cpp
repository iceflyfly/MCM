#include<stdio.h>
#include<stdlib.h>
#include <time.h> //�õ���time����

#define N 20000    //������
#define BBL 2     //B/L
#define L   4     //������
#define STARTN 12  //��ʼ�ڼ��п�ʼ����
#define B BBL*L   //�շ�վ����
#define CARMAXNUM 500000 //�����
#define CARSNEEDTIME 0//ÿ3 tһ����
#define CARSBS 2//ÿt 3����
struct table {
    int havecar;//���޳�
    int bianjie;//�Ƿ��Ǳ߽�
    int qianjin;//ĳʱ���Ƿ�ǰ��
    int bindao;//�Ƿ����󲢵�
    int carnum;//��Ӧ�ĳ���
    int weightchange;//�ı�Ȩֵ��
};

struct car {
    int number;//���ı�� ������������
    int wait_weight;//�ȴ�Ȩ��
};

int chuche[N];
struct table table[4*STARTN][B]; //B��8��L+2 ����һ�����ڽ�����һ��������������
struct car   car[CARMAXNUM];

void move(int x,int y);//x y Ϊ���ĺ�������
void change_table();//�ı��
int current_carnum = 0;
int a[20] = {0,1,2,3,4,4,5,5,6,6,7,7};
int main() {

    srand((unsigned)time(0)); //��ʱ�����֣�ÿ�β����������һ��
    int i, j;
    int k = 1;
    //��ʼ��table��
    for (i = 0; i<(4 * STARTN ); i++) {
        for (j = 0; j<B; j++) {
            table[i][j].havecar = 0;
            table[i][j].bianjie = 0;
            table[i][j].qianjin = 0;
            table[i][j].bindao = 0;
            table[i][j].carnum = -1;
        }
    }
    //��ʼ��car����
    for (i = 0; i < 500; i++) {
        car[i].number = i;
        car[i].wait_weight = 0;
    }
    //��ʼ����������
    for (i = 0; i < N; i++) {
        chuche[i] =-1;
    }
    //��ʼ���߽�

    table[STARTN][0].bianjie = 1;
    table[2*STARTN][1].bianjie = 1;
    table[3*STARTN-1][2].bianjie = 1;
    table[4*STARTN-1][3].bianjie = 1;
    /*printf("\n\n");
    for (i = 0; i<(L + 2); i++) {
        for (j = 0; j<B; j++) {
            if (table[i][j].bianjie == 1)
                printf("%d %d \n", i, j);
        }
    }*/

    int t = 0,flag=0;

    while (!flag) {

        if (CARSNEEDTIME <1) {
            int pos = 0;
            for (i = 0; i < CARSBS;i++){
                pos =rand() % 12;
                if (table[0][a[pos]].havecar == 0) {
                    table[0][a[pos]].havecar = 1;
                    table[0][a[pos]].carnum = current_carnum;
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


        if (t == 600)//ѭ��10���˳�
            flag = 1;
        for (i = 0; i < (4 * STARTN ); i++) {
            for (j = 0; j < B; j++) {
                if(table[i][j].havecar==1)
                     move(i, j);
            }
        }

        change_table();
        //����������
            int isin = 0;
            int l1 = 4 * STARTN -1;
            for (j = 0; j < B; j++) {
                if (table[l1][j].carnum >= 0) {
                    for (i = 0; chuche[i] != -1; i++) {//�����������Ƿ��ظ�
                        if (table[l1][j].carnum == chuche[i]) {
                            isin = 1;
                        }
                    }
                    if (isin == 0) {//������ڼ���������
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


    flag = 0;
    for (i = 0; chuche[i] != -1; i++)
        flag++;
    printf("������Ϊ%d", flag);

    system("pause");
    return 0;
}

void move(int x,int y)//
{
    if (table[x+1][y].bianjie != 1){//���ǰ�����Ǳ߽�

        if (x != 0 && y != B - 1) {//���ǳ��շ�վҲ����������������Բ���
            if (table[x][y + 1].havecar == 0 && table[x + 1][y + 1].havecar == 0) {//�����ǰ��û�г����Ҳû�г� ���Բ���
                table[x][y].bindao = 1;
            }
            else if (table[x ][y+1].havecar == 1 && table[x + 1][y + 1].havecar ==0&& car[table[x][y + 1].carnum].wait_weight <car[table[x][y].carnum].wait_weight) {//�����ǰ���޳�������г���Ƚϵȴ�Ȩֵ��Ȩֵ�������
                //����೵Ȩֵ��
                    table[x][y].bindao = 1;
            }
            else if (table[x + 1][y].havecar == 0 ) {//һά ����޳���ǰ��
                if(table[x][y-1].havecar&&car[table[x][y - 1].carnum].wait_weight <= car[table[x][y].carnum].wait_weight)//�����Ҳ� ֻ�еȴ�Ȩֵ���ڵ����Ҳ�ʱǰ��
                    table[x][y].qianjin = 1;
                else if(table[x][y-1].havecar==0)
                    table[x][y].qianjin = 1;
            }
        }
        else if (table[x + 1][y].havecar == 0) {//һά ����޳���ǰ��
            if (table[x][y - 1].havecar&&car[table[x][y - 1].carnum].wait_weight <= car[table[x][y].carnum].wait_weight)//�����Ҳ� ֻ�еȴ�Ȩֵ���ڵ����Ҳ�ʱǰ��
                table[x][y].qianjin = 1;
            else if (table[x][y - 1].havecar == 0)
                table[x][y].qianjin = 1;
        }
    }
    else {//�Ǳ߽�
        if (table[x][y + 1].havecar == 0 && table[x + 1][y + 1].havecar == 0)//�����ǰ��û�г����Ҳû�г� ���Բ���
            table[x][y].bindao = 1;
        if (table[x][y + 1].havecar == 1 && table[x + 1][y + 1].havecar == 0) {//�����ǰ���޳�������г���Ƚϵȴ�Ȩֵ��Ȩֵ�������
            if (car[table[x][y + 1].carnum].wait_weight < car[table[x][y].carnum].wait_weight)//����೵Ȩֵ��
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
            printf("    ");
            else  printf("%4d ", table[i][j].carnum);
        }
        printf("\n");
    }
    printf("-------------------------------------------------\n");
}

