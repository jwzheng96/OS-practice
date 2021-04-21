#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include<vector>
#include<queue>
#include<cstdlib>
#include<iomanip>
#include<algorithm>
using namespace std;
#define N 20                            //进程数

///数据结构——通用的进程控制块PCB
struct PCB
{
    char process_name;                  //进程名
    double arrival_time;                //进程到达时间
    double service_time;                //进程要求服务时间
    double start_time;                  //进程开始执行时间
    double need_service_time;           //进程还需占用CPU时间
    double end_time;                    //进程执行完成时间
    double process_priority;            //进程优先级
    double turnaround_time;             //进程周转时间
    double weight_turnaround_time;      //进程带权周转时间
    string status;                      //进程状态
};

PCB pcb[N];                             //进程
double average_TT;                      //平均周转时间
double average_WTT;                     //平均带权周转时间
int n;                                  //进程数
int flag = 0;

///函数定义：
void Init_process();                     //进程初始化
bool cmp_name(PCB a, PCB b);             //按进程名排序，按序输出
bool cmp_process_priority(PCB a, PCB b); //按优先级大小排序

void HPF();                              //HPF优先级调度算法
void Result_Print(PCB Tmp_pcb[]);        //最终结果打印（各进程的完成时间，各进程的周转时间及带权周转时间）
bool All_finished(PCB pd_pcb[]);         //判断是不是所有进程都已经执行结束，作为循环的判断条件（状态输出的时间优化）

/************************************
主函数：main()
*************************************/
int main()
{
    Init_process();    //进程初始化
	HPF();
    return 0;
}

/************************************
函数名称：Init_process()
函数功能：进程初始化，输入进程数，以及每个进程的进程名 到达时间 服务时间
返回值：无
*************************************/
void Init_process()
{
	int i;
	cout<<endl<<"---------------高优先级调度算法--------------"<<endl;
    cout<<"请输入进程数：";
    cin>>n;
    cout<<"请依次输入 进程名 到达时间 服务时间 优先级(如：A 0 3 5)，注意优先级数值越大优先级越高"<<endl;
    for( i = 0; i < n; i++)
    {
        char p_name;             //进程名
        double p_arrive,p_service; //进程到达时间 服务时间
        double p_priority;
        cin>>p_name>>p_arrive>>p_service>>p_priority;
        pcb[i].process_name = p_name;
        pcb[i].arrival_time = p_arrive;
        pcb[i].service_time = p_service;
        pcb[i].process_priority = p_priority;
        if(!p_arrive) pcb[i].status = "Ready";
        else pcb[i].status = "WFA";
    }
}

/************************************
函数名称：cmp_name(PCB a, PCB b)
函数功能：按进程名排序，按序输出
返回值：无
*************************************/
bool cmp_name(PCB a, PCB b)
{
    return a.process_name < b.process_name;
}

/************************************
函数名称：Print(PCB Tmp_pcb[])
函数功能：格式化输出
返回值：无
*************************************/
void Result_Print(PCB Tmp_pcb[])
{
    PCB cp_pcb[N];
	int i;
    for( i=0; i < n; i++)
        cp_pcb[i] = Tmp_pcb[i];
    sort(cp_pcb,cp_pcb+n,cmp_name);
    /*for( i = 0; i < 100; i++) cout<<"*";*/
    cout<<endl;
    cout<<setw(3)<<" "<<"进程名"<<setw(14)<<"      到达时间"<<setw(13)<<"    服务时间"<<setw(12)<<"    完成时间"<<setw(12)<<"    周转时间"<<setw(20)<<"      带权周转时间"<<"\t  "<<endl;
   
    for( i = 0; i < n; i++)
    {
        cout<<setw(6)<<cp_pcb[i].process_name<<setw(14)<<cp_pcb[i].arrival_time<<setw(12)<<cp_pcb[i].service_time
        <<setw(12)<<cp_pcb[i].end_time<<setw(12)
        <<cp_pcb[i].turnaround_time<<setw(4)<<" "<<"\t\t"<<cp_pcb[i].weight_turnaround_time<<setw(8)<<"\t"<<" "<<" "<<" ";
     cout<<" "<<endl;
    }
   /* for( i = 0; i < 100; i++) cout<<"*";*/
    cout<<endl;
}

