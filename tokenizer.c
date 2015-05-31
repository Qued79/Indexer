#include "tokenizer.h"

char *TypeToString(tokentype x){
	switch(x){
		case NA			:return("ERROR");
		case WORD 		:return("Word");			
		case DECI 		:return("Decimal Number");		
		case OCT 		:return("Octal Number");		
		case HEX 		:return("Hexadecimal Number");		
		case FLO 		:return("Float");			
		case OP 		:return("Operator");			
		case LPAREN 		:return("Left Parentheses");		
		case RPAREN 		:return("Right Parentheses");	
		case LBRACK 		:return("Left Bracket");		
		case RBRACK 		:return("Right Bracket");		
		case POINTER 		:return("Structure Pointer");	
		case MINUS 		:return("Subtraction Symbol (binary) or Minus Symbol (unary)");		
		case NEGATE 		:return("Negation Sign");		
		case COMP 		:return("Complement Sign");		
		case INC 		:return("Increment");			
		case DEC 		:return("Decrement");			
		case MULTIPLY 		:return("Multiplication Symbol (binary) or Indirect (unary)");		
		case DIVIDE 		:return("Division Symbol");		
		case MOD 		:return("Modulus Symbol");		
		case ADD 		:return("Addition Symbol");		
		case RSHIFT 		:return("Right Shift Operator");	
		case LSHIFT 		:return("Left Shift Operator");		
		case LESS 		:return("Less Than Symbol");		
		case GREATER 		:return("Greater Than Symbol");		
		case LESSEQ 		:return("'Less Than or Equal To' Symbol");		
		case GREATEREQ 		:return("'Greater Than or Equal To' Symbol");		
		case EQUALS 		:return("Equals Symbol");		
		case NOTEQUALS 		:return("'Not Equals' Symbol");		
		case BITAND 		:return("'Bitwise AND' Symbol");		
		case BITXOR 		:return("'Bitwise XOR' Symbol");		
		case BITOR 		:return("'Bitwise OR' Symbol");		
		case LOGAND 		:return("Logical AND Symbol");		
		case LOGOR 		:return("Logical OR Symbol");		
		case CONDTRUE 		:return("Condition TRUE Symbol");		
		case CONDFALSE 		:return("Condition FALSE Symbol");		
		case ASSIGN 		:return("Assignment Operator");		
		case PLUSEQUALS 	:return("Plus Equals Operator");		
		case MINUSEQUALS 	:return("Minus Equals Operator");		
		case TIMESEQUALS 	:return("Times Equals Operator");		
		case DIVEQUALS 		:return("'Divided By Equals' Operator");		
		case MODEQUALS 		:return("Modulus Equals Operator");		
		case RSHIFTEQUALS 	:return("Right Bitwise Shift Equals");		
		case LSHIFTEQUALS 	:return("Left Bitwise Shift Equals");		
		case BITANDEQUALS 	:return("Bitwise AND Equals");		
		case BITXOREQUALS 	:return("Bitwise XOR Equals");		
		case BITOREQUALS 	:return("Bitwise OR Equals");		
		case COMMAOP 		:return("Comma Operator");
		case SEMICOLON		:return("Semicolon");
		//                 :(
		}
return("ERROR");
}




struct TokenizerT_ *TKCreate( char *ts){
	if(ts==NULL){return (NULL);}
	if(ts[0]=='\0'){return (NULL);}
	struct TokenizerT_ *token =  malloc(sizeof(struct TokenizerT_));
	token->data = ts;		//This will hold the String part of the token (i.e. the token)
	token->type = NA;		//This is an enumerated type that was originally just 6 types that got a bit out of hand. NA is a sort of null value.
	token->next = NULL;		//I'm using this to create a linked list structure since I have no way of knowing how many tokens there will be (except <= length of string but that oculd be huge)
	return token;
}


void TKDestroy(struct TokenizerT_ *tk ){
	free(tk->data);
	free(tk);
}


