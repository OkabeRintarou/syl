package main

const (
	oblique = 0
	down    = 1
	left    = 2
)

func lps(s string) string {
	dir := make([][]byte, len(s))
	length := make([][]int, len(s))
	for i := 0; i < len(s); i++ {
		dir[i] = make([]byte, len(s))
		length[i] = make([]int, len(s))
		length[i][i] = 1
	}

	for delta := 1; delta < len(s); delta++ {
		for i := 0; i < len(s)-delta; i++ {
			j := i + delta
			if s[i] == s[j] {
				length[i][j] = length[i+1][j-1] + 2
				dir[i][j] = oblique
			} else if length[i+1][j] > length[i][j-1] {
				length[i][j] = length[i+1][j]
				dir[i][j] = down
			} else {
				length[i][j] = length[i][j-1]
				dir[i][j] = left
			}
		}
	}

	path := make([]byte, 0)
	i, j := 0, len(s)-1
	for i <= j {
		if i == j {
			path = append(path, s[i])
			break
		} else if dir[i][j] == left {
			j--
		} else if dir[i][j] == down {
			i++
		} else {
			path = append(path, s[i])
			i++
			j--
		}
	}
	for j = len(path) - 2; j >= 0; j-- {
		path = append(path, path[j])
	}
	return string(path)
}

func main() {
	println(lps("character"))
	println(lps("aaa"))
	println(lps("aaabbbba"))
	println(lps("carac"))
	println(lps(""))
}
