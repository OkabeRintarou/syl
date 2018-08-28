package sort

func qsort(data []int, lower, upper int) {
	if lower < upper {
		last := data[upper]
		i := lower
		for j := lower; j < upper; j++ {
			if data[j] < last {
				data[i], data[j] = data[j], data[i]
				i++
			}
		}

		data[i], data[upper] = last, data[i]
		qsort(data, lower, i-1)
		qsort(data, i+1, upper)
	}
}

func QuickSort(data []int) {
	if len(data) > 0 {
		qsort(data, 0, len(data)-1)
	}
}
