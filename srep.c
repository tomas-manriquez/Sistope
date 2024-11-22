#define TRUE 1
#define FALSE 0 

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

//Entradas: input (texto entrada), target_string (string en el input), new_string (el string que ocupara el lugar nuevo en el input
//Salidas: string input
//Reemplaza todas las apariciones de 'target_string' por un nuevo 'new_string' en el string 'input' de entrada
char* srep(char* input, char* target_string, char* new_string)
{
	if(target_string == NULL)
		return input;
	if(new_string == NULL)
		return input;
	char* aux= (char*)malloc(sizeof(char)* (strlen(input)+1));
	char* find= (char*)malloc(sizeof(char)* (strlen(input)+1));

	while(1)
	{
		if ( (find = strstr(input, target_string)) == NULL)		//no se encontro target_string, se copia el resto del input
		{
			strcat(aux, input);
			break;
		}
		else
		{
			long pre_find_size = find - input;			//bytes del texto de input previo al 1er 'target_string' encontrado
			strncat(aux, input, pre_find_size);			//se copia el input hasta antes del 1er 'target_string' encontrado
			strcat(aux, new_string);				//se copia el 'new_string' en reemplazo de 'target_string'
			input = find + strlen(new_string);			//se sigue revisando el resto del input
		}
	}
	free(find);
	return aux;
}

//Entradas: -i nombre de archivo de input (stdin si no se incluye -i), -o para archivo de output deseado (stdout si no se incluye -o)...
//..., -s para string objetivo, -S para string nuevo
//Salidas: input modificado via STDOUT, o archivo nuevo de nombre output_file si este fue indicado con -o
//Hace control de argumentos de entrada, y realiza operacion 'srep' segun definido en enunciado de este laboratorio
int main(int argc, char* argv[])
{
	//strcat(argv[argc-1], "\\");
	printf("argc=%d\n",argc);
	int i;
	for(i=0; i<argc; i++)
	{
		printf("%s\t", argv[i]);
	}
	printf("\n");
	
	//Variables para manejo de banderas
	int opt;
	char* input_file = NULL;
	char* output_file = NULL;
	char* target_string = NULL;
	char* new_string = NULL;

 
	//OBTENER PARAMETROS DE ENTRADA
	while((opt = getopt(argc, argv, "i:o:s:S::")) != -1)
	{
		switch(opt)
		{
			case 'i':
			input_file = optarg;
			break;

			case 'o':
			output_file = optarg;
			break;

			case 's':
			if(strcmp(optarg, "-S"))
			{
				if(argv[optind]==NULL)
				{
					target_string = optarg;
					new_string = NULL;
					printf("case s if: target_string = %s, new_string = %s\n", target_string, new_string);
				}
				else
				{
					target_string = "\\";
					new_string = argv[optind];
					printf("case s else: target_string = %s, new_string = %s\n", target_string, new_string);
				}
			}
			else
			{
				target_string = optarg;
			}
			break;

			case 'S':
			new_string = optarg;
			if(optarg ==NULL)
			{
				if(argv[optind]==NULL || strcmp(argv[optind],"\\") )
					new_string = "\\";
				else
				{
					new_string = argv[optind];
				}
			}
			break;
			
			case '?':
        		if (optopt == 'i' || optopt == 'o' || optopt == 's' || optopt == 'S')		//banderas se ingresaron pero sin argumento (ej. '-i -o' es decir -i no tenia nombre de archivo)
          			fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        		else if (isprint (optopt))							//se ingreso un parametro desconocido
          			fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        		else										//se ingreso un parametro desconocido que no se puede imprimir en pantalla como char
          			fprintf (stderr,
                   			"Unknown option character `\\x%x'.\n",
                   			optopt);
        		return 1;
			break;

			default:
			abort();
		}
	}

	
	int index;
	for (index = optind; index < argc; index++)
    	printf ("Non-option argument %s\n", argv[index]);
	

	//SREP
	if(input_file == NULL)			//input mediante STDIN
	{
		char* input = (char*)malloc(sizeof(char)*2048);
		printf("Ingrese input: ");
		fgets(input, sizeof(char)*2048, stdin);
		printf("\n");
		printf("%s\n", input);

		//SREP
		input = srep(input, target_string, new_string);
		
		if(output_file == NULL)		//output mediante STDOUT
		{
			printf("%s\n", input);
		}
		else				//output mediante archivo
		{
			FILE* f2 = fopen(output_file, "a");
			fprintf(f2,"%s", input);
			fclose(f2);
		}
		
	}
	else					//input mediante archivo
	{
		FILE* f1 = fopen(input_file, "r");

		//Calculando tamano de archivo para asignar a string de input
		fseek(f1, 0L, SEEK_END);
		long file_size = ftell(f1);
		printf("fileSize = %ld\n", file_size);
		char* input = (char*)malloc(sizeof(char)*(file_size+1));
		rewind(f1);


		//Leyendo archivo a string de input
		fread(input, sizeof(char), file_size+1,f1);
		
		
		//SREP
		input = srep(input, target_string, new_string);

		if(output_file == NULL)		//ouput mediante STDOUT
		{
			printf("%s\n", input);
		}
		else
		{
			FILE* f2 = fopen(output_file, "a");
			fprintf(f2, "%s", input);
			fclose(f2);
		}
		
		fclose(f1);
	}
	
	exit(0);
}
