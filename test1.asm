
;um comentário


SECTION           TEXT;um outro comentário 
INPUT 		 		N    	 	 	 ;e mais um pra ter certeza
LOAD  	 	 	 N 
FAT: SUB ONE 
JMPZ          	 FIM 
STORE AUX 


;olha
	;só 
		;esses 
			;comentários

MULT N 

 
   STORE N 
			 LOAD AUX 
JMP FAT 
FIM: OUTPUT N 
 	 	 	 	 	 	 	 STOP 
SECTION DATA 
AUX: SPACE 
N: SPACE 



  	 	 	 ONE: CONST 1 
