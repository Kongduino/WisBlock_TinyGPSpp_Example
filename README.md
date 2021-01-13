# WisBlock_TinyGPSpp_Example

This is an adaptation of an existing TinyGPS++ example for [WisBlock](https://store.rakwireless.com/pages/wisblock): the main differences are:

* The initalization routine of the [RAK1910 GPS UBLOX7 module](https://store.rakwireless.com/products/rak1910-max-7q-gnss-location-sensor)
* SoftwareSerial is replaced by Serial1 – I kept the ss variable.

The main reason for this is to help you kick-start code with TinyGPS++, in replacement of the sample code we have in the official examples. There is a bug in the GPS code, [which I fixed](https://github.com/Kongduino/RAK1910_GPS_UBLOX7-Update), but I feel that we'd be better off with a trusted library.