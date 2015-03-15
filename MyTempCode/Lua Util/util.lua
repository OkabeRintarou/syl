--求a,b最大公约数
function math.gcd(a,b)
	if b == 0 
		then return a
	else 
		return math.gcd(b , a % b)
	end 
end 

--打印表的内容
--[[
function table.print(tab)
	local function table.format(tab,str)
		for _,v in tab 
	end
end
]]
--返回表的尺寸
function table.size(table)
	local count = 0
	for _ in pairs(table) do count = count + 1 end
	return count
end

--逆向表
function table.reverse(tab)
	local  d = {}
	local  size = table.size(tab)
	for i = 1,size do
		d[size + 1 - i] = tab[i]
	end
	return d
end

--逆向表深拷贝
function table.dreverse(tab)
	local d = {}
	local size =  table.size(tab)
	for i = 1,size do
			d[size + 1  - i] = table.dcopy(tab[i])
	end
end

--浅拷贝
function table.copy(table)
	local d = {}
	for k,v in pairs(table) do
		d[k] = v
	end
	return d
end

--参拷贝
function table.dcopy(t)
	local d = {}
	for k,v in pairs(t) do
		if type(v) == "table" then d[k] = table.dcopy(v)
		else d[k] = v end
	end
	return d
end