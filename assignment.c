#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t m =    PTHREAD_MUTEX_INITIALIZER;

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

int CustomerHamAte =0;
int CustomerFryAte =0;
int CustomerSodAte =0;


int getRand(int range){
    
    int randNum = rand() % range;
    
    return randNum;
    
}

void * cheff(void * arg){
    
    for (int i =0; i < 100; i++) {    //test loop
        
        sleep(1);
        printf ("Cheff cooking round: %i\n", i);
        pthread_mutex_lock(&m);
        
        //Chef stays waiting if CheffCooking is equal 0
        while(CheffCooking == 0)
            pthread_cond_wait(&Cheff, &m);
        int randNum = getRand(3);
        
        //Fry and Soda
        if ( randNum == 0 ) {
            CheffCooking = 0;
            have_Soda = 1;
            have_Fry = 1;
            //  printf("made Fry and Soda"); //debug
            pthread_cond_signal(&Fry);
            pthread_cond_signal(&Soda);
            
        }
        //Hamburger and Soda
        else if ( randNum == 1 ) {
            CheffCooking = 0;
            have_Soda = 1;
            have_Hamburger = 1;
            //   printf("made Hamburger and Soda"); //debug
            pthread_cond_signal(&Fry);
            pthread_cond_signal(&Soda);
        }
        //Hamburger and Fry
        else if ( randNum == 2 ) {
            CheffCooking = 0;
            have_Hamburger = 1;
            have_Fry = 1;
            //printf("Put Fry and Hamburger"); //debug
            pthread_cond_signal(&Fry);
            pthread_cond_signal(&Hamburger);
        }
        
        pthread_mutex_unlock(&m);
    }
    return 0;
}

void * FryEats(void * arg){
    
    while(1){
        pthread_mutex_lock(&m);
        while(have_Fry == 0)
            pthread_cond_wait(&Fry, &m);
        
        if(have_Soda == 1) {
            have_Soda = 0;
            CheffCooking = 0;
            HamburgerCustomerEating = 1;
            CustomerHamAte++;
            //printf("Fry guy eats"); //debug
            have_Hamburger = 0;
            have_Soda = 0;
            FryCustomerEating = 0;
            CheffCooking = 1;
        }
        if(have_Hamburger == 1) {
            have_Hamburger = 0;
            CheffCooking = 0;
            SodaCustomerEating = 1;
            CustomerSodAte++;
            //printf("Soda guy eats"); //debug
            have_Hamburger = 0;
            have_Soda = 0;
            FryCustomerEating = 0;
            CheffCooking = 1;
        }
        pthread_mutex_unlock(&m);
    }
    
    return 0 ;
}

void * SodaEats(void * arg){
    
    while(1) {
        pthread_mutex_lock(&m);
        while(have_Soda == 0)
            pthread_cond_wait(&Soda, &m);
        
        if(have_Fry == 1) {
            have_Fry = 0;
            CheffCooking = 0;
            HamburgerCustomerEating = 1;
            CustomerHamAte++;
            //printf("Ham guy eats"); //debug
            have_Hamburger = 0;
            have_Soda = 0;
            FryCustomerEating = 0;
            CheffCooking = 1;
        }
        if(have_Hamburger == 1) {
            have_Hamburger = 0;
            CheffCooking = 0;
            FryCustomerEating = 1;
            CustomerFryAte++;
            //printf("Fry guy eats"); //debug
            have_Hamburger = 0;
            have_Soda = 0;
            FryCustomerEating = 0;
            CheffCooking = 1;
        }
        pthread_mutex_unlock(&m);
    }
    
    return 0 ;
}

void * HamEats(void * arg){
    while(1){
        pthread_mutex_lock(&m);
        while(have_Hamburger == 0)
            pthread_cond_wait(&Hamburger, &m);
        
        if(have_Soda == 1) {
            have_Soda = 0;
            CheffCooking = 0;
            FryCustomerEating = 1;
            CustomerFryAte++;
            //printf("Fry guy eats"); //debug
            have_Hamburger = 0;
            have_Soda = 0;
            FryCustomerEating = 0;
            CheffCooking = 1;
        }
        if(have_Fry == 1) {
            have_Hamburger = 0;
            CheffCooking = 0;
            SodaCustomerEating = 1;
            CustomerSodAte++;
            //printf("Soda guy eats"); //debug
            have_Hamburger = 0;
            have_Soda = 0;
            FryCustomerEating = 0;
            CheffCooking = 1;
        }
        pthread_mutex_unlock(&m);
    }
    
    return 0 ;
}


int main()
{
    pthread_t Cheff_t, HamburgerCustomerFunction, FryCustomerFunction, SodaCustomerFunction;
    
    //random seed
    time_t t;
    srand((unsigned) time(&t));
    
    //create threads
    if (pthread_create(&Cheff_t,NULL,cheff,NULL) != 0) {
        err(1, "cheff_t");
    }
    
    if (pthread_create(&HamburgerCustomerFunction,NULL,HamEats,NULL) != 0) {
        err(1, "hamburger_t");
    }
    
    if (pthread_create(&FryCustomerFunction,NULL,FryEats,NULL) != 0) {
        err(1, "fry_t");
    }
    
    if (pthread_create(&SodaCustomerFunction,NULL,SodaEats,NULL) != 0) {
        err(1, "SodaCustomerFunction");
    }
    
    // join threads
    if (pthread_join(Cheff_t, NULL))
        err(1, "cheff_t");
    printf("Fry guy ate: %i\n", CustomerFryAte);
    printf("Soda guy ate: %i\n", CustomerSodAte);
    printf("Ham guy ate: %i\n", CustomerHamAte);
    if (pthread_join(HamburgerCustomerFunction, NULL))
        err(1, "HamburgerCustomerFunction");
    
    
    if (pthread_join(FryCustomerFunction, NULL))
        err(1, "FryCustomerFunction");
    
    
    if (pthread_join(SodaCustomerFunction, NULL))
        err(1, "SodaCustomerFunction");
    
    
}
