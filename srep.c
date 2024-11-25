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
    if(target_string == NULL || new_string == NULL || input == NULL)
        return input;

    // Calculate the maximum possible length of the result
    size_t input_len = strlen(input);
    size_t target_len = strlen(target_string);
    size_t new_len = strlen(new_string);

    // Count occurrences of target_string in input
    size_t count = 0;
    char* temp = input;
    while ((temp = strstr(temp, target_string)) != NULL) {
        count++;
        temp += target_len;
    }

    // Calculate required size and allocate memory
    size_t max_size = input_len + (count * (new_len - target_len)) + 1;
    char* aux = (char*)malloc(sizeof(char) * max_size);
    if (aux == NULL) return input;  // Handle allocation failure
    aux[0] = '\0';  // Initialize empty string

    char* current_pos = input;
    char* find;

    while(1)
    {
        find = strstr(current_pos, target_string);
        if (find == NULL)             // No more occurrences found
        {
            strcat(aux, current_pos);  // Copy the rest of the input
            break;
        }
        else
        {
            size_t pre_find_size = find - current_pos;  // Length of text before the match
            strncat(aux, current_pos, pre_find_size);   // Copy text up to the match
            strcat(aux, new_string);                    // Replace with new string
            current_pos = find + target_len;            // Advance past the target string
        }
    }

    return aux;
}


//Entradas: -i nombre de archivo de input (stdin si no se incluye -i), -o para archivo de output deseado (stdout si no se incluye -o)...
//..., -s para string objetivo, -S para string nuevo
//Salidas: input modificado via STDOUT, o archivo nuevo de nombre output_file si este fue indicado con -o
//Hace control de argumentos de entrada, y realiza operacion 'srep' segun definido en enunciado de este laboratorio
int main(int argc, char* argv[])
{
	//strcat(argv[argc-1], "\\");
	//printf("argc=%d\n",argc);
	//int i;
	/**
	for(i=0; i<argc; i++)
	{
		printf("%s\t", argv[i]);
	}
	printf("\n");
	**/
	
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
			target_string = optarg;
			/**
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
			**/

			break;

			case 'S':
			//new_string = optarg;
			new_string = argv[optind];
			/**
			if(optarg ==NULL)
			{
				if(argv[optind]==NULL || strcmp(argv[optind],"\\") )
					new_string = "\\";
				else
				{
					new_string = argv[optind];
				}
			}
			**/
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
	//printf("target_string = %s, new_string=%s\n", target_string, new_string);

	
	//int index;
	//for (index = optind; index < argc; index++)
    	//printf ("Non-option argument %s\n", argv[index]);
	

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
	else    // input mediante archivo
	{
	   FILE* f1 = fopen(input_file, "r");
	   if (f1 == NULL) {
	       fprintf(stderr, "Error: No se pudo abrir el archivo de entrada\n");
	       return 1;
	   }
	
	   // Calculando tamano de archivo para asignar a string de input
	   fseek(f1, 0L, SEEK_END);
	   long file_size = ftell(f1);
	   rewind(f1);
	
	   // Agregamos espacio para el caracter nulo
	   char* input = (char*)malloc(sizeof(char) * (file_size + 1));
	   if (input == NULL) {
	       fprintf(stderr, "Error: No se pudo asignar memoria\n");
	       fclose(f1);
	       return 1;
	   }
	
	   // Leyendo archivo a string de input
	   size_t bytes_read = fread(input, 1, file_size, f1);
	   if (bytes_read != (size_t)file_size) {
	       fprintf(stderr, "Error: No se pudo leer el archivo completo\n");
	       free(input);
	       fclose(f1);
	       return 1;
	   }
	
	   // Aseguramos que el string termine en nulo
	   input[file_size] = '\0';
	
	   // SREP
	   char* result = srep(input, target_string, new_string);
	   if (result == NULL) {
	       fprintf(stderr, "Error: Fallo en el reemplazo de strings\n");
	       free(input);
	       fclose(f1);
	       return 1;
	   }
	
	   if (result != input) {  // srep allocated new memory
	       free(input);
	       input = result;
	   }
	
	   if (output_file == NULL) {    // output mediante STDOUT
	       printf("%s\n", input);
	   }
	   else {
	       FILE* f2 = fopen(output_file, "a");
	       if (f2 == NULL) {
	           fprintf(stderr, "Error: No se pudo abrir el archivo de salida\n");
	           free(input);
	           fclose(f1);
	           return 1;
	       }
	       fprintf(f2, "%s", input);
	       fclose(f2);
	   }
	
	   free(input);
	   fclose(f1);
	   return 0;
	}
	/**
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
	**/
	
	exit(0);
}
