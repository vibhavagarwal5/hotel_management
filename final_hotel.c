#include <stdio.h>
#include <string.h>

//contructing the structure for the room with details like name,room type and from and to dates of booking
struct room
{
    char name[100];
    int num;
    int fd,fm,fy,td,tm,ty;
    char status[20];
}a,a2;

//constructing the structure for the user credentials 
struct details
{
    char name[100];
    char password[100];
}user;

//constructing structure for the dates
struct date
{
    int d,m,y;
}b,r;


/*__________________________________________________________________________________________________________________*/


//making a function for counting the no of days between the from and to dates of booking provided by the user
int days (int fd, int fm, int fy, int td, int tm, int ty)
{
    if (fm==tm && fy==ty)
    {
        return td-fd;
    } 
    else if (fm!=tm || fy!=ty)
    {
        int day=0;
        while(fd!=td || fm!=tm || fy!=ty)
        {   
            if (fd==31 && (fm==1 || fm==3 || fm==5 || fm==7 || fm==8 || fm==10 ))
            {
                fm++;
                fd=1;
            }
            else if (fd==30 && (fm==4 || fm==6 || fm==9 || fm==11 ))
            {
                fm++;
                fd=1;
            }
            else if (fd==29 && fm==2 && ((fy%4==0 && fy%100!=0) || (fy%400==0)))
            {
                fm++;
                fd=1;
            }
            else if (fd==28 && fm==2 && !((fy%4==0 && fy%100!=0) || (fy%400==0)))
            {
                fd=1;
                fm=3;   
            }
            else if (fd==31 && fm==12)
            {
                fd=1;
                fm=1;
                fy++;
            }   
            else 
            {
                fd++;
            }
            day++;
        }
        return day;
    }
}

