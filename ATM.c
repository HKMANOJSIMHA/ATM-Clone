/*C program to fullfill the SRS for the design of ATM machine*/

#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#define MAXN 64
#define MAXC 1024
/*Structure definition*/
struct accountDetails{
    char cardNum[17];
    int password;
    int month;
    int year;
    int daily_trans;
    char bankcode[11];
    long int balance;
    long int trans_limit;
    char block[1];
};
typedef struct accountDetails AD;
/*Function name:fncheckDigit
  Return type:int
  Parameters:char*
  Operation Performed : Check whether the parameter passed is a sequence of digits or not
*/
int fncheckDigit(char*Input){
    int i,count=0;
    for(i=0;Input[i]!='\0';i++){
        if(isdigit(Input[i])||Input[0]=='+'||Input[0]=='-'){
             count++;
        }
    }
    if(count==strlen(Input)){
        return 1;
    }
    else{
        printf("\nInvalid input please try again\n");
        return 0;
    }
}

/*Function name:fncheckCard
  Return type:int
  Parameters:AD*,char*,char*
  Operation Performed : Check whether the entered card is present in the database or not
*/

int fncheckCard(AD*input,char*card,char*file){
    FILE *fp = fopen (file, "r");

    if (!fp) { 
        perror ("file open failed");
        return 1;
    }

        char buf[MAXC],password[100],month[100],year[100],balance[100],total[100],limit[20];
        unsigned len, found = 0;
        card[strcspn (card, "\n")] = 0;                 

        while (fgets (buf, MAXC, fp)) {                
            if (strncmp (buf, card, 16) == 0) {         
                if (sscanf (buf, "%63[^|]|%63[^|]|%63[^|]|%63[^|]|%63[^|]|%63[^|]|%63[^|]|%63[^|]|%63[^|]|",
                                input->cardNum,input->bankcode,password,month,year,balance,total,input->block,limit) == 9) {
                    input->password=atoi(password);
                    input->month=atoi(month);
                    input->year=atoi(year);
                    input->balance=atoi(balance);
                    input->daily_trans=atoi(total); 
                    input->trans_limit=atoi(limit);     
                    found = 1; 
                    break;     
                }
            }
        }
        
        if (!found){
            printf("\nCard number not found please try again\n");
            return 0;
        } 
        else{
             return 1;
         }
    fclose (fp);  
}

/*Function name:fnCheckBankCode
  Return type:int
  Parameters:char*,char*
  Operation Performed : Check whether the entered bank code is matched with the stored bank code
*/
int fnCheckBankCode(char*input,char*code){
    if(strcmp(input,code)==0){
        return 1;
    }
    else{
        printf("\nOops! bad bank code please try again with valid code\n");
        return 0;
    }
}

/*Function name:fnCheckCardValidity
  Return type:int
  Parameters:int,int
  Operation Performed : Check whether the card has expired or not 
*/

int fnCheckCardValidity(int month,int year){
    time_t t = time(NULL);
    struct tm tm =*localtime(&t);
    if(year>tm.tm_year+1900){
        return 1;
    }
    else if(year==tm.tm_year+1900){
        if(month<tm.tm_mon+1){
        printf("\nOops your card has expired please visit your bank\n\n");
        return 0;
        }
        else{
        return 1;
        }
    }
    else{
        printf("\nOops your card has expired please visit your bank\n\n");
        return 0;
    }
}

/*Function name:fngetPin
  Return type:int
  Parameters:AD
  Operation Performed : To check whether the entered pin is correct or not
*/

int fngetPin(AD input){
    int pin,count=0;
    while(count!=3){
        scanf("%d",&pin);
        count++;
        if(input.password==pin){
            return 1;
        }
        else{
            if(count!=3)
            printf("\nOops! bad password please try again:");
        }
    }
    return 0;
}

/*Function name:fnUpdateDatabase
  Return type:void
  Parameters:AD,char*
  Operation Performed : To update the database after each transaction
*/

void fnUpdateDatabase(AD input,char*file){
    FILE * fPtr;
    FILE * fTemp;
    
    char buffer[MAXC];
    int line, count;

    fflush(stdin);
    fPtr  = fopen(file, "r");
    fTemp = fopen("replace.tmp", "w"); 
    if (fPtr == NULL || fTemp == NULL)
    {
        printf("\nUnable to open file.\n");
        printf("Please check whether file exists and you have read/write privilege.\n");
        exit(EXIT_SUCCESS);
    }
    count = 0;
    while ((fgets(buffer, MAXC, fPtr)) != NULL)
    {

         if (strncmp (buffer,input.cardNum, 16) == 0)
            fprintf(fTemp,"%s|%s|%d|%d|%d|%ld\t|%d\t|%s|%ld\n",input.cardNum,input.bankcode,input.password,input.month,input.year
            ,input.balance,input.daily_trans,input.block,input.trans_limit);
        else
            fputs(buffer, fTemp);
    }
    fclose(fPtr);
    fclose(fTemp);
    remove(file);
    rename("replace.tmp", file);
}

/*Function name:fnBlockCard
  Return type:void
  Parameters:AD,char*
  Operation Performed : To block a card and update the database to showcase that card is blocked
*/

