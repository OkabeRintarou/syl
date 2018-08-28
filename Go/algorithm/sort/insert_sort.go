package sort

func InsertSort(data []int) {
	for i := 1; i < len(data); i++ {
		for j := 0; j < i; j++ {
			if data[i] < data[j] {
				v := data[i]
				for k := i; k > j; k-- {
					data[k] = data[k-1]
				}
				data[j] = v
			}
		}
	}
}