/************************************
函数名称：bool All_finished(PCB pd_pcb[]);
函数功能：判断是不是所有进程都已经
执行结束，作为循环的判断条件（状态
输出的时间优化）
返回值：true or false
*************************************/
bool All_finished(PCB pd_pcb[])
{
    int t=0,i;
    for( i = 0; i < n; i++)
    {
        if(pd_pcb[i].status == "Finish")
            t++;   //继续循环执行
    }
    if(t==n)  return false;
    else return true;
}

/************************************
函数名称：cmp_process_priority(PCB a, PCB b)
函数功能：按优先级大小排序
（注：优先级数值越大，优先级越高）
返回值：无
*************************************/
bool cmp_process_priority(PCB a, PCB b)
{
    if(a.arrival_time != b.arrival_time)
        return a.process_priority > b.process_priority;
    else return a.arrival_time < b.arrival_time;
}

/************************************
函数名称：HPF()
函数功能：HPF优先级调度算法
返回值：无
*************************************/
void HPF()
{
    PCB HPF_pcb[N];
    PCB Res_PCB[N];
    int t = 0;
	int i,j,m,k;
    for( i = 0; i < n; i++)
        HPF_pcb[i] = pcb[i];
    sort(HPF_pcb,HPF_pcb+n,cmp_process_priority);                 //按优先级大小排序
    cout<<endl;
    int excute_time = HPF_pcb[0].arrival_time;
    flag = 0;
    for( i = 0; i < n; i++)                                    //找 所有进程中 最早到达的进程时间 即为 系统的初始时间
    {
        excute_time = excute_time > HPF_pcb[i].arrival_time ? HPF_pcb[i].arrival_time : excute_time;
    }
    for(; All_finished(HPF_pcb); excute_time++)               //按总时间遍历 输出中间状态
    {
        for ( i = 0; i < n; i++)                               //遍历进程
        {
            if(HPF_pcb[i].arrival_time == excute_time && !flag)   //先到的进程若CPU空闲则先执行，就绪队列中有其他进程排队时，选择优先级高的进程
            {
                for( j = 0; j < n; j++)                        //同时间到达的，均修改状态
                {
                    if(HPF_pcb[j].arrival_time <= excute_time && HPF_pcb[j].status == "WFA")
                        HPF_pcb[j].status = "Ready";
                }
                for( j = 0; j <= i; j++)                                          //同时间到达的，先选优先级高的
                {
                    if(HPF_pcb[j].status == "Ready")
                    {
                        HPF_pcb[j].start_time = excute_time;                          //当前CPU空闲，到达的进程执行
                        HPF_pcb[j].end_time = excute_time + HPF_pcb[j].service_time;  //非抢占式，故该进程结束时间= 当前时间 + 服务时间
                        HPF_pcb[j].need_service_time = HPF_pcb[j].service_time;       //仍需要占用CPU的时间
                        HPF_pcb[j].status = "Running";                                //修改当前进程的状态为Running
                        Res_PCB[t++] = HPF_pcb[j];
                        flag = 1;
                        break;
                    }
                }
                break;
            }
            else if(HPF_pcb[i].arrival_time == excute_time && flag && HPF_pcb[i].status == "WFA")   //某进程结束，当前进程到达
            {
                bool mark = 0;
                HPF_pcb[i].status = "Ready";                //某进程到达，但CPU被占用着，该状态为Ready
                for( j = 0; j < n; j++)
                {
                    if(HPF_pcb[j].status == "Running" && HPF_pcb[j].end_time == excute_time)    //找到正在执行的进程，有且只有一个,这个进程刚好结束
                    {
                        HPF_pcb[j].status = "Finish";       //某进程执行完毕
                        flag = 0;
                        HPF_pcb[j].need_service_time = 0;   //不再需要CPU资源
                        HPF_pcb[j].turnaround_time = excute_time - HPF_pcb[j].arrival_time;     //周转时间 = 当前时间 - 到达时间
                        HPF_pcb[j].weight_turnaround_time = 1.0 * (HPF_pcb[j].turnaround_time)/(HPF_pcb[j].service_time);   //带权周转时间 = 周转时间 / 服务时间
                        HPF_pcb[j].weight_turnaround_time = ((int)((HPF_pcb[j].weight_turnaround_time*1000) + 0.5)) * 1.0 / 1000;
                        for(m = i; m < n; m++)         //若有其他进程也在当前时刻到达，状态位需进行修改
                        {
                            if(HPF_pcb[m].arrival_time <= HPF_pcb[j].end_time && HPF_pcb[j].status == "WFA")
                            {
                                HPF_pcb[m].status = "Ready";
                            }
                        }
                        for(k = 0; k < n; k++)                                             //找第一个ready进程
                        {
                            if(HPF_pcb[k].status == "Ready")
                            {
                                flag = 1;
                                HPF_pcb[k].start_time = excute_time;                           //开始时间等于当前时间
                                HPF_pcb[k].end_time = excute_time + HPF_pcb[k].service_time;   //进程的结束时间 = 当前时间 + 服务时间
                                HPF_pcb[k].status = "Running";                                 //修改进程状态 为 Running
                                Res_PCB[t++] = HPF_pcb[k];
                                HPF_pcb[k].need_service_time = HPF_pcb[k].service_time;
                                break;
                            }
                        }
                        mark = 1;
                    }
                    else if(HPF_pcb[j].status == "Running" && HPF_pcb[j].end_time!=excute_time) //找到正在执行的进程，有且只有一个，正在执行
                    {
                        HPF_pcb[j].need_service_time = HPF_pcb[j].end_time - excute_time;       //结束时间 - 当前时间 = 进程仍需CPU时间
                        mark = 1;
                        for(int m = i; m < n; m++)
                        {
                            if(HPF_pcb[m].arrival_time <= excute_time && HPF_pcb[m].status == "WFA")
                            {
                                HPF_pcb[m].status = "Ready";
                            }
                        }
                    }
                    if(mark) break;
                }
                break;
            }
            else if(HPF_pcb[i].end_time == excute_time && HPF_pcb[i].status == "Running")   //某进程执行结束
            {
                HPF_pcb[i].status = "Finish";        //某进程执行完毕
                flag = 0;
                HPF_pcb[i].need_service_time = 0;    //不再需要CPU资源
                HPF_pcb[i].turnaround_time = excute_time - HPF_pcb[i].arrival_time;
                //周转时间 = 当前时间 - 到达时间
                HPF_pcb[i].weight_turnaround_time = 1.0 * (HPF_pcb[i].turnaround_time)/(HPF_pcb[i].service_time);
                //带权周转时间 = 周转时间 / 服务时间
                HPF_pcb[i].weight_turnaround_time = ((int)((HPF_pcb[i].weight_turnaround_time*1000) + 0.5)) * 1.0 / 1000;
                //保留两位小数——四舍五入
                for(j = i; j < n; j++)           //如果当前进程执行结束时，下一进程刚好到达，状态位需进行修改
                {
                    if(HPF_pcb[j].arrival_time == HPF_pcb[i].end_time && HPF_pcb[j].status == "WFA")
                    {
                        HPF_pcb[j].status = "Ready";
                    }
                }
                for( j = 0; j < n; j++)                                             //找第一个ready进程
                {
                    if(HPF_pcb[j].status == "Ready")
                    {
                        flag = 1;
                        HPF_pcb[j].start_time = excute_time;                           //开始时间等于当前时间
                        HPF_pcb[j].end_time = excute_time + HPF_pcb[j].service_time;   //进程的结束时间 = 当前时间 + 服务时间
                        HPF_pcb[j].status = "Running";                                 //修改进程状态 为 Running
                        Res_PCB[t++] = HPF_pcb[j];
                        HPF_pcb[j].need_service_time = HPF_pcb[j].service_time;
                        break;
                    }
                }
                break;
            }
        }
    }

   
    Result_Print(HPF_pcb);
    average_TT = 0;
    average_WTT = 0;
    for(i = 0; i < n; i++)
    {
        average_TT += HPF_pcb[i].turnaround_time;
        average_WTT += HPF_pcb[i].weight_turnaround_time;
    }
    cout<<"调用优先级算法的平均周转时间为:";
    cout<<average_TT/n<<endl;
    cout<<"调用优先级算法的平均带权周转时间为:";
    cout<<average_WTT/n<<endl<<endl;
}