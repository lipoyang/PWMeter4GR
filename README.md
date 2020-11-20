# PWMeter for GR

[Japanese README (和文ドキュメント)](README-ja.md)

Pulse width meter class library for GR-CITRUS / GR-ROSE

<br>

## PWMeter::begin(pin)
## PWMeter::begin(pin, polarity)
## PWMeter::begin(pin, polarity, resolution)
Begins measurement of pulse width.

### pin: pulse input pin
#### for GR-CITRUS
Pin 0, 1, 2, 3, 4, 7, 8, 10, 11, 12, 13 are supported for pulse input. They are divided into following groups. Same resolution setting is applied to same group pins. Just the last setting is effective.

- 0, 1 (using MTU1)
- 2, 3, 4, 10, 11, 12, 13 (using MTU3)
- 4 (using MTU4)
- 7, 8  (using MTU0)

Following pins can not be used together due to resource conflict.

- 2 and 11 (using MTIOC3C)
- 3 and 12 (using MTIOC3A)

#### for GR-ROSE
Pin 1, 3, 5, 7, 14(A0), 15(A1), 16(A2), 17(A3), 18(A4) are supported for pulse input. They are divided into following groups. Same resolution setting is applied to same group pins. Just the last setting is effective.

- 1 (using MTU1)
- 3, 7 (using MTU0)
- 5, 14 (using MTU4)
- 15, 16, 17, 18 (using MTU8)

Following pins can not be used together due to resource conflict.

- 5 と 14 (using MTIOC4D)

### polarity: polarity of pulse (optional)
Pulse polarity (positive or negative) is selectable. If omitted, the default is positive.
- PWMETER_POSITIVE: positive logic (default)
- PWMETER_NEGATIVE: negative logic

### resolution: resolution of measurement (optional)
The meter resolution is selectable from the following. If omitted, the default is 1usec.

#### for GR-CITRUS

- PWMETER_48TH_USEC: 1/48usec / Max  1365usec
- PWMETER_12TH_USEC: 1/12usec / Max  5461usec
- PWMETER_3RD_USEC: 1/3usec / Max 21845usec
- PWMETER_1USEC: 1usec / Max 21845usec (default)
- PWMETER_4_3RD_USEC: 4/3usec / Max 87380usec

#### for GR-ROSE
- PWMETER_60TH_USEC: 1/60usec / Max 1092usec
- PWMETER_15TH_USEC: 1/15usec / Max 4369usec
- PWMETER_4_15TH_USEC: 4/15usec / Max 17476usec
- PWMETER_1USEC: 1usec / Max 17476usec (default)
- PWMETER_32_15TH_USEC: 32/15usec / Max 139808usec

<br>

## PWMeter::available()
Checks whether or not new pulses are detected.

### returns: true or false

<br>

## PWMeter::get()
Gets the measured pulse width. Measurement values are buffered up to 32 items.

### returns: pulse width
LSB is equivalent to the resolution specified by PWMeter::begin(). If no new measurement value, it returns 0xFFFF.

<br>

## PWMeter::getLast()
Gets the last measured pulse width. Bufferd measurement values are discarded.

### returns: pulse width
LSB is equivalent to the resolution specified by PWMeter::begin(). If no new measurement value, it returns 0xFFFF.

<br>

## PWMeter::stop()

Stops mesurement.

<br>

## PWMeter::restart()

Restarts mesurement.

<br>

## Notes (for GR=CITRUS)
This class uses MTUs (Multi function timer pulse units). Therefore, it conflicts with PWM output (Servo library and analogWrite) of pin 0 and 11. Timer resouces used by Servo library and analogWrite are shown below.
- 0: MTU1 (conflicts with pulse width meter of pin 0, 1)
- 1: TPU3
- 5: TPU4
- 7: TPU0
- 8: TPU0
- 11: MTU3 (conflicts with pulse width meter of pin 2, 3, 4, 10, 11, 12, 13)
- others: TPU2

## Notes (for GR=ROSE)
This class uses MTUs (Multi function timer pulse units). Therefore, it conflicts with PWM output (Servo library and analogWrite) of pin 10 and 13. Timer resouces used by Servo library and analogWrite are shown below.
- 3: TPU5
- 7: TPU0
- 9: MTU2
- 10: MTU1 (conflicts with pulse width meter of pin 1)
- 13: MTU4 (conflicts with pulse width meter of pin 5, 14)
- others: TPU4
