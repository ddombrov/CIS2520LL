Your name: Daniel Dombrovsky 

Student number: 1211846

Resources used: 2500 notes, 2520 slides

Implementation State: currently incomplete (hope I update this when I'm done)

Summary: 
- a short summary of what you have learned about the relative amounts of time required to perform these tasks

use description not sequence as key, dont use vs code for storage space, use append FIFO

			/**
			FASTArecord *array = NULL;
			array = fastaAllocateRecord();
			if (array == NULL) {
        		fprintf(stderr, "Memory allocation failed.\n");
				return -1;
        	}	
			for (int i=0;i<status; i++) {
				array[i] = fastaAllocateRecord();
				status = fastaReadRecord(fp, array[i]);
				
				if (array[i] == NULL) {
        			fprintf(stderr, "Memory allocation failed.\n");
					return -1;
        		}	
			}
			fastaDeallocateRecord(array);
			*/

			
		LLvNode *l_head = NULL;
		LLvNode *l_node = NULL;
		LLvNode *l_node2 = NULL;

		l_node = malloc(sizeof(LLvNode));
		//l_node->key=fRecord->description;
		l_node->next=l_head;
		l_head=l_node;

		l_node2 = malloc(sizeof(LLvNode));
		//l_node2->key=fRecord->description;
		l_node2->next=NULL;
		l_node=l_head;
		
		while (l_node!=NULL) {
			l_node=l_node->next;
		}
		l_node->next=l_node2;

		//print all
		while (l_head!=NULL) {

			printf("%p", l_head->next);
			l_head = l_head->next;

		}