# OledBME
slim version of weatherradio for devices with small amount of memory

This is basically copied from Wolfgang Reissenberger's work on weatherradio https://github.com/sterne-jaeger/indi-3rdparty

weatherradio is a very nice weather station which can be used with indi https://indilib.org/about/discover-indi.html
or it can be used on its own.

weatherradio works very nicely with a wemos d1, but there have been some problems running it with an arduino uno or nano,
due to the low amount of memory available. This causes strange behaviour as more sensors are added and the memory becomes
tighter.

This rework, aims to permit use of more sensors with an arduino uno or nano.
At present it works with 
BME280
TSL2591
MLX90614
and
Davis anenometer.

If you have an interest in weatherradio, it works very nicely with the Wemos D1 mini, which is cheaper than an arduino Uno or a
nano. The wifi connection is thrown in for nothing and the memory available is much greater.

