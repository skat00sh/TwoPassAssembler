#include<stdio.h>
#include<string.h>

main()
{
	int i,j,type,p,length,value,n,c,k,colon;
	char opcode[18],reg1_add[4],reg2_add[4],reg3_add[4],label[15],label_add[18];
	char part1[15],part2[10],opcode1[15],opcode2[18],A[2],B[2],C[2],immediate[18],str[25],obj[25],obj_created[50];
    FILE *file,*file_opcode,*file_address,*object_file;

/* Opening Requested File */
    printf("enter the file name: ");
    scanf("%s",str);
    strcpy(obj,str);
    strcpy(obj_created,str);
    strcat(obj_created,"_object.txt");
    strcat(str,".txt");
    file = fopen(str,"r");
    if ( file != NULL )
     {
   	  type=0;
   	  memset(immediate,0,18);

/* Fetching both columns from opcode file */
   	  memset(part1,0,15);memset(part2,0,10);
      while (fscanf(file,"%s %s",part1,part2) != EOF) {
      memset(A,0,2);memset(B,0,2);memset(C,0,2);
      colon = strlen(part1);
      if(strcmp(part1,"HLT")==0 || strcmp(part1,"RZ")==0 || strcmp(part1,"RETURN")==0 )
      {
        length = strlen(part2);
        fseek(file,-length,1);
        memset(part2,0,10);
      }

      if(part1[colon-1] == ':' )
      {

          colon=strlen(part2);
          fseek(file,-colon,1);
          memset(part1,0,15);
          memset(part2,0,10);
          continue;
      }



      file_opcode = fopen("opcode.txt","r");
      memset(reg3_add,0,4);
      memset(reg2_add,0,4);
      memset(reg1_add,0,4);
      while(fscanf(file_opcode,"%s %s",opcode1,opcode2) != EOF) {
   		if(strcmp(part1,opcode1) == 0)
        {
          strcpy(opcode,opcode2);
        }

        memset(opcode1,0,15);
        memset(opcode2,0,18);
      }
      fclose(file_opcode);

/* Classifying Opcode into different types */

/* Opcode + Register type */
      if(strlen(opcode) < 13 && part2 != NULL)
	  {
        type=1;
        i=0;
        while(part2[i] != 44){
		 A[0]=part2[i];
		 i++;
		}
		i++;
        while(part2[i] != 44 && i < strlen(part2)){
		 B[0]=part2[i];
		 i++;
	 	}
        while(i < strlen(part2)){
         C[0]=part2[i];
		 i++;
        }
      }

/* Opcode + immediate value type */
	  else if(part2 != NULL && strlen(opcode)==13)
      {
		type=2;
		i=0;
        while(part2[i] != 44){
		 A[0]=part2[i];
		 i++;
		}
		j=0;
	 	while(i <= strlen(part2)){
         i++;
		 immediate[j]=part2[i];
		 j++;
		}
		value = atoi(immediate);
        j=0;
		for(c=15;c>=0;c--)
        {
		  k = value >> c;
		  if(k & 1){
		    immediate[j]='1';j++;
          }
          else
          {
			immediate[j]='0';j++;
          }
         }
       }

/* Opcode + Label type */
       else if(part2[0] != NULL && strlen(opcode)==16)
       {
         type = 3;
       	 i=0;
		 j=0;
		 while(i <= strlen(part2)){
          label[j]=part2[i];
          i++;
          j++;
         }
		}

/* Only Opcode type */
		else
		  type=4;

/* Fetching address from Symbol Table */
       file_address = fopen("Symbol.txt","r");
       while(fscanf(file_address,"%s %s",opcode1,opcode2) != EOF) {
          if(A[0] != NULL || label != NULL)
          {
            if(strcmp(A,opcode1) == 0)
            {
      		  	strcpy(reg1_add,opcode2);
            }
          }
       }
       fclose(file_address);
       //checking for address of second reg
       file_address = fopen("Symbol.txt","r");
       while(fscanf(file_address,"%s %s",opcode1,opcode2) != EOF) {
          if(B[0] != NULL)
          {
            if(strcmp(B,opcode1) == 0)
            {
               	strcpy(reg2_add,opcode2);
            }
          }
       }
       fclose(file_address);
       //checking 3rd
       file_address = fopen("Symbol.txt","r");
       while(fscanf(file_address,"%s %s",opcode1,opcode2) != EOF) {
          if(C[0] != NULL || label != NULL)
          {
            if(strcmp(C,opcode1) == 0)
            {
            	strcpy(reg3_add,opcode2);
            }
          }
       }
       //checking label address
       fclose(file_address);
       file_address = fopen("Symbol.txt","r");
       while(fscanf(file_address,"%s %s",opcode1,opcode2) != EOF) {
          if(label != NULL)
          {
             if(strcmp(label,opcode1)==0)
             {
               	strcpy(label_add,opcode2);
             }
          }
       }

/* Putting machine code in the desired file */
       fclose(file_address);
       object_file = fopen(obj_created,"a+");
       if (object_file == NULL){
          printf("I couldn't open object.txt for writing.\n");
       }
       switch(type)
       {
      	case 1:
      		fprintf(object_file,"%s%s%s%s\n",opcode,reg1_add,reg2_add,reg3_add);
            break;
      	case 2:
            fprintf(object_file,"%s%s\n%s\n",opcode,reg1_add,immediate);
      		break;
      	case 3:
      		fprintf(object_file,"%s\n%s\n",opcode,label_add);
			break;
        case 4:
			fprintf(object_file,"%s\n",opcode);
			break;
        }
      	memset(part1,0,15);memset(part2,0,10);
     }
     fclose ( file );
     printf("\nYour Machine Code has been generated in %s_object.txt\n",obj);

   }
   else
   {
      perror ( str ); /* why didn't the file open? */
   }
}


