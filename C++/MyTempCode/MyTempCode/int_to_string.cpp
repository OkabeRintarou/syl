
  // 将int型变量转换成std::string和std::wstring类型
std::string i_to_a(int val,int radix = 10)
{
    char buf[256];
    _itoa_s(val, buf, _countof(buf), radix);
    return std::string(buf);
}
 
std::wstring i_to_w(int val, int radix = 10)
{ 
    wchar_t buf[256];
    _itow_s(val, buf, _countof(buf), radix);
    return std::wstring(buf);
}


// 将double类型转换成std::string和std::wstring类型
std::string d_to_s(double val, int _Count = 6)
{
    char * buf = new char[_CVTBUFSIZE];
    int decimal;
    int sign;
 
    _ecvt_s(buf, _CVTBUFSIZE, val, _Count, &decimal, &sign);
 
    std::string ret(buf);
    ret.insert(decimal, std::string("."));
 
    delete buf;
 
    return ret;
}
 
std::wstring d_to_w(double val, int _Count = 5)
{
    return toWideString(d_to_s(val, _Count));
}

  
