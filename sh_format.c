//./sh_format rev.txt | tr -d "\n" | tr -d "x" | tr -d "\\" | tr -d "\"" | tr -d ";"

#include <stdio.h>
#include <string.h>

int main (int argc, char **argv)
{
  char *filename = argv[1];
  FILE *file = fopen ( filename, "r" );
  int u = 0;
  if ( file != NULL )
  {
    char line [ 500 ];
    while ( ( fgets ( line, sizeof line, file ) != NULL ) && (u<=1) )
    {
      if (line[0]=='u')      
	u++;

      
      else if (line[0]=='\"')      
        fputs ( line, stdout );           
      
    }
    fclose ( file );
  }
  else
  {
    perror ( filename );
  }
  return 0;
}
