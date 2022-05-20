package main

import "os"
import "C"

func main() {}

//export Init
func Init() {
	f, err := os.Open("test.txt")
	if err != nil {
		return
	}
	defer f.Close()

	f.WriteString("test")
}

//export RVExtensionArgsImpl
func RVExtensionArgsImpl(outputSize int, function string, argv []string, argc int) (status int, output string) {
	status = 1
	output = "2"
	return
}

//export RVExtensionVersionImpl
func RVExtensionVersionImpl(outputSize int) (output string) {
	output = "v0.0.1"
	return
}
