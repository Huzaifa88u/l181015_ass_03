#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <semaphore.h>
int potent_c_pat;
sem_t corona_patient;
sem_t flu_patient;
void* corona_test(void* val);
int flu(int val);
int corona(int val);
int main()
{
   pthread_t *thread;
   sem_init(&corona_patient,0,1);
   sem_init(&flu_patient,0,1);
  printf("Enter the number of corona patients: ");
  int n=0;
  scanf("%d",&n);
  thread = (pthread_t *) malloc (n*sizeof(pthread_t));
 
  potent_c_pat=potent_c_pat+n;
  printf("number of patients: %d",potent_c_pat);
  
  
      printf("\nChecking for corona: \n");
     for (int i=0;i<n;i++)
  {         
  	pthread_create(&thread[i], NULL, &corona_test, (void*)&potent_c_pat);
  	pthread_join(thread[i],NULL);
  
  }
  
  sem_destroy(&corona_patient);
  sem_destroy(&flu_patient);
  
  
  return 0;
}
void* corona_test(void* val)
{
   int value=*(int*)val;
   int check=1;
  
  

   for (int i=1;i<=value;i++)
   {
   if (i%2==0)
   {
   check=corona(i);
      if (check==0)
      {
          check= corona(i);
          if (check==0)
          {
             printf("When Corona test Negative two times, now potential patients are: %d \n",potent_c_pat);
             sem_wait(&corona_patient);
          }
      }
      else
      {
       potent_c_pat--;
       printf("Potential Patients: %d \n",potent_c_pat);
        sem_post(&corona_patient);
       
      }
      sem_post(&corona_patient);
    }
    else
    {
    int chk=0;
    chk=flu(i);
       if (chk==1)
      {
        potent_c_pat--;
        printf("Potential Patients: %d \n",potent_c_pat);
        sem_post(&flu_patient);
      }
      
    
    }
   
   }
   pthread_exit(NULL);
}
int flu(int val)
{

 printf("Thankfully its just flu: \n");
 return 1;
}
int corona(int val)
{
  if (val%3==0)
  {
  printf("You are corona positive: \n");
  return 1;
  }
  else 
  {
    printf("You are corona negative: \n");
    return 0;
    
  }

}

