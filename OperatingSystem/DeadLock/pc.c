#include<stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include<stdlib.h>
#include <string.h>
#define SEMPERM 0600
#define TRUE 1
#define FALSE 0

#define R1_F "R1.txt"
#define R2_F "R2.txt"
#define R3_F "R3.txt"
#define R1_Q "Q_R1.txt"
#define R2_Q "Q_R2.txt"
#define R3_Q "Q_R3.txt"


typedef union   _semun
{
     int val;
     struct semid_ds *buf;
     ushort *array;
} semun;

// Class Lock
typedef struct _lock { 
   int semid;
} Lock;

// Class CondVar
typedef struct _cond // file 1개가 1개의 conditional val
{
   int semid;
   char *queueLength; // 파일이름
} CondVar;

// Class Process information
typedef struct _process // 기본적인 프로세스의 정보를 저장하는 클래스가 있으면 유용할것 같았습니다.
{
    int thinking_time;
    int eating_time;
    pid_t pid; 
    char * name;
}ProcessInfo;

int initsem (key_t semkey, int n)
{
   int status = 0, semid;
   if ((semid = semget (semkey, 1, SEMPERM | IPC_CREAT | IPC_EXCL)) == -1)//semget 은 세마포어 식별자 또는 key 와 일치하는 세마포어 식별자 반환 
   {
       if (errno == EEXIST)
            semid = semget (semkey, 1, 0);
   }
   else
   {
       semun arg;
       arg.val = n;
       status = semctl(semid, 0, SETVAL, arg);
   }
   if (semid == -1 || status == -1)
   {
       perror("initsem failed");
       return (-1);
   }
   return (semid);
}

int p (int semid)
{
   struct sembuf p_buf; 
   p_buf.sem_num = 0; // 세마포어 번호
   p_buf.sem_op = -1; // 세마포어 증감값
   p_buf.sem_flg = SEM_UNDO; // 옵션
   if (semop(semid, &p_buf, 1) == -1)
   {
      printf("p(semid) failed");
      exit(1);
   }
   return (0);
}

int v (int semid)
{
   struct sembuf v_buf;
   v_buf.sem_num = 0;
   v_buf.sem_op = 1;
   v_buf.sem_flg = SEM_UNDO;
   if (semop(semid, &v_buf, 1) == -1)
   {
      printf("v(semid) failed");
      exit(1);
   }
   return (0);
}

// Shared variable by file
reset(char *fileVar) {
// fileVar라는 이름의 텍스트 화일을 새로 만들고 0값을 기록한다.
    FILE * out = fopen(fileVar,"w+");
    fprintf(out," %d",0);
    fclose(out);
}
chopstick_reset(char  *fileVar)
{
    FILE * out = fopen(fileVar,"w+");
    fprintf(out," %d",1);
    fclose(out);
}
Store(char *fileVar,int i) {
// fileVar 화일 끝에 i 값을 append한다.
    FILE * out = fopen(fileVar,"a");
    fprintf(out," %d",i);
    fclose(out);
}

int Load(char *fileVar) {
// fileVar 화일의 마지막 값을 읽어 온다.
    FILE * in = fopen(fileVar , "r");
    int last_num;
    while(1)
    {
        fscanf(in,"%d",&last_num);
        if(feof(in))
            break;
    }
    fclose(in);
    return last_num;
}

add(char *fileVar,int i) {
// fileVar 화일의 마지막 값을 읽어서 i를 더한 후에 이를 끝에 append 한다.
    int last_num = Load(fileVar);
    last_num += i;
    Store(fileVar,last_num);
}

sub(char *fileVar,int i) {
// fileVar 화일의 마지막 값을 읽어서 i를 뺀 후에 이를 끝에 append 한다.
    int last_num = Load(fileVar);
    last_num -= i;
    Store(fileVar,last_num);
}


initLock(Lock *l, key_t semkey) {
   if ((l->semid = initsem(semkey,1)) < 0)    
   // 세마포를 연결한다.(없으면 초기값을 1로 주면서 새로 만들어서 연결한다.)
      exit(1);
}

Acquire(Lock *l)
{
   p(l->semid);
}

Release(Lock *l)
{

   v(l->semid);
}

