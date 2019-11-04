# ESP8266 RGB-Strip controller Protocol (UDP)

The UDP Protocol is simply RGB bytes stitched together

```
|--------------------------------------------|
| Bytes  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
|--------|---|---|---|---|---|---|---|---|---|
| Colors | R | G | B | R | G | B | R | G | B |
|--------|-----------|-----------|-----------|
| LEDs   |   LED 1   |   LED 2   |   LED 3   |
|--------------------------------------------|
```

Sending `0x123456123456` to the LEDs will set the first two LEDs to `#123456` 

That Protocol is known in Hyperion as UDP Protocol 0
