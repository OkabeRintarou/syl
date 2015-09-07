

bool strings_not_equals(const char * s1,const char * s2)
{
	// check whether s1 is not equal to s2
}

void phase_1(const char * str) // str is user input
{
	const char * answer = "Public speaking is very easy."; // answer is correct input string of phase one
	if(strings_not_equals(str,answer)){
		explode_bomb();
	}
}

void phase_2(const char * str) // str is user input
{
	int a[6];
	read_six_number(a);

	if(a[0] == 1){
		goto label;
	}
	explode_bomb();
label:
	int b = 1;
	while(b < 5){
		int c = a[b - 1] * b;
		if(c == a[b] ){
			++b;
			goto label;
		}
		else{
			explode_bomb();
		}
		++b;
	}
}

void phase_3(const char * str) // str is user input
{
	int a,b;
	char c,d;
	if(sscanf(str,"%d %c %d",&a,&c,&b) > 2){
		switch(b){
			case 0:
				d = 0x71;
				if(a == 0x309)goto label;
			case 1:
				d = 0x62;
				if(a == 0xd6)goto label;
			case 2:
				d = 0x62;
				if(a == 0x2f3)goto label;
			case 3:
				d = 0x6b;
				if(a == 0xfb)goto label;
			case 4:
				d = 0x6f;
				if(a == 0xa0)goto label;
			case 5:
				d = 0x74;
				if(a == 0x1ca)goto label;
			case 6:
				d = 0x76;
				if(a == 0x30c)goto label;
			case 7:
				d = 0x62;
				if(a == 0x20c)goto label;
			default:
				explode_bomb();break;
		}

	}else{
		d = 0x78;
		explode_bomb();
	}
	label:
		if(c == d)return;
		else{
			explode_bomb();
		}
	return;
}

int func4(int a)
{
	if(a <= 1){
		return 1;
	}
	else{
		return func(a - 1) + func(a - 2);
	}
}

void phase_4(const char * str) // str is user input
{ 
	int a;
	if(sscanf(str,"%d",&a) != 1){
		explode_bomb();
	}
	if( a > 0)goto label;
	explode_bomb();
label:
	func4(a);

}

void phase_5(const char * str) // str is user input
{
	int a = string_length(str);
	if(a == 6)goto label1;
	explode_bomb();
label1:
	const char * esi = "isrveawhobpnutfg\260\001";
	const char * target = "giants";
	int index = 0;
	do{
		char ch = 0;
		ch = str[index];
		ch = ch & 0x0f;
		str[index] = esi[ch];
		++index;
	}while(index <= 5);

	if(strings_not_equals(str,target) == 0){
		return;
	}
	else{
		explode_bomb();
	}
}

struct list_node{
	int v[2];
	list_node * next;
};

void phase_6(const char * str) // str is user input
{
	char arr[6]; // 1 2 3 4 5 6
	int i;

	read_six_number(str,a);

	i = 0;
	do{
		int tmp = arr[i];
		tmp = tmp - 1;
		if(tmp > 5){
			explode_bomb();
		}
		else{
			int ebx = i + 1;
			while(ebx < 5){
				if(arr[ebx] != arr[i]){
					;
				}
				else{
					explode_bomb();
				}
				ebx  = ebx + 1;
			}
		}
		i =  i + 1;
	}while(i <= 5);

	// 

	i = 0;
	do{
		int ebx = 1;
		int tmp = arr[i];
		if(tmp > 1){
			do{
				++ebx;
			}while(tmp > ebx);
		}
		++i;
	}while(i <= 5);

}


int main()
{

}