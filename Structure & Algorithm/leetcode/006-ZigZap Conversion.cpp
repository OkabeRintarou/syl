string convert(string s,int nRows)
{
	// Why Input "AB",2 not passed?
	    const int len = s.length();
	    if(len <= 1)return s;
	    
	    std::vector<std::string> arrs(nRows);

	    int row = 0, step = 1;
	    for (int i = 0; i < len; ++i){
		    arrs[row].push_back(s[i]);

		    if (row == 0)step = 1;
		    else if (row == nRows - 1)step = -1;

		    row += step;
	    }

	    s.clear();
	    for (int i = 0; i < nRows; ++i){
		    s.append(arrs[i]);
	    }

	    return s;
}