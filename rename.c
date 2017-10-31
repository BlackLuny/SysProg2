/*argv[0] - program name (rename)*/

#include <stdio.h>

int main(int argc, char **argv)
{
	int result;
        if (argc != 3) {
                printf("Less arguments in %s\n", argv[0]);
                return (1);
        }
        result = rename(argv[1], argv[2]);
        if (!result){                
		printf("Success");
                return (result);
        }
	else{
	printf("Error");
        return (0);
	}
}
