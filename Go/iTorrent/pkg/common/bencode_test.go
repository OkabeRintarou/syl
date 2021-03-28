package common

import "testing"

func unexpectedError(t *testing.T, err error) {
    t.Fatalf("Unexpected error: %v\n", err)
}

func testInt(t *testing.T, v int, expected string) {
    if d, err := EncodeInt(v); err != nil {
        unexpectedError(t, err)
    } else if d != expected {
        t.Fatalf("Expected %v, got %v\n", expected, d)
    }
}

func TestEncodeInt(t *testing.T) {

    testInt(t, 0, "i0e")
    testInt(t, 42, "i42e")
    testInt(t, -42, "i-42e")
}

func TestEncodeString(t *testing.T) {
    if d, err := EncodeString("spam"); err != nil || d != "4:spam" {
        unexpectedError(t, err)
    } else if d != "4:spam" {
        t.Fatalf("Expect 4:spam, got %v\n", d)
    }
}

func TestEncodeList(t *testing.T) {
    if d, err := EncodeList([]interface{}{"spam", 42}); err != nil {
        unexpectedError(t, err)
    } else if d != "l4:spami42ee" {
        t.Fatalf("Expect l4:spami42ee, got %v\n", d)
    }
}

func TestEncodeMap(t *testing.T) {
    data := map[string]interface{}{"bar": "spam", "foo": 42}
    target := "d3:bar4:spam3:fooi42ee"
    actual, err := EncodeMap(data)
    if err != nil {
        unexpectedError(t, err)
    }
    if actual != target {
        t.Fatalf("Expect %v, got %v\n", target, actual)
    }
}

func TestDecodeInt(t *testing.T) {
    num, eat, err := decodeInt("i-42e")
    if err != nil {
        t.Fatalf("Expect -42, got error: %v\n", err)
    } else if num != -42 {
        t.Fatalf("Expect -42, got %d\n", num)
    }
    if eat != 5 {
        t.Fatalf("Expect advance 5 character, got %d\n", eat)
    }
}

func TestDecodeString(t *testing.T) {
    str, eat, err := decodeString("4:spam")
    if err != nil {
        t.Fatalf("Expect 'spam', got error: %v\n", err)
    } else if str != "spam" {
        t.Fatalf("Expect 'spam', got '%s'\n", str)
    }
    if eat != 6 {
        t.Fatalf("Expect advance 6 character, got %d\n", eat)
    }
}

func TestDecodeList(t *testing.T) {
    data := "l4:spami42ee"
    list, eat, err := decodeList(data)
    if err != nil {
        t.Fatalf("Expect ['spam',42], got error: %v\n", err)
    }
    if eat != len(data) {
        t.Fatalf("Expect advance %d character, got %d\n", len(data), eat)
    }
    if len(list) == 2 {
        if str, ok := list[0].(string); !ok || str != "spam" {
            t.Fatalf("First item is expected to be string 'spam', got %v[%T]\n", list[0], list[0])
        }
        if num, ok := list[1].(int); !ok || num != 42 {
            t.Fatalf("Second item is expected to be int 42, got %d[%T]\n", list[1], list[1])
        }
    } else {
        t.Fatalf("Expect ['spam',42], got %v\n", list)
    }
}

func TestDecodeMap(t *testing.T) {
    target := map[string]interface{}{"bar": "spam", "foo": 42}
    data := "d3:bar4:spam3:fooi42ee"
    mp, eat, err := decodeMap(data)
    if err != nil {
        t.Fatalf("Expect {\"bar\": \"spam\", \"foo\": 42}, got error: %v\n", err)
    } else if eat != len(data) {
        t.Fatalf("Expect advance %d character, got %d\n", len(data), eat)
    }

    for k, v := range mp {
        if v2, ok := target[k]; !ok || v != v2 {
            t.Fatalf("Expect get %v, actual get %v\n", v2, v)
        }
    }
}
