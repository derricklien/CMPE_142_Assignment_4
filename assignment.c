#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t m =    PTHREAD_MUTEX_INITIALIZER;

//actor
pthread_cond_t Chef =    PTHREAD_COND_INITIALIZER;
pthread_cond_t CustomerHamburger =    PTHREAD_COND_INITIALIZER;
pthread_cond_t CustomerFry =    PTHREAD_COND_INITIALIZER;
pthread_cond_t CustomerSoda =    PTHREAD_COND_INITIALIZER;

//resource
pthread_cond_t Hamburger =    PTHREAD_COND_INITIALIZER;
pthread_cond_t Fry =    PTHREAD_COND_INITIALIZER;
pthread_cond_t Soda =    PTHREAD_COND_INITIALIZER;

int have_Hamburger = 0, have_Fry = 0;
int have_Soda = 0;

int ChefCooking = 1, HamburgerCustomerEating = 0, SodaCustomerEating = 0, FryCustomerEating = 0, CustomerHamAte =0;
int CustomerFryAte =0;
int CustomerSodAte =0;


int getRand(int range){
    
    int randNum = rand() % range;
    
    return randNum;
    
}

void * chef(void * arg){
    
    for (int i =0; i < 100; i++) {    //test loop
        
        sleep(1);
        printf ("Chef cooking round: %i\n", i);
        pthread_mutex_lock(&m);
        
        //Chef stays waiting if ChefCooking is equal 0
        while(ChefCooking == 0)
            pthread_cond_wait(&Chef, &m);
        int randNum = getRand(3);
        
        switch (randNum) {
        case 0:
            ChefCooking = 0;
            have_Soda = 1;
            have_Fry = 1;
            //  printf("made Fry and Soda"); //debug
            pthread_cond_signal(&Fry);
            pthread_cond_signal(&Soda);
            break;
        case 1:
            ChefCooking = 0;
            have_Soda = 1;
            have_Hamburger = 1;
            pthread_cond_signal(&Hamburger);
            pthread_cond_signal(&Soda);
            break;
        case 2:
            ChefCooking = 0;
            have_Fry = 1;
            have_Hamburger = 1;
            //  printf("made Fry and Soda"); //debug
            pthread_cond_signal(&Fry);
            pthread_cond_signal(&Hamburger);
            break;
        default:
            break;
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
            ChefCooking = 0;
            HamburgerCustomerEating = 1;
            CustomerHamAte++;
            //printf("Fry guy eats"); //debug
            have_Hamburger = 0;
            have_Soda = 0;
            FryCustomerEating = 0;
            ChefCooking = 1;
        }
        if(have_Hamburger == 1) {
            have_Hamburger = 0;
            ChefCooking = 0;
            SodaCustomerEating = 1;
            CustomerSodAte++;
            //printf("Soda guy eats"); //debug
            have_Hamburger = 0;
            have_Soda = 0;
            FryCustomerEating = 0;
            ChefCooking = 1;
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
            ChefCooking = 0;
            HamburgerCustomerEating = 1;
            CustomerHamAte++;
            //printf("Ham guy eats"); //debug
            have_Hamburger = 0;
            have_Soda = 0;
            FryCustomerEating = 0;
            ChefCooking = 1;
        }
        if(have_Hamburger == 1) {
            have_Hamburger = 0;
            ChefCooking = 0;
            FryCustomerEating = 1;
            CustomerFryAte++;
            //printf("Fry guy eats"); //debug
            have_Hamburger = 0;
            have_Soda = 0;
            FryCustomerEating = 0;
            ChefCooking = 1;
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
            ChefCooking = 0;
            FryCustomerEating = 1;
            CustomerFryAte++;
            //printf("Fry guy eats"); //debug
            have_Hamburger = 0;
            have_Soda = 0;
            FryCustomerEating = 0;
            ChefCooking = 1;
        }
        if(have_Fry == 1) {
            have_Hamburger = 0;
            ChefCooking = 0;
            SodaCustomerEating = 1;
            CustomerSodAte++;
            //printf("Soda guy eats"); //debug
            have_Hamburger = 0;
            have_Soda = 0;
            FryCustomerEating = 0;
            ChefCooking = 1;
        }
        pthread_mutex_unlock(&m);
    }
    
    return 0 ;
}


int main()
{
    pthread_t Chef_t, HamburgerCustomerFunction, FryCustomerFunction, SodaCustomerFunction;
    
    //random seed
    time_t t;
    srand((unsigned) time(&t));
    
    //create threads
    if (pthread_create(&Chef_t,NULL,chef,NULL) != 0) {
        err(1, "chef_t");
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
    if (pthread_join(Chef_t, NULL))
        err(1, "chef_t");
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
