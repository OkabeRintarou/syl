#include <cstdio>

int main()
{
	int year,month,day;
	char c;
	while(scanf("%d",&year) != EOF){
		scanf("%c",&c);
		scanf("%d",&month);
		scanf("%c",&c);
		scanf("%d",&day);
		int total = 0;
		for(int i = 1;i < month;i++){
			switch(i){
			case 1:
				total += 31;break;
			case 2:
				if(((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)){
					total += 29;
				}else{
					total += 28;
				}
				break;
			case 3:
				total+=31;break;
			case 4:
				total+=30;break;
			case 5:
				total+=31;break;
			case 6:
				total+=30;break;
			case 7:
				total+=31;break;
			case 8:
				total+=31;break;
			case 9:
				total+=30;break;
			case 10:
				total+=31;break;
			case 11:
				total+=30;break;
			case 12:
				total+=31;break;
				break;
			}
		}
		total += day;
		printf("%d\n",total);
	}
	return 0;
}
