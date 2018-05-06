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

#define AR_F "ar.txt"
#define AW_F "aw.txt"
#define WR_F "wr.txt"
#define WW_F "ww.txt"
#define R_F "result.txt" // result파일을 따로만들어, 실행 시간을 동기화해 저장합니다.

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
   int sleep_time;//실행전 몇초동안 sleep 하는지
   int act_time;// 실행을 몇초동안 하는지
   int c_time;//현재시간을 파악합니다.
   pid_t pid;
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

initProcessInfo(ProcessInfo * p, int sleep_time , int act_time)
{
   p->sleep_time = sleep_time;
   p->act_time = act_time;
   p->pid = getpid();
   reset(R_F); // for check time and last output
   reset(AW_F);
   reset(AR_F);
   reset(WW_F);
   reset(WR_F);
}

int get_time(ProcessInfo * p) //result 파일에서 가장 최근에 끝난 프로세스에 찍힌 시간을 들고옵니다.
{
    int current_time = Load(R_F); // Read Result file (current time)
    return current_time;
}
AppendToFile(ProcessInfo * p , int second_val_file , int third_val_file , char * filename)
{
    FILE * in = fopen(filename,"a");
    fprintf(in,"\n%d %d %d",p->pid,second_val_file,third_val_file); // 각자 화일에 맞는 정보를 입력해줍니다.
    fclose(in);
}

enQueue_Process(ProcessInfo *p ,  CondVar * c , char * filename)
{
    add(c->queueLength,1);//enqueue
    int total_number = Load(filename); // check the total waiting Read or write Process number
    int current_time = get_time(p); // Read Result file (current time)
    //add WR file
    AppendToFile(p,current_time,total_number+1, filename); // Wait 파일에 넣어줍니다.
}

deQueue_Process(ProcessInfo *p ,  CondVar * c , char *filename)
{
    sub(c->queueLength,1);
    int total_number = Load(filename);
    int current_time = get_time(p); // Read Result file time
    AppendToFile(p,current_time,total_number-1, filename);//Wait file에서 꺼내줍니다
}

Wait(ProcessInfo *p, CondVar *c, Lock *lock, char* filename)
{
   // enQueue
   enQueue_Process(p,c,filename);
   Release(lock);
   Ac_Con(c);
   Acquire(lock);
   deQueue_Process(p,c,filename);
}

Signal(CondVar *c)
{
    Re_Con(c);
}

Broadcast(CondVar *c)
{
    int number = Load(c->queueLength);
    for(int i=0;i<number;i++)
        Re_Con(c);
}

int check_AW_AR()
{
    int aw,ar;
    aw = Load(AW_F);
    ar = Load(AR_F);
    return ((aw+ar)>0?1:0);
}
int check_AW_WW()
{
    int aw,ww;
    aw = Load(AW_F);
    ww = Load(WW_F);
    return ((aw+ww)>0?1:0);
}

write(ProcessInfo *p , Lock *lock, CondVar * reader, CondVar * writer)
{
    Acquire(lock);   // lock.Acquire()
    while(check_AW_AR())
    {
        Wait(p,writer,lock,WW_F);
    }
    //write down Active Reader file
    
    AppendToFile(p,get_time(p),1,AW_F); //AW로 넣어줍니다.

    if(p->sleep_time > get_time(p)) // 현재시간을 찾습니다.
        p->c_time = p->sleep_time;
    else
       p->c_time = get_time(p);

    Release(lock);
    
    sleep(p->act_time); // 문서 작성중...
    
    Acquire(lock);  // lock.Acquire()
    
    //write result file (time)
    AppendToFile(p,get_time(p),p->c_time + p->act_time,R_F);
        
    //finish write, write in AW file
    AppendToFile(p,get_time(p),0,AW_F);
    
    if(Load(WW_F)>0)
        Signal(writer);
    else if(Load(WR_F)>0)
        Broadcast(reader);
    Release(lock);
}

read(ProcessInfo *p , Lock *lock, CondVar * reader, CondVar * writer)
{
    Acquire(lock);   // lock.Acquire()
    while(check_AW_WW())
    {
        Wait(p,reader,lock,WR_F);
    }
    //write down Active Reader file
    AppendToFile(p,get_time(p),Load(AR_F)+1,AR_F);
    
    if(p->sleep_time > get_time(p))
        p->c_time = p->sleep_time;
    else
       p->c_time = get_time(p);

    Release(lock);
    
    sleep(p->act_time);
    
    Acquire(lock);   // lock.Acquire()
    
    //write result file (time)
    AppendToFile(p,get_time(p),p->c_time + p->act_time,R_F);
    
    //finish read, write in AR file
    AppendToFile(p, get_time(p) ,Load(AR_F)-1,AR_F);
    
    if(Load(AR_F) == 0 && Load(WW_F)>0)
        Signal(writer);
    Release(lock);
}
void main(int arg , char * argv[]) //this is for reader (sleep time , active time)
{
    if(arg != 3)
    {
        printf("please input sleep time and active time !!\n");
        return;
    }

    int sleep_time,act_time;
    sleep_time = atoi(argv[1]);
    act_time = atoi(argv[2]);

    key_t semkey = 0x200;// 시스템에서 세머포어를 식별하는 집합 번호  
    key_t r_key = 0x201;
    key_t w_key = 0x202;

    //  $ ipcs                 // 남아 있는 세마포 확인
    //  $ ipcrm -s <semid>     // <semid>라는 세마포 제거

    int semid; 
    Lock lock;      // Lock
    CondVar reader; // reader 용 queue !
    CondVar writer; // writer 용 queue !
    ProcessInfo p;  // 현재 process 정보

    initCondVar(&reader, r_key, "reader.txt");
    initCondVar(&writer, w_key, "writer.txt");
    initLock(&lock,semkey);
    initProcessInfo(&p,sleep_time,act_time);

    //sleep
    printf("\nprocess %d before critical section, sleep now... \n",p.pid);
    sleep(p.sleep_time);
    
    //critical section
    printf("process %d in critical section\n",p.pid);
    read(&p,&lock,&reader,&writer); // critical sectio
    printf("process %d leaving critical section\n", p.pid);
    
    //end
    sleep(1);
    printf("process %d exiting\n",p.pid);
    exit(0);
}