// CondVar 의 acquire & release
Ac_Con(CondVar * c)
{
    p(c->semid);
}

Re_Con(CondVar * c)
{
    v(c->semid);
}

initCondVar(CondVar *c, key_t semkey, char *queueLength) //queue length  파일이름
{
   c->queueLength = queueLength;
   reset(c->queueLength); // queueLength=0
   if ((c->semid = initsem(semkey,0)) < 0)    
   // 세마포를 연결한다.(없으면 초기값을 0로 주면서 새로 만들어서 연결한다.)
      exit(1); 
}

initProcessInfo(ProcessInfo * prc, int eating_time , int thinking_time, char * name)
{
   prc->eating_time = eating_time;
   prc->thinking_time = thinking_time;
   prc->name = name;
   prc->pid = getpid();
   chopstick_reset(R1_F);
   chopstick_reset(R2_F);
   chopstick_reset(R3_F);
}

//int get_time(ProcessInfo * p) //result 파일에서 가장 최근에 끝난 프로세스에 찍힌 시간을 들고옵니다.
//{
//    int current_time = Load(R_F); // Read Result file (current time)
//    return current_time;
//}
AppendToFile(ProcessInfo * prc , int val , char * filename)
{
    FILE * in = fopen(filename,"a");
    fprintf(in,"\n%d %d",prc->pid,val); // 각자 화일에 맞는 정보를 입력해줍니다.
    fclose(in);
}

enQueue_Process(ProcessInfo *p ,  CondVar * c , char * filename)
{
    add(c->queueLength,1);//enqueue
}

deQueue_Process(ProcessInfo *p ,  CondVar * c , char *filename)
{
    sub(c->queueLength,1);
}

Wait(ProcessInfo *prc, CondVar *c, Lock *lock, char* filename)
{
   // enQueue
   enQueue_Process(prc,c,filename);
   Release(lock);
   Ac_Con(c);
   Acquire(lock);
   deQueue_Process(prc,c,filename);
}

Signal(CondVar *c)
{
    Re_Con(c);
}

//Broadcast(CondVar *c)
//{
//    int number = Load(c->queueLength);
//    for(int i=0;i<number;i++)
//        Re_Con(c);
//}
//
//int check_AW_AR()
//{
//    int aw,ar;
//    aw = Load(AW_F);
//    ar = Load(AR_F);
//    return ((aw+ar)>0?1:0);
//}
//int check_AW_WW()
//{
//    int aw,ww;
//    aw = Load(AW_F);
//    ww = Load(WW_F);
//    return ((aw+ww)>0?1:0);
//}
//
//write(ProcessInfo *p , Lock *lock, CondVar * reader, CondVar * writer)
//{
//    Acquire(lock);   // lock.Acquire()
//    while(check_AW_AR())
//    {
//        Wait(p,writer,lock,WW_F);
//    }
//    //write down Active Reader file
//    
//    AppendToFile(p,get_time(p),1,AW_F); //AW로 넣어줍니다.
//
//    if(p->sleep_time > get_time(p)) // 현재시간을 찾습니다.
//        p->c_time = p->sleep_time;
//    else
//       p->c_time = get_time(p);
//
//    Release(lock);
//    
//    sleep(p->act_time); // 문서 작성중...
//    
//    Acquire(lock);  // lock.Acquire()
//    
//    //write result file (time)
//    AppendToFile(p,get_time(p),p->c_time + p->act_time,R_F);
//        
//    //finish write, write in AW file
//    AppendToFile(p,get_time(p),0,AW_F);
//    
//    if(Load(WW_F)>0)
//        Signal(writer);
//    else if(Load(WR_F)>0)
//        Broadcast(reader);
//    Release(lock);
//}
//
//read(ProcessInfo *p , Lock *lock, CondVar * reader, CondVar * writer)
//{
//    Acquire(lock);   // lock.Acquire()
//    while(check_AW_WW())
//    {
//        Wait(p,reader,lock,WR_F);
//    }
//    //write down Active Reader file
//    AppendToFile(p,get_time(p),Load(AR_F)+1,AR_F);
//    
//    if(p->sleep_time > get_time(p))
//        p->c_time = p->sleep_time;
//    else
//       p->c_time = get_time(p);
//
//    Release(lock);
//    
//    sleep(p->act_time);
//    
//    Acquire(lock);   // lock.Acquire()
//    
//    //write result file (time)
//    AppendToFile(p,get_time(p),p->c_time + p->act_time,R_F);
//    
//    //finish read, write in AR file
//    AppendToFile(p, get_time(p) ,Load(AR_F)-1,AR_F);
//    
//    if(Load(AR_F) == 0 && Load(WW_F)>0)
//        Signal(writer);
//    Release(lock);
//}
//sleep func
void think(int time)
{
	sleep(time);
}
void eat(int time)
{
	sleep(time);
}
void Take_R1(ProcessInfo * prc , Lock *lock ,CondVar * c)
{
    Acquire(lock);
	while (Load(R1_F)==0) // R1이 1이면 젓가락이 있고 0이면 젓가락이 없어서 기다려야 함
    {
		printf("%s is waiting R1.. \n",prc->name); // print message: getpid()가 R1을 기다림
        Wait(prc,c,lock,R1_Q);
		printf("%s is wake up for R1.. \n",prc->name); // print message: getpid()가 R1을 기다리다가 깨어남
    }
    AppendToFile(prc,0,R1_F); //Store(R1,0) 대신에 파일에 어느 철학자 인지와 값을 저장해 놓음
    printf("%s bring R1 !! \n",prc->name);// print message: getpid()가  R1을 가져옴
    Release(lock);
}