char *TKGetNextToken(struct TokenizerT_ *tk){
	if(tk==NULL){return 0;}
	if(tk->data[0]=='\0'){return 0;}
	char *ptr;
	char *beginning = tk->data;
	while(isspace(beginning[0])){beginning++;}
	ptr=beginning;
	tk->type = NA;//resetting this as it will be used to check for invalid input
	if(isdigit(ptr[0])){//if it is a digit
		if(ptr[0]=='0'){//check for 0 as the first digit, potentially a hex or octal
			if(ptr[1]=='x'){
					tk->type = HEX; 
					ptr+=2;//I skip over the 0x for easier parsing later, but they will be in the final output
					}//if it begins with 0x it is a hex
			else if(!isdigit(ptr[1])){tk->type = DECI;}//if the entire number is just 0 then it is an int equal to 0 (for now, revisit for floats)
			else{tk->type = OCT;}//if it did not become either a hex or an oct and it is more than one digit long, then it is an octal (for now - float possibly)
			      }
		else{tk->type = DECI;}//if it did not begin with a number != 0 then we know we have a decimal number
			   }
	else if(isalpha(ptr[0])){tk->type = WORD;ptr[0]=tolower(ptr[0]);}//if it instead began with an alphabetic character, then we have a word.
	else if ((ispunct(ptr[0])) && *ptr!='#' && *ptr!= '\"' && *ptr!='$' && *ptr!='.' && *ptr!='\"' && *ptr!='@' && *ptr!='_' && *ptr!='\'' && *ptr!='`'){
	tk->type = OP;//Holy god that is ugly. I have a lot of different types of characters though, so there's really no avoiding it. If this is a punct that is also an operator, set it as such
																		 	    }
	 else if(ptr[0]=='.'){tk->type = FLO;	ptr+=1;}//the +=1 just skips over the . when we begin to parse
//In all of the below I walk until I hit a character that changes the type. The exception being that I allow conversion to float as it is essentially two decimal (or octal actually) numbers separated by a .
//at the end of this section I will malloc space enough to hold ths string between the pointer location and the beginning of the stream passed in and copy it over, making a new token.
	
if(tk->type!=WORD){return 0;}/*ADDED TO MAKE TOKENIZER RETURN 0 FOR NON-WORD TOKENS*/

	switch (tk->type){

		case OCT:
			while( (isdigit(*ptr)) ||(*ptr == '.') ||(*ptr == 'E' ) ){ //checks if it is a digit, then makes sure it is not 9 or 8 so that overall we are checking if it is an octal #	
				if(*ptr == '.' || *ptr=='E'){tk->type = FLO;}//The reason that I added the ugly checks for float is because otherwise I would have to have a separate check for FLO type and loop at the end otherwise
				ptr++;				// If I hadn't done the octal check, numbers like 03.29 would have problems. Though to be fair, if you're putting an extra 0 in front of your numbers in a sketchy
				if(*ptr=='9'||*ptr=='8'){if(tk->type != FLO){tk->type=DECI;}		break;}	 }//tokenizer, you sort of deserve what you get. Numbers like 0.29 would be classified as DEC and be fine.
				if(tk->type==OCT){break;}//lets it fall through if it turned into a decimal number
		
		case DECI:
			while(isdigit(*ptr) || *ptr == '.' || *ptr =='E'){
				if(*ptr == '.'||*ptr=='E'){tk->type = FLO;}
				ptr++;
							   		 }
			break;

		case HEX:
			while(isxdigit(*ptr)){ptr++;}			break;
		
		case FLO:{
			if(*ptr=='\0'){return (0);} //special check for '.' case	
			while(isdigit(*ptr)||*ptr=='E'){ptr++;}
			break;
			 }

		case WORD:
			while(isalnum(*ptr)){
			ptr[0]=tolower(ptr[0]);
			ptr++;
			}			break;
			
		//Now for the case you've been dreading since you saw my enum!
		case OP:{
			switch (*ptr)	{
				//The reason each of these have ptr++ past them is because I want pointer at the end of the string for uniform parsing when the method is finished
				//All of this part is hardcoded, so there's really not much to say, hence the lack of comments. I know that it looks sloppy to increment at every level
				//instead of all at once before the fact, but that would have required me to use negative indices for a pointer, and I didn't want to make the code look
				//that confusing, especially since it is really simple hardcoding. Also, efficiencywise it isn't any different. The ++ happens once per iteration either way.
				//(twice for longer operators, but that would have been the case anyway)
				case '(':{tk->type = LPAREN; 	ptr++;	break;		} 
				case ')':{tk->type = RPAREN;	ptr++;	break;		}
				case '[':{tk->type = LBRACK;	ptr++;	break;		}
				case ']':{tk->type = RBRACK;	ptr++;	break;		}
				case '-':{
					if(ptr[1]=='>'){tk->type = POINTER; ptr++;}	
					else if(ptr[1]=='-'){tk->type = DEC; ptr++;}
					else if(ptr[1]=='='){tk->type = MINUSEQUALS; ptr++;}
					else tk->type = MINUS;
					ptr++;
					break;
					}
				case '*':{
					if(ptr[1]=='='){tk->type = TIMESEQUALS;}
					else {tk->type = MULTIPLY;}
					ptr++;
					break;
					}
				case '&':{					
					if(ptr[1]=='&'){tk->type = LOGAND; ptr++;}
					else if(ptr[1]=='='){tk->type = BITANDEQUALS; ptr++;}
					else {tk->type = BITAND;}
					ptr++;
					break;
					}
				case '!':{
					if(ptr[1]=='='){tk->type = NOTEQUALS; ptr++;}
					else {tk->type = NEGATE;}
					ptr++;
					break;
					 }
				case '~':{tk->type = COMP;	ptr++;	break;		}
				case '+':{	
					if(ptr[1]=='+'){tk->type = INC; ptr++;}
					else if(ptr[1]=='='){tk->type = PLUSEQUALS; ptr++;}
					else{tk->type = ADD;}
					ptr++;	
					break;
					 }
				case '/':{					
					if(ptr[1]=='='){tk->type = DIVEQUALS; ptr++;}
					else{tk->type = DIVIDE;}
					ptr++;
					break;
					 }
				case '%':{	
					if(ptr[1]=='='){tk->type = MODEQUALS; ptr++;}
					else{tk->type = MOD;}
					ptr++;
					break;
					 }
				case '<':{	
					if(ptr[1]=='<'){
						if(ptr[2] == '='){tk->type = LSHIFTEQUALS; ptr+=2;}
						else {tk->type = LSHIFT; ptr+=1;}
						}
					else if(ptr[1]=='='){tk->type = LESSEQ; ptr++;}
					else{tk->type = LESS;}
					ptr++;
					break;
					 }
				case '>':{					
					if(ptr[1]=='>'){
						if(ptr[2]=='='){tk->type = RSHIFTEQUALS; ptr+=2;}
						else {tk->type = RSHIFT; ptr+=1;}
						}
					else if(ptr[1]=='='){tk->type = GREATEREQ; ptr++;}
					else{tk->type = GREATER;}
					ptr++;
					break;
					 }					

				case '=':{
					if(ptr[1]=='='){tk->type = EQUALS; ptr++;}
					else{tk->type = ASSIGN;}
					ptr++;
					break;
					 }
				case '^':{
					if(ptr[1]=='='){tk->type = BITXOREQUALS; ptr++;}
					else{tk->type = BITXOR;}
					ptr++;
					break;
					 }
				case '|':{
					if(ptr[1]=='='){tk->type = BITOREQUALS; ptr++;}
					else if(ptr[1]=='|'){tk->type = LOGOR; ptr++;}
					else{tk->type = BITOR;}
					ptr++;
					break;	
					 }
				case '?':{tk->type = CONDTRUE;	ptr++;	break;	} 
				case ':':{tk->type = CONDFALSE;	ptr++;	break;	}
				case ',':{tk->type = COMMAOP;	ptr++;	break;	}
				case ';':{tk->type = SEMICOLON;	ptr++;	break;	}
				default: return (0);
					}//end OP switch
			break;
			}//end case OP (thank god)
			default: return (0); //did not match any pattern for different types of token. It is bad or unhandled input, so return 0;			
			}//end larger switch
			//Very ugly code. I apologize but I really don't see a way to deal with unique characters (mapped to unique types) without dealing with each individually

			//At this point we have a pointer to the end of the token and a pointer to the beginning (tk->data) so we can make a token as we have a string and a type
			// ptr-(beginning) will give the length of the token to be created.
			char* str = malloc((ptr-(beginning)+2)*sizeof(char));
			int i;
			//iterate over the values between ptr and tk->data as these are the ones in the token that we just built
			for(i=0;i<(ptr-(beginning));i++){
				str[i]=beginning[i]; 
							}
			
			str[i]='\0';//finish by appending the end of string character to the newly built string.



			if(tk->type != WORD){free(str);return 0;}/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ADDED TO MAKE THIS TOKENIZER ONLY TOKENIZE WORDS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			


			return str;
			//Because I am restricted by the interface, I am going to use the tk->type as a way to pass additional information out of the method.
			//I know that this is less than ideal for a lot of reasons, but as you can see I had to determine type before I could determine the length of the token
			//and as it is writtem, the determining of the type and finding the end of the token are too interconnected to make one a separate method to be used again
			//This is mostly due to the operators whose type and length are determined at the same time.
	}