// compare function for two dates(from and to of database and from and to provided by the user)
int compare(int fd1,int fm1,int fy1, int td1,int tm1,int ty1,int fd2,int fm2,int fy2,int td2,int tm2,int ty2 )
{
    int flag=0;
    if((fd1==fd2) && (fm1==fm2) && (fy1==fy2) && (td1==td2) && (tm1==tm2) && (ty1==ty2))
    {
        return 0;
    }

    if(fy2>ty1)
    {
        flag=1;
        return 1;
    }
    else if(fm2>tm1 && fy2>=ty1)
    {
        flag=1;
        return 1;
    }
    else if(fd2>=td1 && fm2>=tm1 && fy2>=ty1)
    {
        flag=1;
        return 1;
    }
    else if(ty2<fy1 && flag==0)
    {
        return 1;
    }
    else if(tm2<fm1 && flag==0 && ty2<=fy1)
    {
        return 1;
    }
    else if(td2<=fd1 && flag==0 && ty2<=fy1 && tm2<=fm1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//checking if the "to" date is after the "from" date
int checkdate(int fd, int fm, int fy, int td, int tm, int ty)
{
    if(tm>12 || tm<=0 || td <= 0 || td>31 || fm>12 || fm<=0 || fd <= 0 || fd>31)
    {
        return 1;
    }
    else if((( tm == 4 || tm == 6 || tm == 9 || tm == 11) && td>30) || ((fm == 4 || fm == 6 || fm == 9 || fm == 11) && fd>30))
    {
        return 1;
    }
    else if((tm == 2 && td > 29 && ((ty%4 == 0 && ty%100 != 0) || ty%400 == 0)) || (tm == 2 && td > 28 && !((ty%4 == 0 && ty%100 != 0) || ty%400 == 0)))
    {
        return 1;
    }
    else if(fy > ty)
    {
        return 1;
    }
    else if(fy == ty)
    {
        if(fm>tm)
        {
            return 1;
        }
        else if(fm == tm)
        {
            if(fd>=td)
            {
                return 1;
            }
        }
    }
    return 0;
}

//checking all the room of the type the user wanted, between the from and to dates and if available then alloted
int available(char name2[],int room,int fd,int fm,int fy,int td,int tm,int ty,char status2[])
{
        FILE *q;
        int f=0,flag1=0,d1,m1,y1,d2,m2,y2,i=0,j,g=0;
        q=fopen("database","r");
        do
        {
            fscanf(q,"%s : Room no. %d from %d %d %d to %d %d %d : %s",a2.name,&a2.num,&a2.fd,&a2.fm,&a2.fy,&a2.td,&a2.tm,&a2.ty,a2.status);
            if(a2.num==room)
            {
                if(compare(a2.fd,a2.fm,a2.fy,a2.td,a2.tm,a2.ty,fd,fm,fy,td,tm,ty)==0)
                {
                    if(strcmp(a2.status,"Booked") == 0)
                    {
                        return 0;
                    }
                }
                flag1=1;
            }
        }
        while(!feof(q));

        if(flag1==1)
        {
            fclose(q);
            return 1;
        }
}

//checking the room if its available or not, if available then alloted 
int allot(char name1[],int type,int fd,int fm,int fy,int td,int tm,int ty,char status1[])
{
    int i;
    for(i=(type*100)+1;i<(type*100)+10;i++)
    {
        if(available(name1,i,fd,fm,fy,td,tm,ty,status1)==1)
        {
            return i;
        }
    }
    return 0;
}

//function to get the choice of "valid" type of room that the user wants
int getchoice()
{
    int choice=0;
    printf("\nEnter Type of Room(Type 1 to 6):");
    scanf("%d",&choice);
    if(choice>6 || choice<1)
    {
        printf("\nChoice you entered is invalid.Enter a valid choice.\n\n");
        getchoice();
    }
    else
    {
        return choice;
    }
}

//function to give the user an another chance of trying some other type of room since it may not be available
int printroom(int choice)
{
    int choice1,num=0;
    num=allot(a.name,choice,a.fd,a.fm,a.fy,a.td,a.tm,a.ty,a.status);
    if(num==0)
    {  
        int cont;
        printf("\nSorry No room available of this type!\n\nEnter 1 to try another type, 0 to exit to main menu: ");
        scanf("%d",&cont);
        if (cont == 1)
        {
            printf("\n\nTry another type: ");
            choice1 = getchoice();
            printroom(choice1);
        }
        else
        {
            return -2;
        }
    }
    else
    {
        return num;
    }
}

//checking if the entered date("from" and "to" dates) is correct or not
struct date check() 
{
    printf("Enter date in format dd mm yyyy: ");
    scanf("%d %d %d",&b.d,&b.m,&b.y);
    if(b.m>12 || b.m<=0 || b.d <= 0 || b.d>31) 
    {
        printf("Sorry the Date you entered is invalid please enter again\n\n");
        check();
    }
    else if((( b.m == 4 || b.m == 6 || b.m == 9 || b.m == 11) && b.d>30))
    {
        printf("Sorry the Date you entered is invalid please enter again\n\n");
        check();
    }
    else if((b.m == 2 && b.d > 29 && ((b.y%4 == 0 && b.y%100 != 0) || b.y%400 == 0)) || (b.m == 2 && b.d > 28 && !((b.y%4 == 0 && b.y%100 != 0) || b.y%400 == 0)))
    {
        printf("Sorry the Date you entered is invalid please enter again\n\n");
        check();
    }
    return b;
}

//booking rooms taking the type of room,no of such rooms and dates for the booking
void book()
{
    FILE *p,*q;
    int b,n,i,count=0,flag1;
    long long int fare = 0;
    int price[6]={2500,2000,3500,3000,4500,5000};
    char ipassword[100];
    FILE *pass;
    do
    {
        flag1 = 0;
        do
        {
        	flag1 = 0;
		    printf("\nEnter Name(Without spaces): ");

            //checking if the name has space in between or not
		    scanf(" %[^\n]s",a.name);
		    for(i = 0; i<strlen(a.name); i++)
		    {
		    	if(a.name[i] == ' ')
		    	{
		    		flag1 = 1;
		    		printf("Invalid Username");
		    		break;
		    		
		    	}
			}
		}
		while (flag1 == 1);
        
        //Entering the password for a new user or entering the password already set by the registered user
        pass = fopen("user credentials","r");
        do
        {
            fscanf(pass,"%s : %s",user.name,user.password);
            if( strcmp(user.name,a.name) == 0 )
            {
                printf("\nYou are already an existing user\n\nEnter your password: ");

                //checking if the password has space in between or not
                scanf(" %[^\n]s",ipassword);
                for(i = 0; i<strlen(ipassword); i++)
        		{
		        	if(ipassword[i] == ' ')
        			{
        				printf("Invalid password");
        				flag1 = 2;
        				break;
        			} 
				}
				if (flag1 == 2)
				{
					flag1 = -1;
					break;
				}
                if(strcmp(user.password,ipassword) == 0)
                {
                    flag1 = 1;
                }
                else
                {
                    flag1 = -1;
                    printf("\nInvalid password, You will have to make a new account with a different name\n\n");
                }
                break;
            }
        }
        while( !feof(pass) );
        fclose(pass);
    }
    while(flag1 == -1);

    if(flag1 == 0)
    {       
    	do
    	{
    		flag1 = 0;
		    printf("You are a new user, please enter a password for your account: ");

            //checking if the password has space in between or not
		    scanf(" %[^\n]s",ipassword);
		    for(i = 0; i<strlen(ipassword); i++)
		    {
				if(ipassword[i] == ' ')
		    	{
		    		printf("Invalid password");
		    		flag1 = 1;
		    		break;
		    	} 
			}
		}
		while(flag1 == 1);
        pass = fopen("user credentials","a");
        fprintf(pass,"%s : %s\n",a.name,ipassword);
        fclose(pass);
        flag1 = 1;
    }

    //entering the dates for booking 
    if (flag1 == 1)
    {
        printf("\nFROM date\n");
        r=check();
        a.fd=r.d;
        a.fm=r.m;
        a.fy=r.y;
        printf("\nTO date\n");
        r=check();
        a.td=r.d;
        a.tm=r.m;
        a.ty=r.y;
        b=checkdate(a.fd,a.fm,a.fy,a.td,a.tm,a.ty);
        while(b==1)
        {
            printf("\nThe dates you entered are not in proper order.Try again\n\n");
            printf("\nFROM date\n");
            r=check();
            a.fd=r.d;
            a.fm=r.m;
            a.fy=r.y;
            printf("\nTO date\n");
            r=check();
            a.td=r.d;
            a.tm=r.m;
            a.ty=r.y;
            b=checkdate(a.fd,a.fm,a.fy,a.td,a.tm,a.ty);
        }

        //entering the details regarding the type and no of rooms
        printf("\n\nHere are the type of rooms and their maximum occupancy with tariffs:\n  Type         Max Occupancy    Tariff(/Night)\n");
        printf("1.Single AC          1             Rs.2500\n2.Non AC Single      1             Rs.2000\n3.Double AC          2             Rs.3500\n4.Double Non AC      2             Rs.3000\n5.Duplex             4             Rs.4500\n6.Suite              5             Rs.5000\n");
        printf("\nEnter Number of Rooms you want:");
        scanf("%d",&n);
        int choice;
        while(1)
        {
            choice=getchoice();
            a.num=printroom(choice);
            if (a.num == -2)
            {
                printf("\nTotal fare is Rs.%lld\n\n",fare);
                return;
            }
            count++;
            printf("\nHey %s, your booking is successful and your room number is %d!!\n\n",a.name,a.num);
            fare+=days(a.fd,a.fm,a.fy,a.td,a.tm,a.ty)*price[choice-1];
            strncpy(a.status,"Booked",sizeof(a.status));
            p=fopen("database","a");
            fprintf(p,"%s : Room no. %d from %d %d %d to %d %d %d : %s Fare:%d\n",a.name,a.num,a.fd,a.fm,a.fy,a.td,a.tm,a.ty,a.status,days(a.fd,a.fm,a.fy,a.td,a.tm,a.ty)*price[choice-1]);
            fclose(p);
            if(count==n)
            {
                break;
            }
        }
        printf("\nTotal fare is Rs.%lld\n\n",fare);
    }
}


/*__________________________________________________________________________________________________________________*/


//To view the general arrangement of the rooms at all the floors
void view()
{
	printf("             sea facing\n");
	printf("____________________________________\n");
	printf("|  -------    -------    -------   |\n");
	printf("|  |  1  |    |  2  |    |  3  |   |\n");
	printf("|  -------    -------    -------   |\n");
	printf("|  -------    -------    -------   |\n");
	printf("|  |  4  |    |  5  |    |  6  |   |\n");
	printf("|  -------    -------    -------   |\n");
	printf("|  -------    -------    -------   |\n");
	printf("|  |  7  |    |  8  |    |  9  |   |\n");
	printf("|  -------    -------    -------   |\n");
	printf("------------------------------------\n");
}


/*__________________________________________________________________________________________________________________*/


//Cancelling the already booked reservations by the authorised user
void cancel()
{
    int fare,i;
    int flag = 0,flag1,control = 0;
    FILE *database,*q,*newdata,*pass;
    char nam[100];
    char ipassword[100];
    printf("\nEnter Name(Without spaces): ");

    //checking if the name has space in between or not
    scanf(" %[^\n]s",nam);
    for(i = 0; i<strlen(nam); i++)
    {
		if(nam[i] == ' ')
        {
        	printf("Invalid Username\n\n");
        	return;
        }
	}
    printf("Enter password(Without spaces): ");

    //checking if the password has space in between or not
    scanf(" %[^\n]s",ipassword);
    for(i = 0; i<strlen(ipassword); i++)
    {
	    if(ipassword[i] == ' ')
        {
        	printf("Invalid Password\n\n");
        	return;
       	} 
	}
    pass = fopen("user credentials","r");

    //Checking if the username and password are correct
    do
    {
        fscanf(pass,"%s : %s",user.name,user.password);
        if(strcmp(user.name,nam) == 0 && strcmp(user.password,ipassword) == 0)
        {
            flag1 = 1; 
        }
    }
    while( !feof(pass) );
    fclose(pass);

    //printing all the bookings made in the name of the user 
    if(flag1 == 1)
    {
        int cntrl = 0;
        q = fopen("database","r");
        do
        {
           fscanf(q,"%s : Room no. %d from %d %d %d to %d %d %d : %s Fare:%d",a.name,&a.num,&a.fd,&a.fm,&a.fy,&a.td,&a.tm,&a.ty,a.status,&fare);
           if(feof(q))
            {
               break;
            }
            if(strcmp(a.name,nam) == 0)
            {
                if(strcmp(a.status,"Booked") == 0)
                {
                    cntrl++;
                }
                control++;
                flag = 1;
                printf("%d.) %s : Room no. %d from %d %d %d to %d %d %d : %s Fare:%d\n\n",control,a.name,a.num,a.fd,a.fm,a.fy,a.td,a.tm,a.ty,a.status,fare);
            }    
        }
        while( !feof(q) );

        if (flag == 0)
        {
           printf("Booking Not Found\n\n");
           return;
        }
        fclose(q);
        int flag2 = 0;
        int loop;

        //Asking the user how many bookings(in no.) he wants to cancel
        do
        {
            flag2 = 0;
            printf("How many bookings do you want to cancel: ");
            scanf("%d",&loop);
            if(loop>cntrl)
            {
                printf("Invalid number of cancellatons\n\n");
                flag2 = 1;
                int choice;
                printf("Do you want to continue with cancellation? Enter 1 to continue, 0 to return to main menu: ");
                scanf("%d",&choice);
                if (choice == 0)
                {
                    return;
                }
            }
        }
        while(flag2 == 1);

        //Asking him the serial no. of the booking which he wants to cancel and then cancelling it if the serial no. is valid
        for(int i = 0; i<loop; i++)
        {
            int ctr,ctr1 = 0;
            flag2 = 0;
            do
            {
                flag2 = 0;
                printf("\nWhich number booking do you want to cancel: ");
                database = fopen("database","a");
                q = fopen("database","r");
                newdata = fopen("newdatabase","w");
                scanf("%d",&ctr);
                if(ctr > control)
                {
                    flag2 = 1;
                    printf("Booking does not exist\n\n");
                }
            }
            while(flag2 == 1);

            do
            {
               fscanf(q,"%s : Room no. %d from %d %d %d to %d %d %d : %s Fare:%d" ,a.name,&a.num,&a.fd,&a.fm,&a.fy,&a.td,&a.tm,&a.ty,a.status,&fare);
               if(strcmp(a.name,nam) == 0)
               {
                    if( feof(q) )
                    {
                        break;
                    }
                    ctr1++;
                    if (ctr == ctr1)
                    {
                        flag2 = 1;
                        if (strcmp(a.status,"Booked") == 0)
                        {
                            fprintf(newdata,"%s : Room no. %d from %d %d %d to %d %d %d : Cancelled Fare:%d\n",a.name,a.num,a.fd,a.fm,a.fy,a.td,a.tm,a.ty,fare);
                            printf("Your booking has been cancelled\n");
                        }
                        else
                        {
                            printf("This booking has already been cancelled\n\n");
                            int choice;
                            printf("Do you want to continue with the cancellation? Enter 1 to continue, 0 to return to main menu: ");
                            scanf("%d",&choice);
                            if (choice == 0)
                            {
                                return;
                            }
                            fprintf(newdata,"%s : Room no. %d from %d %d %d to %d %d %d : Cancelled Fare:%d\n",a.name,a.num,a.fd,a.fm,a.fy,a.td,a.tm,a.ty,fare);
                            loop++;
                        }
                    } 
                    else
                    {
                        fprintf(newdata,"%s : Room no. %d from %d %d %d to %d %d %d : %s Fare:%d\n",a.name,a.num,a.fd,a.fm,a.fy,a.td,a.tm,a.ty,a.status,fare);
                    }
                }
                else
                {
                    fprintf(newdata,"%s : Room no. %d from %d %d %d to %d %d %d : %s Fare:%d\n",a.name,a.num,a.fd,a.fm,a.fy,a.td,a.tm,a.ty,a.status,fare);
                }
            }
            while( !feof(q) );

            fclose(database);
            fclose(q);
            fclose(newdata);
            newdata = fopen("newdatabase","r");
            database = fopen("database","w");
            do
            {
                fscanf(newdata,"%s : Room no. %d from %d %d %d to %d %d %d : %s Fare:%d" ,a.name,&a.num,&a.fd,&a.fm,&a.fy,&a.td,&a.tm,&a.ty,a.status,&fare);
                if( feof(newdata) )
                {
                    break;
                }
                fprintf(database,"%s : Room no. %d from %d %d %d to %d %d %d : %s Fare:%d\n",a.name,a.num,a.fd,a.fm,a.fy,a.td,a.tm,a.ty,a.status,fare);
            }
            while( !feof(newdata) );

            fclose(newdata);
            fclose(database);
        }
    }
    else
    {
        printf("Invalid username or password\n");
    }
}


/*__________________________________________________________________________________________________________________*/


//To review the bookings made by a registered person using its password
void review()
{
    int fare,i;
    printf("Review\n");
    int flag = 0,flag1 = 0;
    FILE *data,*pass;
    data = fopen("database","r");
    char nam[100];
    char ipassword[100];
    printf("\nEnter Name(Without spaces): ");

    //checking if the name has space in between or not
    scanf(" %[^\n]s",nam);
    for(i = 0; i<strlen(nam); i++)
    {
		if(nam[i] == ' ')
        {
        	printf("Invalid Username\n\n");
        	return;
        }
	}
    printf("Enter your password(Without spaces): ");

    //checking if the password has space in between or not
    scanf(" %[^\n]s",ipassword);
    for(i = 0; i<strlen(ipassword); i++)
    {
	    if(ipassword[i] == ' ')
        {
        	printf("Invalid password\n\n");
        	return;
       	} 
	}
    //checking if the password is correct or not
    pass = fopen("user credentials","r");
    do
    {
        fscanf(pass,"%s : %s",user.name,user.password);
        if(strcmp(user.name,nam) == 0 && strcmp(user.password,ipassword) == 0)
        {
            flag1 = 1; 
        }
    }
    while( !feof(pass) );
    fclose(pass);

    //printing the bookings of the person after the password is found to be legitimate
    if(flag1 == 1)
    {
        printf("Booking History:\n");
        do
        {
            fscanf(data,"%s : Room no. %d from %d %d %d to %d %d %d : %s Fare:%d" ,a.name,&a.num,&a.fd,&a.fm,&a.fy,&a.td,&a.tm,&a.ty,a.status,&fare);
            if( feof(data) )
            {
                break;
            }
            if(strcmp(a.name,nam) == 0)
            {
                flag = 1;
                printf("%s : Room no. %d from %d %d %d to %d %d %d : %s Fare:%d\n\n",a.name,a.num,a.fd,a.fm,a.fy,a.td,a.tm,a.ty,a.status,fare);
            }
        }
        while( !feof(data) );

    }
    else
    {
        printf("Invalid username or password\n");
    }
}


/*__________________________________________________________________________________________________________________*/


//Main function to run everything
int main()
{
    int choice1,flag,flag1=0;
    char string[10],s1[2]={'y'},s2[2]={'n'};
    void (*f1[4])()={book,cancel,review,view};
    printf("\n");
    printf("\n");
    printf(" /$$      /$$ /$$$$$$$$ /$$        /$$$$$$   /$$$$$$  /$$      /$$ /$$$$$$$$       /$$$$$$$$ /$$$$$$        /$$   /$$  /$$$$$$  /$$$$$$$$ /$$$$$$$$ /$$      \n");
    printf("| $$  /$ | $$| $$_____/| $$       /$$__  $$ /$$__  $$| $$$    /$$$| $$_____/      |__  $$__//$$__  $$      | $$  | $$ /$$__  $$|__  $$__/| $$_____/| $$      \n");
    printf("| $$ /$$$| $$| $$      | $$      | $$  \\__/| $$  \\ $$| $$$$  /$$$$| $$               | $$  | $$  \\ $$      | $$  | $$| $$  \\ $$   | $$   | $$      | $$      \n"); 
    printf("| $$/$$ $$ $$| $$$$$   | $$      | $$      | $$  | $$| $$ $$/$$ $$| $$$$$            | $$  | $$  | $$      | $$$$$$$$| $$  | $$   | $$   | $$$$$   | $$      \n");
    printf("| $$$$_  $$$$| $$__/   | $$      | $$      | $$  | $$| $$  $$$| $$| $$__/            | $$  | $$  | $$      | $$__  $$| $$  | $$   | $$   | $$__/   | $$      \n");    
    printf("| $$$/ \\  $$$| $$      | $$      | $$    $$| $$  | $$| $$\\  $ | $$| $$               | $$  | $$  | $$      | $$  | $$| $$  | $$   | $$   | $$      | $$      \n");
    printf("| $$/   \\  $$| $$$$$$$$| $$$$$$$$|  $$$$$$/|  $$$$$$/| $$ \\/  | $$| $$$$$$$$         | $$  |  $$$$$$/      | $$  | $$|  $$$$$$/   | $$   | $$$$$$$$| $$$$$$$$\n");
    printf("|__/     \\__/|________/|________/ \\______/  \\______/ |__/     |__/|________/         |__/   \\______/       |__/  |__/ \\______/    |__/   |________/|________/\n");
    printf("\n");
    printf("\n");

    //Making the menu for booking, cancellation or review of the rooms booked by a user
    while(1)
    {
        printf("\n");
        printf("Enter:\n \n1.  Book\n \n2.  Cancel\n \n3.  Review booking\n \n4.  View\n \n5.  Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d",&choice1);
        if(choice1 > 5 || choice1<1)
        {
            printf("Invalid choice\n\n");
            continue;
        }
        if(choice1==5)
        {
            break;
        }
        else
        {
            (*f1[choice1-1])();
        }
        do
        {
            flag = 0;
            printf("\nDo you wish to continue?(y/n):");
            scanf("%s",string);
            if (strcmp(string,s2) == 0)
            {
                flag1=0;
                break;
            }
            else if (strcmp(string,s1) == 0)
            {
                flag1=1;
                break;
            }
            else 
            {
                printf("sorry you entered a wrong choice\n");
                flag = 1;
                continue;
            }
        }
        while (flag == 1);

        if(flag1==0)
        {
            break;
        }
        else if(flag1==1)
        {
            continue;
        }
    }
    return 0;
} 