void Take_R2(ProcessInfo * prc , Lock *lock ,CondVar * c )
{
    Acquire(lock);
    while (Load(R2_F)==0) // R2가 1이면 젓가락이 있고 0이면 젓가락이 없어서 기다려야 함
    {
		printf("%s is waiting R2.. \n",prc->name); // print message: getpid()가 R2을 기다림
        Wait(prc,c,lock,R2_Q);
	    printf("%s is wake up for R2.. \n",prc->name); // print message: getpid()가 R2을 기다리다가 깨어남
    }
    AppendToFile(prc,0,R2_F); //Store(R2,0) 대신에 파일에 어느 철학자 인지와 값을 저장해 놓음
    printf("%s bring R2 !! \n",prc->name);// print message: getpid()가  R2을 가져옴
    Release(lock);
}

void Take_R3(ProcessInfo * prc , Lock *lock ,CondVar * c )
{
    Acquire(lock);
    while (Load(R3_F)==0) // R3가 1이면 젓가락이 있고 0이면 젓가락이 없어서 기다려야 함
    {
		printf("%s is waiting R3.. \n",prc->name); // print message: getpid()가 R3을 기다림
        Wait(prc,c,lock,R3_Q);
        printf("%s is wake up for R3.. \n",prc->name); // print message: getpid()가 R3을 기다리다가 깨어남
    }
    AppendToFile(prc,0,R3_F); //Store(R3,0) 대신에 파일에 어느 철학자 인지와 값을 저장해 놓음
    printf("%s bring R3 !! \n",prc->name);// print message: getpid()가  R3을 가져옴
    Release(lock);
}

void Put_R1(ProcessInfo * prc , Lock *lock, CondVar *c)
{
    Acquire(lock);
    AppendToFile(prc,1,R1_F); //Store(R1,1) 대신에 파일에 어느 철학자 인지와 값을 저장해 놓음
    if(Load(R1_Q) != 0)
        Signal(c);
    printf("%s Putdown R1 !\n",prc->name);
    Release(lock);
}
void Put_R2(ProcessInfo * prc , Lock *lock, CondVar *c)
{
    Acquire(lock);
    AppendToFile(prc,1,R2_F); //Store(R2,1) 대신에 파일에 어느 철학자 인지와 값을 저장해 놓음
    if(Load(R2_Q) != 0)
        Signal(c);
    printf("%s Putdown R2 !\n",prc->name);
    Release(lock);
}

