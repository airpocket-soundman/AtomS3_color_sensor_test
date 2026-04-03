# AtomS3_color_sensor_test

M5Stack AtomS3 + Color Unit のサンプルです。

`BtnA` を押すたびに Color Unit から色を読み取り、AtomS3 のディスプレイ全体をその色で塗りつぶします。

## 配線

Color Unit は Grove で AtomS3 の Port.A に接続します。

- SDA: GPIO 2
- SCL: GPIO 1
- I2C address: `0x29`

## ビルド

```powershell
pio run
```

## 書き込み

```powershell
pio run -t upload
```
