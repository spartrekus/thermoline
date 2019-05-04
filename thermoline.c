
#include <stdio.h>
#define PATH_MAX 2500
#if defined(__linux__) //linux
#define MYOS 1
#elif defined(_WIN32)
#define MYOS 2
#elif defined(_WIN64)
#define MYOS 3
#elif defined(__unix__) 
#define MYOS 4  // freebsd
#define PATH_MAX 2500
#else
#define MYOS 0
#endif
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <time.h>

#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>  

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <time.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h> 
#include <time.h>



#define ESC "\033"
#define home() 			printf(ESC "[H") 
#define clrscr()		printf(ESC "[2J") 
#define gotoxy(x,y)		printf(ESC "[%d;%dH", y, x);
#define ansigotoyx(y,x)		printf(ESC "[%d;%dH", y, x);



 int usersel   = 1;
 int show_menu = 1;
 int menu_usersel = 0;
 char fichier[PATH_MAX];
 char statusline[PATH_MAX];
 char strmenu[PATH_MAX];




static struct termios oldt;

void restore_terminal_settings(void)
{
    tcsetattr(0, TCSANOW, &oldt);  /* Apply saved settings */
}

void enable_waiting_for_enter(void)
{
    tcsetattr(0, TCSANOW, &oldt);  /* Apply saved settings */
}

void disable_waiting_for_enter(void)
{
    struct termios newt;

    /* Make terminal read 1 char at a time */
    tcgetattr(0, &oldt);  /* Save terminal settings */
    newt = oldt;  /* Init new settings */
    newt.c_lflag &= ~(ICANON | ECHO);  /* Change settings */
    tcsetattr(0, TCSANOW, &newt);  /* Apply settings */
    atexit(restore_terminal_settings); /* Make sure settings will be restored when program ends  */
}


/////////////////////////////////
/////////////////////////////////
int fexist(char *a_option)
{
  char dir1[PATH_MAX]; 
  char *dir2;
  DIR *dip;
  strncpy( dir1 , "",  PATH_MAX  );
  strncpy( dir1 , a_option,  PATH_MAX  );

  struct stat st_buf; 
  int status; 
  int fileordir = 0 ; 

  status = stat ( dir1 , &st_buf);
  if (status != 0) {
    fileordir = 0;
  }

  // this is compatible to check if a file exists
  FILE *fp2check = fopen( dir1  ,"r");
  if( fp2check ) {
  // exists
  fileordir = 1; 
  fclose(fp2check);
  } 

  if (S_ISDIR (st_buf.st_mode)) {
    fileordir = 2; 
  }
return fileordir;
/////////////////////////////
}



void nrunwith( char *cmdapp, char *filesource )
{
           char cmdi[PATH_MAX];
           strncpy( cmdi , "  " , PATH_MAX );
           strncat( cmdi , cmdapp , PATH_MAX - strlen( cmdi ) -1 );
           strncat( cmdi , " " , PATH_MAX - strlen( cmdi ) -1 );
           strncat( cmdi , " \"" , PATH_MAX - strlen( cmdi ) -1 );
           strncat( cmdi ,  filesource , PATH_MAX - strlen( cmdi ) -1 );
           strncat( cmdi , "\" " , PATH_MAX - strlen( cmdi ) -1 );
           system( cmdi ); 
}



void nls( int foosel )
{ 
   DIR *dirp; 
   int foocounter = 1;
   strncpy( strmenu , "" , PATH_MAX );
   struct dirent *dp;
   dirp = opendir( "." );
   while  ((dp = readdir( dirp )) != NULL ) 
   {
         if (  strcmp( dp->d_name, "." ) != 0 )
         if (  strcmp( dp->d_name, ".." ) != 0 )
         {
           if ( foocounter == foosel )  
           {
             printf( ">%s\n", dp->d_name );
             strncpy( strmenu , dp->d_name , PATH_MAX );
           }
           else
           {
             printf( " %s\n", dp->d_name );
           }
           foocounter++;
         }
   }
   closedir( dirp );
}




///////////////////////////////////////////
void readfile( char *filesource )
{
  FILE *source; 
  int ch ; 
  if ( fexist( filesource ) == 1 ) 
  {
   source = fopen( filesource , "r");
   while( ( ch = fgetc(source) ) != EOF )
   {
         printf( "%c", ch );
   }
   fclose(source);
  }
}
   





float limleft  = 0.16;
float limright = 0.24;
float point_sel_a_x = 0;
float point_sel_a_y = 0;
float point_sel_b_x = 0;
float point_sel_b_y = 0;
float point_calc_x = 0.2;
float point_calc_y = 0;
float cur_x = 0;
float cur_y = 0;