void Put_R3(ProcessInfo * prc , Lock *lock, CondVar *c)
{
    Acquire(lock);
    AppendToFile(prc,1,R3_F); //Store(R3,1) 대신에 파일에 어느 철학자 인지와 값을 저장해 놓음
    if(Load(R3_Q) != 0)
        Signal(c);
    printf("%s Putdown R3 !\n",prc->name);
    Release(lock);
}
Phil_A(ProcessInfo *prc ,Lock *lock_r1, Lock *lock_r2 ,CondVar *con_r1 , CondVar *con_r2)
{	
    Take_R1(prc,lock_r1,con_r1);
    printf("%s start thinking .. \n",prc->name); // print message: getpid()가  생각을 시작함
    think(prc->thinking_time);
    printf("%s stop thinking .. \n",prc->name); // print message: getpid()가  생각을 멈춤
    Take_R2(prc,lock_r2,con_r2);
    printf("%s start eating .. \n",prc->name); // print message: getpid()가  먹기 시작함
    eat(prc->eating_time);
    printf("%s stop eating .. \n",prc->name); // print message: getpid()가  먹기를 멈춤
    Put_R1(prc,lock_r1,con_r1);
    Put_R2(prc,lock_r2,con_r2);
}

Phil_B(ProcessInfo *prc ,Lock *lock_r2, Lock *lock_r3 ,CondVar *con_r2 , CondVar *con_r3)
{	
    Take_R2(prc,lock_r2,con_r2);
    printf("%s start thinking .. \n",prc->name); // print message: getpid()가  생각을 시작함
    think(prc->thinking_time);
    printf("%s stop thinking .. \n",prc->name); // print message: getpid()가  생각을 멈춤
    Take_R3(prc,lock_r3,con_r3);
    printf("%s start eating .. \n",prc->name); // print message: getpid()가  먹기 시작함
    eat(prc->eating_time);
    printf("%s stop eating .. \n",prc->name); // print message: getpid()가  먹기를 멈춤
    Put_R2(prc,lock_r2,con_r2);
    Put_R3(prc,lock_r3,con_r3);
}

Phil_C(ProcessInfo *prc ,Lock *lock_r3, Lock *lock_r1 ,CondVar *con_r3 , CondVar *con_r1)
{	
    Take_R3(prc,lock_r3,con_r3);
    printf("%s start thinking .. \n",prc->name); // print message: getpid()가  생각을 시작함
    think(prc->thinking_time);
    printf("%s stop thinking .. \n",prc->name); // print message: getpid()가  생각을 멈춤
    Take_R1(prc,lock_r1,con_r1);
    printf("%s start eating .. \n",prc->name); // print message: getpid()가  먹기 시작함
    eat(prc->eating_time);
    printf("%s stop eating .. \n",prc->name); // print message: getpid()가  먹기를 멈춤
    Put_R3(prc,lock_r3,con_r3);
    Put_R1(prc,lock_r1,con_r1);
}
void main(void)
{
    //  $ ipcs                 // 남아 있는 세마포 확인
    //  $ ipcrm -s <semid>     // <semid>라는 세마포 제거

    //init Lock r1,r2,r3 
    Lock lock_r1;      // Lock for R1.txt
    Lock lock_r2;      // Lock for R2.txt
    Lock lock_r3;      // Lock for R3.txt
    key_t lock_r1_key = 0x200; 
    key_t lock_r2_key = 0x201; 
    key_t lock_r3_key = 0x202;
    initLock(&lock_r1,lock_r1_key);
    initLock(&lock_r2,lock_r2_key);
    initLock(&lock_r3,lock_r3_key);
    
    //init Convar r1,r2,r3
    CondVar con_r1;        // Con for R1
    CondVar con_r2;        // Con for R2
    CondVar con_r3;        // Con for R3
    key_t con_r1_key = 0x203;
    key_t con_r2_key = 0x204;
    key_t con_r3_key = 0x205;
    initCondVar(&con_r1,con_r1_key,R1_Q);
    initCondVar(&con_r2,con_r2_key,R2_Q);
    initCondVar(&con_r3,con_r3_key,R3_Q);
    
    ProcessInfo prc;  // 현재 process 정보
    //process has name , thinking time , eating time
    
    initProcessInfo(&prc,1,1,"Philo_C");
    //파일 초기화가 엮이지 않게 하기위해서, sleep 을 잠깐 걸어둡니다.
    sleep(1);
    //
    for(int i=0;i<20;i++)
        Phil_C(&prc,&lock_r3,&lock_r1,&con_r3,&con_r1);
}