void tokenizeAndStoreFile(FILE *fp, const char* filePath, struct hashtable *table){
	char *words;
	int size=0;	
	struct TokenizerT_ *root,*last;
	size_t temp;
	char *aToken;	
	int i;
	struct TokenizerT_ *x;
	/*Jump to end, grab the relative location of EOF then jump back to beginning*/
	fseek(fp,0L,SEEK_END);
	size=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	words = (char*)malloc(size+1);
	/*read contents of entire file into words, appending a \0 at the end (reason for +1 above)*/
	temp = fread(words,1,size,fp);/*temp is to hold the number of bytes written to words so we can append the \0 at the end*/
	fclose(fp);
	words[temp]='\0';	
	x = TKCreate(words);
	aToken = TKGetNextToken(x);
	if(aToken==0){free(words);return;}
	while(aToken==0){//Skips over any unhandled characters. 
		x->data++;
		if(x->data=='\0'){return;}//if you never got a root there is nothing to output so you should just return.
		aToken = TKGetNextToken(x);
			}
	root=last=TKCreate(aToken);
	root->type = x->type;//as discussed above I'm pulling the data from the token passed into TKGetNextToken	
	i=0;
	while((last->data[i]==(*x->data)) && ((x->data[0])!='\0')) {x->data++; i++;}//move x's data until it is past the token that was just retrieved by TKGetNextToken (unless they hit end of string char) 
	while(x->data[0]!='\0'){//until our pointer hits the null character
		aToken = TKGetNextToken(x);	//get the next token from x
		if(aToken==0){
			x->data++;
			continue;
			     }
		last->next = TKCreate(aToken);	//make this token into a Tokenizer and put it as the next in the linked list
		last = last->next;		//move last to the new end of the list
		last->type = x->type;		//pull the type of the new token from x and set it in the last
		i=0; 					
		while(isspace(x->data[0])){
			x->data++;
					  } //Eliminate whitespace before moving on
		while((last->data[i]==(*x->data)) && ((x->data[0])!='\0')){
			x->data++;
			i++;
									  }//};//move x->data pointer to the beginning of the string not yet tokenized
	}
		
	last = root;//no longer adding so don't need a pointer to the end of the list. Repurposing last as a curr pointer for output
	while(last!=NULL){
		root=last;	
		update(last->data,filePath,table); 
		last = last->next;	
		TKDestroy(root);
			}
	free(x);
	free(words);
	return;

} 