void fnBlockCard(AD input,char*file){
     FILE * fPtr;
    FILE * fTemp;
    
    char buffer[MAXC];
    int line, count;

    fflush(stdin);
    fPtr  = fopen(file, "r");
    fTemp = fopen("replace.tmp", "w"); 
    count = 0;
    while ((fgets(buffer, MAXC, fPtr)) != NULL)
    {

         if (strncmp (buffer,input.cardNum, 16) == 0)
            fprintf(fTemp,"%s|%s|%d|%d|%d|%ld\t|%d\t|B|%ld\n",input.cardNum,input.bankcode,input.password,input.month,input.year
            ,input.balance,input.daily_trans,input.trans_limit);
        else
            fputs(buffer, fTemp);
    }
    fclose(fPtr);
    fclose(fTemp);
    remove(file);
    rename("replace.tmp", file);
}

/*Function name:fnCheckBlockStatus
  Return type:int
  Parameters:AD
  Operation Performed : To check whether the entered card is blocked or not
*/


int fnCheckBlockStatus(AD input){
    if(strcmp(input.block,"B")==0){
        printf("\nSorry your card has been blocked please visit your bank\n");
        return 0;
    }
    else{
        return 1;
    }
}

/*Function name:fnGetCurrentTime
  Return type:char*
  Parameters:no parameters
  Operation Performed : To fetch the current date and time
*/

char*fnGetCurrentTime(){
    time_t current_time;
    char* c_time_string;

    current_time = time(NULL);   
    c_time_string = ctime(&current_time);
    return c_time_string;
}

/*Function name:fnAddTransactionEntry
  Return type:void
  Parameters:AD,long int
  Operation Performed : To add the transaction details to the respective bank file
*/

void fnAddTransactionEntry(AD input,long int amount){
    FILE *fPtr;
    char*time;
    char bank[3];
    for(int i=0;i<3;i++){
        bank[i]=input.bankcode[i];
    }
    fPtr = fopen(bank, "a");
    time=fnGetCurrentTime();
    fprintf(fPtr,"%s|%s|%ld\t|%ld\t|%s",input.cardNum,input.bankcode,amount,input.balance,time);
    fclose(fPtr);

}

/*Driver function*/

int main(){
    int atmAmt=10000,DailyLmt=10000,tranLmt=5000,minCash=500,flag=0,track=0,choice=0,password;
    char cardnum[16],bankcd[11],Witamount[30];
    long int amount;
    AD input;
    
    /*Opening the database file*/
    
    FILE*dataBase=fopen("Database.txt","r");
    for(;;){
        printf("\n==================Welcome to SDMCET ATM==========================\n\n");
        /*To check whether the atm is out of money or not*/
        
        if(atmAmt==0){
            printf("This machine is out of money and cannot perform any transaction\n");
            break;
        }
        else{
            flag=0;track=0;
            while(1){
            	printf("\nEnter the 16 digit valid card number:");
                scanf("%s",cardnum);
                flag=fncheckDigit(cardnum);
                if(flag==0){ //if the entered input does not contain all digits
                    continue;
                }
                else{
                    track=fncheckCard(&input,cardnum,"Database.txt");
                    if(track==1){ //If the card number is present in the database
                        break;
                    }
                }
            }
            track=fnCheckBlockStatus(input); //To check whether the card is not blocked 
            if(track==0){
                continue;
            }

            flag=fnCheckCardValidity(input.month,input.year); //To check whether the card is not expired
            if(flag==0){
                   continue;  
             }
            
           while(1){
           
            printf("\nEnter your bank code:");
            scanf("%s",bankcd);
            track=fnCheckBankCode(input.bankcode,bankcd); //Check for proper bankcode
            if(track==0){
                continue;
            }
            else{
            	break;
            }
           }

            printf("\nEnter your 4 digit ATM pin:");
            track=fngetPin(input);		//check for correct pin
            if(track==0){
                printf("\nYou have attempted maximum number of time your card has been blocked\n");
                printf("please visit your bank to unblock the card\n\n");
                fnBlockCard(input,"Database.txt");
                continue;
            }
            
            printf("\nAuthorisation successfull\n");
            
             printf("\nEnter the amount to be withdrawn:");
             scanf("%ld",&amount);
            if(input.daily_trans+amount<=input.trans_limit){
                if(amount<=tranLmt && amount%100==0 && amount<=atmAmt && amount<=input.balance && amount>0){
                    input.balance=input.balance-amount;
                    atmAmt=atmAmt-amount;
                    input.daily_trans=input.daily_trans+amount;
                    fnUpdateDatabase(input,"Database.txt");
                    fnAddTransactionEntry(input,amount);
                    printf("\nTransaction successfull\n");
                    printf("Amount withdrawn is:%ld\n",amount);
                    while(1){
                     	printf("Do you want to display the balance on the screen\nPress:\n1-Yes\n0-No\n");
                    	scanf("%d",&choice);
                    	if(choice==1){
                        printf("Available balance is:%ld\n",input.balance);
                        printf("Thank you for banking with us.Have a nice day!\n");
                    	break;
                    }
                    else if(choice==0){
                        printf("Thank you for banking with us.Have a nice day!\n");
                        break;
                    }
                    else{
                     printf("\nSorry invalid input\n");
                    }
                    }
                }
                else{
                    
                    if(amount>input.balance){
                        printf("\nSorry Insufficient funds\n");
                        continue;
                    }
                    printf("\nRequested amount cannot be dispensed\n");
                    continue;
                }

            }
            else{
                printf("\nYou have exceeded your daily transaction limit\n");
                continue;
            }
            
        }

    }
}
