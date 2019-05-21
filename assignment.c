#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t m =    PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t EatingCustomer =    PTHREAD_MUTEX_INITIALIZER;

//actor
pthread_cond_t Cheff =    PTHREAD_COND_INITIALIZER;
pthread_cond_t CustomerHamburger =    PTHREAD_COND_INITIALIZER;
pthread_cond_t CustomerFry =    PTHREAD_COND_INITIALIZER;
pthread_cond_t CustomerSoda =    PTHREAD_COND_INITIALIZER;

//resource 
pthread_cond_t Hamburger =    PTHREAD_COND_INITIALIZER;
pthread_cond_t Fry =    PTHREAD_COND_INITIALIZER;
pthread_cond_t Soda =    PTHREAD_COND_INITIALIZER;

int have_Hamburger = 0;
int have_Fry = 0;
int have_Soda = 0;

int CheffCooking = 1;
int HamburgerCustomerEating = 0;
int SodaCustomerEating = 0;
int FryCustomerEating = 0;

int getRand(int range){
    
    int randNum = rand() % range;
    
    return randNum;
    
}

void * cheff(void * arg){
    
    for (int i =0; i < 5; i++) {    //test loop
        
        sleep(1);
        
        pthread_mutex_lock(&m);
        
        //Chef stays waiting if CheffCooking is equal 0
        while(CheffCooking == 0)
            pthread_cond_wait(&Cheff, &m);
        
        
      //  printf("-------------------------------\n");
        
        int randNum = getRand(3);
        
        //Fry and Soda
        if ( randNum == 0 ) {
   
            
        }
        //Hamburger and Soda
        else if ( randNum == 1 ) {

        }
        //Hamburger and Fry
        else if ( randNum == 2 ) {
            
        }
        
        pthread_mutex_unlock(&m);
    }
    return 0;
}

void * pusher_Fry(void * arg){
    
    return 0 ;
}

void * pusher_Soda(void * arg){
    
    return 0 ;
}

void * pusher_Hamburger(void * arg){
  
    return 0 ;
}

void * EatingCustomer_Hamburger(void * arg){
    
    return 0;
}

void * EatingCustomer_Fry(){
    
    return 0;
}

void * EatingCustomer_Soda(){
    
    return 0;
}

int main()
{
    pthread_t Cheff_t, EatingCustomer_Hamburger_t, EatingCustomer_Fry_t, EatingCustomer_Soda_t, pusher_Hamburger_t, pusher_Fry_t, pusher_Soda_t;
    
    //random seed
    time_t t;
    srand((unsigned) time(&t));
    
    //create threads
    if (pthread_create(&Cheff_t,NULL,cheff,NULL) != 0) {
        err(1, "cheff_t");
    }   
    
    if (pthread_create(&pusher_Hamburger_t,NULL,pusher_Hamburger,NULL) != 0) {
         err(1, "hamburger_t");
    }
    
    if (pthread_create(&pusher_Fry_t,NULL,pusher_Fry,NULL) != 0) {
        err(1, "fry_t");
    }
    
    if (pthread_create(&pusher_Soda_t,NULL,pusher_Soda,NULL) != 0) {
         err(1, "pusher_soda_t");
    }
    
    if (pthread_create(&EatingCustomer_Hamburger_t,NULL,EatingCustomer_Hamburger,NULL) != 0) {
         err(1, "eating_customer_hamburger_t");
    }
    
    if (pthread_create(&EatingCustomer_Fry_t,NULL,EatingCustomer_Fry,NULL) != 0) {
        err(1, "eating_customer_fry_t");
    }
    
    if (pthread_create(&EatingCustomer_Soda_t,NULL,EatingCustomer_Soda,NULL) != 0) {
        err(1, "customer_soda_t");
    }
    
    // join threads
    if (pthread_join(Cheff_t, NULL)) {
        err(1, "cheff_t");    
    }
    if (pthread_join(pusher_Hamburger_t, NULL){
        err(1, "pusher_hamburger_t");
    }

    if (pthread_join(pusher_Fry_t, NULL){
        err(1, "pusher_fry_t");
    }

    pthread_join(pusher_Soda_t, NULL){
        err(1, "pusher_soda_t");
    }

    pthread_join(EatingCustomer_Hamburger_t, NULL){
        err(1, "eating_customer_hamburger_t");
    }

    pthread_join(EatingCustomer_Fry_t, NULL){
        err(1, "eating_customer_fry_t");
    }

    pthread_join(EatingCustomer_Soda_t, NULL){
        err(1, "eating_customer_soda_t");
    }
    
}
