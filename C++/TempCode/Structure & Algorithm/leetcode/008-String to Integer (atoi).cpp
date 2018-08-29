int myAtoi(string str) 
{
	if (str.empty()){
		return 0;
	}
	
	int i = 0;
	int size = str.size();
	while (i < size && std::isspace(str[i])){
		++i;
	}

	if (i == size)return 0;
	bool negative = false;
	if (str[i] == '-'){
		negative = true;
		++i;
	}
	else if (str[i] == '+'){
		++i;
	}
	
	int ans = 0;
	for (; i < size; ++i){
		if (std::isdigit(str[i])){
			if ((ans > 214748364)  || (ans == 214748364 && str[i] > '7')){
				return negative ? INT_MIN : INT_MAX;
			}
			else{
				ans *= 10;
				ans += (str[i] - '0');
			}
		}
		else{
			break;
		}
	}

	return negative ? -ans : ans;
}