///////////////////////////////////////////
void append_to_file( char *targetfile )
{
   FILE *target;  
   char foostr[PATH_MAX];
   target = fopen( "datapoints.log" , "ab+");
     snprintf( foostr, PATH_MAX , "#%s\n" , fichier );
     fputs( foostr , target ); 

     snprintf( foostr, PATH_MAX , "#[A] (%lf;%lf)\n" , point_sel_a_x, point_sel_a_y );
     fputs( foostr , target ); 

     snprintf( foostr, PATH_MAX , "#[B] (%lf;%lf)\n" , point_sel_b_x, point_sel_b_y );
     fputs( foostr , target ); 

     snprintf( foostr, PATH_MAX , "%lf;%lf" , point_calc_x, point_calc_y );
     fputs( foostr , target ); 
     fputs( "\n" , target ); 
   fclose( target );


   target = fopen( targetfile , "ab+");
     snprintf( foostr, PATH_MAX , "%lf;%lf" , point_calc_x, point_calc_y );
     fputs( foostr , target ); 
     fputs( "\n" , target ); 
   fclose( target );

   strncpy( statusline, "Added points to file.", PATH_MAX );
}







///////////////////////////////////////////
void readfileline( char *filesource )
{
   FILE *source; 
   FILE *target; 
   FILE *fp5; 
   int ch, c, co;
   char lline[PATH_MAX];
   char ncpfile[PATH_MAX];
   char charo[PATH_MAX];
   int pcc = 0;
   int export_skip = 0;
   int area_archive = 0;
   int fileeof = 0;
   int separator = ';'; 

   int i,j, k ; 
   char ptr[PATH_MAX];
   char qtr[PATH_MAX];
   char str[PATH_MAX];
   int rdp ; 
   float value    = 0; 
   float middlept = 0; 
   int  counterln = 1;

   printf( "File: %s\n", filesource );
   source = fopen( filesource , "r");
   //target = fopen( fileoutput , "wb+");
   while( fileeof == 0 )
   {
       export_skip = 0;
       c = fgetc( source); 
       if ( c == EOF ) 
         fileeof = 1;

       else
       {
          if ( c != '\n' )
               lline[pcc++]=c;
          else 
          {

           if ( ( lline[ 0 ] == '/' )  && ( lline[ 1 ] == '*' ) ) 
               area_archive = 1;

           if ( ( lline[ 0 ] == '*' )  && ( lline[ 1 ] == '/' ) ) 
           {    area_archive = 0;  export_skip = 1;       }

           if ( fileeof == 0 )
           if ( c == '\n' ) 
           {
              lline[pcc++]='\0';
              if ( area_archive == 0 )
              {
                //printf( "line: %s\n", lline ); 
                if (  ( lline[ 0 ] == '/' ) && ( lline[ 1 ] == '/' )  )
                {  printf( "Skip line\n" ); }
                else 
                {
                    j = 0;  rdp = 0;  k = 0;
                    strncpy( str, lline, PATH_MAX );
                    for(i=0; str[i]!='\0'; i++)
                    {
                      if ( str[i] == ';' ) 
                         rdp++;

                      if ( str[i] != '\0' ) 
                        if ( rdp == 0 ) 
                            ptr[j++]=str[i];

                      if ( str[i] != '\0' ) 
                        if ( rdp == 1 ) 
                          if ( str[ i ] != separator )
                            qtr[k++]=str[i];

                    } 
                    ptr[j]='\0';
                    qtr[k]='\0';

                    value = atof( ptr );
                    if (  ( value >= limleft )  && ( value <= limright ) )
                    {
                       //fputs( lline ,    target);
                       //fputs( "\n" ,     target);
                       //printf( "%lf\n" , value );
                       //printf( "PTR %s\n" , ptr );
                       //printf( "QTR %s\n" , qtr );
                       if ( usersel == counterln ) 
                       {
                         cur_x = atof( ptr ); 
                         cur_y = atof( qtr );    
                       }
                       if ( usersel == counterln )  printf( "> " ); else printf( "  " );
                       printf( "X=%lf Y=%lf \n" , atof( ptr ) , atof( qtr ) );
                       counterln++;
                    }
                }
              }
            }
           // clear
           lline[0]='\0';
           pcc = 0;
          }
       }
       co = c; 
   }
   fclose( source );
   //fclose( target );
}





