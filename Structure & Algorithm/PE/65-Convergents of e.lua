require "util"

local function denominator(n)
	if (n % 3) == 2 then return (n + 1) * 2 / 3
	else return 1
	end
end

local function fraction_simplify(a)
	local q = math.gcd(a.n,a.d)
	while q ~= 1 do
		a.n = a.n / q
		a.d = a.d / q
		q = math.gcd(a.n,a.d)
	end
	return a
end
local function fraction_add(a,b)
	local r = {}
	r.n = a.n * b.d + a.d * b.n
	r.d = a.d * b.d
	return r
end

local function ConvergentsOfe(base,tab)
	local ret =  {n = 0,d = 1}
	local tmp = {n = 0,d = 1}
	for i = #tab,1,-1 do
		tmp.n = tab[i]
		ret = fraction_add(ret,tmp)
		ret.n,ret.d = ret.d,ret.n
	end
	tmp.n ,tmp.d = 2,1
	ret = fraction_add(ret,tmp)
	return ret
end

local function test()
	local base = 2
	local limit  = 99
	tab = {}
	for i = 1,limit,1 do
		tab[i] = denominator(i)
	end
	num= ConvergentsOfe(2,tab).n
	print(num)
end

test()