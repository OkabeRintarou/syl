package sort

func merge(lhs, rhs []int) []int {
	data := make([]int, len(lhs)+len(rhs))

	i, j, k := 0, 0, 0
	for i < len(lhs) && j < len(rhs) {
		if lhs[i] < rhs[j] {
			data[k] = lhs[i]
			k++
			i++
		} else if lhs[i] > rhs[j] {
			data[k] = rhs[j]
			k++
			j++
		} else {
			data[k] = lhs[i]
			data[k+1] = rhs[j]
			k += 2
			i++
			j++
		}
	}

	if i < len(lhs) {
		copy(data[k:], lhs[i:])
	} else if j < len(rhs) {
		copy(data[k:], rhs[j:])
	}
	return data
}

func mergeSort(data []int) []int {
	n := len(data)
	if n >= 2 {
		key := n / 2
		left := mergeSort(data[:key])
		right := mergeSort(data[key:])
		return merge(left, right)
	}
	return data
}

func MergeSort(data []int) {
	copy(data, mergeSort(data))
}