////////////////////////////////////////
int main( int argc, char *argv[])
{
   int i ; int foochg = 0;
   if ( argc >= 2)
   {
          for( i = 1 ; i < argc ; i++) 
	    printf( "%d/%d: %s\n", i, argc-1 ,  argv[ i ] );
   }
   else   
      return 0;

   strncpy( fichier, argv[ 1 ] , PATH_MAX );


   int ch = 0; 
   int gameover = 0;
   char yourcmd[1000];
   char strmsg[PATH_MAX];
   strncpy( statusline, "", PATH_MAX );
   strncpy( yourcmd, "", 1000 );
   disable_waiting_for_enter();
   int j ; int chr ;  char ptr[PATH_MAX];

   while( gameover == 0)
   {
      printf( "\n" );
      if ( fexist( fichier ) == 1 ) 
      {
        printf( "=======================\n" );
        printf( "*=- RESULTS -=*\n" );
        printf( "=======================\n" );
        readfileline( fichier ); 
        printf( "Lim    [%lf ; %lf]\n", limleft, limright );
        printf( "Cur    X=%lf Y=%lf\n", cur_x, cur_y );
        printf( "Point (a)  X=%lf Y=%lf\n", point_sel_a_x, point_sel_a_y );
        printf( "Point (b)  X=%lf Y=%lf\n", point_sel_b_x, point_sel_b_y );
        if ( point_sel_a_x  != 0 )  
        if ( point_sel_b_x  != 0 ) 
        {
            printf( "Slope=%lf\n"       , ( point_sel_b_y - point_sel_a_y )  / (point_sel_b_x - point_sel_a_x ) );

            point_calc_y = ( point_sel_b_y - point_sel_a_y )  / (point_sel_b_x - point_sel_a_x ) * point_calc_x +
            point_sel_a_y - (( point_sel_b_y - point_sel_a_y )  / (point_sel_b_x - point_sel_a_x ) )  * point_sel_a_x       ; 


            printf( "Calc: Y=a*X+b => X=%lf, Y=%lf\n" ,  point_calc_x, point_calc_y );
        }
       }
       else
         printf( "File not found (%s).\n" , fichier );

       printf( "%s\n", statusline ); 
       strncpy( statusline, "", PATH_MAX );

       if ( show_menu == 1 )
       {
          printf( "\n" );
          printf( "=======================\n" );
          printf( "*=- MENU -=*\n" );
          printf( "=======================\n" );
          printf( "  'x': nls\n" );
          printf( "  'w': write/append point calc (x,y)\n" );
          printf( "  'c': cat output file\n" );
          printf( "  'a': cat datapoints log file\n" );
          printf( "  'v': vim output file\n" );
          printf( "  '^o': open data file\n" );
          printf( "  'r': view data file\n" );
          printf( "  'e': edit data file\n" );
          printf( "  'm': hide/show this menu\n" );
          printf( "  ':': command line\n" );
          printf( "  keys j,k:  up/down selection\n" );
          printf( "  keys h,l:  (a) and (b) selection\n" );
          printf( "  keys {[]}: left/right limits\n" );
       }

       printf( "A:> " );
       ch = getchar();
       printf( "Key %d (%s)\n", ch , yourcmd );

       if ( ch == 'q' ) gameover = 1;
       else if ( ch == 'j' )  usersel++; 
       else if ( ch == 'k' )  usersel--; 
       else if ( ch == 'g' )  usersel = 1;

       else if ( ch == 'c' ) { readfile( "output.csv" );      printf("<Press Key>");  getchar();  }
       else if ( ch == 'a' ) { readfile( "datapoints.log" );  printf("<Press Key>");  getchar();  }

       else if ( ch == 'x' ) { nls( 0 ); printf("<Press Key>");  getchar();  }
       else if ( ch == 'v' ) { system( " vim  output.csv" ); }


       else if ( ch == 'r' ) { nrunwith( " less ", fichier ); }
       else if ( ch == 'e' ) { nrunwith( " vim ", fichier ); }

       else if ( ch == 'm' ) { if ( show_menu == 1 ) show_menu = 0; else show_menu = 1 ; }

       else if ( ch == 'h' ) { point_sel_a_x = cur_x ; point_sel_a_y = cur_y; }
       else if ( ch == 'l' ) { point_sel_b_x = cur_x ; point_sel_b_y = cur_y; }

       else if ( ch == '{' ) limleft  -=0.005;
       else if ( ch == '[' ) limleft  +=0.005;
       else if ( ch == ']' ) limright -=0.005;
       else if ( ch == '}' ) limright +=0.005;

       else if ( ch == '+' ) point_calc_x +=0.005;
       else if ( ch == '-' ) point_calc_x -=0.005;

       else if ( ch == 'w' ) append_to_file( "output.csv" );

       else if ( ch == '#' )
       {
           enable_waiting_for_enter();
           printf( "what is calc x value?\n");
           printf( "=> " );
           scanf( "%s", yourcmd );
           point_calc_x = atof( yourcmd );
           disable_waiting_for_enter();
       }

       else if ( ch == 8 )
       {
                point_sel_a_x = 0;
                point_sel_a_y = 0;
                point_sel_b_x = 0;
                point_sel_b_y = 0;
       }

       else if ( ch == 15 )
       {
          strncpy( strmsg , fichier, PATH_MAX );
          strncpy( strmenu, "" , PATH_MAX );
          foochg = 0;  
          while( gameover == 0 ) 
          {
            clrscr();
            home();
            printf( "LS:\n");
            nls( menu_usersel );
	    if ( foochg   ==  1 ) strncpy( strmsg, strmenu, PATH_MAX ); 
            foochg = 0;  
            printf( "\n");
            printf( "(Current File: %s)\n", fichier );
            printf( "File: " );
            printf( "%s", strmsg );
            ch = getchar(); 
            if       ( ch == 27 )  gameover = 1;
            else if  ( ch == 15 )  gameover = 1;
            else if  ( ch == 21 ) { menu_usersel--; foochg = 1; } 
            else if  ( ch == 4  ) { menu_usersel++; foochg = 1; } 
            else if  ( ch == 5  ) { gameover = 1;  strncpy( fichier, strmenu , PATH_MAX ); }
            else if  ( ch == 10 ) { gameover = 1;  strncpy( fichier, strmsg, PATH_MAX ); }
	    else if ( ( ch == 8 )  || ( ch == 127 ) )  
            {
               if ( strlen( strmsg ) >= 1 ) 
               {
                 j = 0; strncpy(  ptr , "" ,  PATH_MAX );
                 for ( chr = 0 ;  chr <= strlen( strmsg )-2 ; chr++) 
                 {
                    ptr[j++] = strmsg[chr];
                 }
	         strncpy( strmsg, ptr ,  PATH_MAX );
               }
            }
	    else if (
			(( ch >= 'a' ) && ( ch <= 'z' ) ) 
		        || (( ch >= 'A' ) && ( ch <= 'Z' ) ) 
		        || (( ch >= '1' ) && ( ch <= '9' ) ) 
		        || (( ch == '0' ) ) 
		        || (( ch == '~' ) ) 
		        || (( ch == '!' ) ) 
		        || (( ch == '&' ) ) 
		        || (( ch == '=' ) ) 
		        || (( ch == ':' ) ) 
		        || (( ch == ';' ) ) 
		        || (( ch == '<' ) ) 
		        || (( ch == '>' ) ) 
		        || (( ch == ' ' ) ) 
		        || (( ch == '|' ) ) 
		        || (( ch == '#' ) ) 
		        || (( ch == '?' ) ) 
		        || (( ch == '+' ) ) 
		        || (( ch == '/' ) ) 
		        || (( ch == '\\' ) ) 
		        || (( ch == '.' ) ) 
		        || (( ch == '$' ) ) 
		        || (( ch == '%' ) ) 
		        || (( ch == '-' ) ) 
		        || (( ch == ',' ) ) 
		        || (( ch == '{' ) ) 
		        || (( ch == '}' ) ) 
		        || (( ch == '(' ) ) 
		        || (( ch == ')' ) ) 
		        || (( ch == ']' ) ) 
		        || (( ch == '[' ) ) 
		        || (( ch == '*' ) ) 
		        || (( ch == '"' ) ) 
		        || (( ch == '@' ) ) 
		        || (( ch == '-' ) ) 
		        || (( ch == '_' ) ) 
		        || (( ch == '^' ) ) 
		        || (( ch == '\'' ) ) 
	             ) 
		  {
                        snprintf( ptr, PATH_MAX , "%s%c",  strmsg, ch );
		        strncpy(  strmsg,  ptr ,  PATH_MAX );
		  }
            
          }
          gameover = 0;
          ch = 0;
       }

       else if ( ch == ':' )
       {
           enable_waiting_for_enter();
           printf( "what is your cmd ?\n");
           printf( "=> " );
           scanf( "%s", yourcmd );
           if ( strcmp( yourcmd, "quit" ) == 0 ) gameover = 1;
           else if ( strcmp( yourcmd, "sh" ) == 0 )   system( " sh  " );

           disable_waiting_for_enter();
       }

   }
   printf( "\n");
   printf( "Bye\n");
   printf( "\n");
   return 0; 
}




