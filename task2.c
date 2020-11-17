#include<stdio.h>
#include<semaphore.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/shm.h>
#include<fcntl.h>

int main()
{    

    
   
    sem_t *sem1 = sem_open ("sem1", O_CREAT | O_EXCL, 0644, 1);
    sem_t *sem2 = sem_open ("sem2", O_CREAT | O_EXCL, 0644, 0);
    sem_t *sem3 = sem_open ("sem3", O_CREAT | O_EXCL, 0644, 0);
    sem_t *sem4 = sem_open ("sem4", O_CREAT | O_EXCL, 0644, 0);
    sem_t *sem5 = sem_open ("sem5", O_CREAT | O_EXCL, 0644, 0);


    key_t key= shmget(12329, 1024, IPC_CREAT | IPC_EXCL | 0644);
    char* buffer= (char*) shmat(key, NULL, 0);
   
    key_t key1= shmget(12339, 1024, IPC_CREAT | IPC_EXCL | 0622);
    char* buffer1= (char*) shmat(key1, NULL, 0);
    
        pid_t id1;
        pid_t id2;
        pid_t id3;
        pid_t id4;

        id1=fork();
        id2=fork();
        id3=fork();
        id4=fork();
       //Process A
        if(id1==0 && id2!=0 && id3!=0 && id4!=0){
       
        sem_wait(sem1);    
         FILE* file=fopen("file1.txt","r");
         printf("This is process A\n"); 
        if(file)
        {
         char temp[15];
         int i=0;
         
          while(i<10){
          temp[i]=fgetc(file);
          i++;
                }
          for(int j=0;j<10;j++)
          buffer[j]=temp[j];
         fclose(file);
        }
       else
            printf("cannot open file: ");
         
           
        sem_post(sem2);
       }

//Process B
       else if(id2==0 && id1!=0 && id3!=0 && id4!=0){
sem_wait(sem2);  
        FILE* file=fopen("file2.txt","r");
         printf("This is process B\n");
        if(file){
         char temp[15];
         int i=0;
         
          while(i<10){
               temp[i]=fgetc(file);
               i++;  
             }
            int k=0;
          for(int j=10;j<20;j++,k++)
          buffer[j]=temp[k];
         fclose(file);
        }
       else
            printf("cannot open file!! ");      
       
      
       sem_post(sem3);
        }
   //process C
       else if(id3==0 && id2!=0 && id1!=0 && id4!=0){
        sem_wait(sem3);  
        for (int i=0;i<20;i++){
           buffer1[i]=buffer[i];    
       }
       printf("This is process C\n");
       sem_post(sem4);
        }
       //process D
       else if(id4==0 && id2!=0 && id3!=0 && id1!=0){
        sem_wait(sem4);
        printf("This is process D\n");
          printf("The elements of buffer 1 are: ");    
          printf("%s", buffer1);
       
            
        sem_post(sem5);
     }
     
     //parent process
     
       else if(id4!=0 && id2!=0 && id3!=0 && id1!=0)
       {
       
        sem_wait(sem5);
      
        shmdt(buffer);
        shmctl(key, IPC_RMID, NULL);
        shmdt(buffer1);
        shmctl(key1, IPC_RMID, NULL);
        sem_unlink ("sem1");  
        sem_close(sem1);
        sem_unlink ("sem2");  
        sem_close(sem2);
        sem_unlink ("sem3");  
        sem_close(sem3);
        sem_unlink ("sem4");  
        sem_close(sem4);
        sem_unlink ("sem5");  
        sem_close(sem5);
        return 0;
        
      }
}


